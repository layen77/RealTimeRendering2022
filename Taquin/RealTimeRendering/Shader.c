#include "Shader.h"
#include "Scene.h"
#include "Graphics.h"
#include "Tools.h"

VShaderOut VertexShader_Base(VShaderIn *in, VShaderGlobals *globals)
{
    // Le vertex shader est une fonction ex�cut�e pour chacun des sommets
    // d'un objet � rendre.
    // Il r�cup�re des donn�es globales � l'objet comme les matrices de conversion.
    // Il se charge principalement de projeter le sommet dans le "clip space"
    // et de convertir la normale/tangente associ�e dans le r�f�rentiel monde.
    // Le vertex shader est ex�cut� pour chacun des sommets d'un objet � rendre.
    VShaderOut out = { 0 };
    
    // Coordonn�es homog�nes du sommet (w = 1 car c'est un point)
    Vec4 vertex = Vec4_From3(in->vertex, 1.0f);

    // Coordonn�es homog�nes de la normale (w = 0 car c'est une direction)
    Vec4 normal = Vec4_From3(in->normal, 0.0f);

    // Coordonn�es homog�nes de la tangent
    Vec4 tangent = Vec4_From3(in->tangent, 0.0f);

    // Transformation du sommet dans le rep�re cam�ra
    Vec4 vertexCamSpace = Mat4_MulMV(globals->objToView, vertex);

    // Projection du sommet dans le "clip space"
    Vec4 vertexClipSpace = Mat4_MulMV(globals->objToClip, vertex); // OBLIGATOIRE (ne pas modifier)

    // TODO
    // Pour la lumi�re diffuse, il faut transformer la normale dans le rep�re monde
    // Pour le mod�le de Blinn-Phong, il faut calculer la position du sommet
    // dans le r�f�rentiel monde et ajouter l'information au VShaderOut

    // D�finit la sortie du vertex shader
    out.clipPos = Vec3_From4(vertexClipSpace);  // OBLIGATOIRE (ne pas modifier)
    out.invDepth = vertexCamSpace.w / vertexCamSpace.z; // OBLIGATOIRE (ne pas modifier)
    out.normal = Vec3_Normalize(Vec3_From4(normal));
    out.textUV = in->textUV;
    out.tangent = Vec3_Normalize(Vec3_From4(tangent));
    return out;
}

Vec4 FragmentShader_Base(FShaderIn *in, FShaderGlobals *globals)
{
    // Le fragment shader est une fonction ex�cut�e pour chaque pixel
    // de tous les triangles visibles.
    // Il r�cup�re un fragment qui contient des donn�es interpol�es
    // (position, normale, tangente, uv).
    // Il retourne la couleur du pixel.
    //
    // Le vecteur (x,y,z,w) renvoy� par le fragment shader est interpr�t� comme suit :
    // - x : intensit� du rouge (entre 0 et 1)
    // - y : intensit� du vert  (entre 0 et 1)
    // - z : intensit� du bleu  (entre 0 et 1)
    // - w : opacit�            (entre 0 et 1)

    // R�cup�ration du mat�riau associ� au pixel (albedo/normal map)
    Material* material = globals->material;
    assert(material);

    // R�cup�ration de la texture (albedo) associ�e au pixel
    MeshTexture* albedoTex = Material_GetAlbedo(material);
    assert(albedoTex);

    MeshTexture* normalTex = Material_GetNormalMap(material);
    if (normalTex)
        assert(normalTex);


    // R�cup�ration des coordonn�es (u,v) associ�e au pixel.
    // Les coordonn�es (u,v) sont dans [0,1]^2.
    // - u repr�sente l'abscisse du point sur une texture 2D.
    // - v repr�sente l'ordonn�e du point sur une texture 2D.
    // (0,0) repr�sente le coin en bas � gauche.
    // (1,1) repr�sente le coin en haut � droite.
    float u = in->textUV.x;
    float v = in->textUV.y;

    // Recup�ration de la couleur du pixel dans la texture
    Vec3 albedo = MeshTexture_GetColorVec3(albedoTex, Vec2_Set(u, v));


#if 1
    // R�cup�re les lumi�res de la sc�ne
    Vec3 lightVector = Scene_GetLight(globals->scene);
    Vec3 lightColor = Scene_GetLightColor(globals->scene);
    Vec3 ambiant = Scene_GetAmbiantColor(globals->scene);

    // R�cup�re la normale interpol�e (non normalis�e)
    Vec3 normal = in->normal;
    Vec3 view = { 0 };
    Vec3 half = { 0 };
    Vec3 tangent = in->tangent;
    Vec3 bitangent = { 0 };
    Vec3 tangentSpaceNormal = { 0 };

    tangent = Vec3_Normalize(tangent);
    normal = Vec3_Normalize(normal);

    if (normalTex && globals->scene->m_normalMapOnOff) {
        tangentSpaceNormal = MeshTexture_GetColorVec3(normalTex, Vec2_Set(u, v));
        bitangent = Vec3_Cross(normal, tangent);
        bitangent = Vec3_Normalize(bitangent);

        Mat3 tanToWrld = {
        tangent.x, bitangent.x , normal.x ,
        tangent.y, bitangent.y , normal.y ,
        tangent.z, bitangent.z , normal.z
        };
        //normal = Vec3_Add(Vec3_Add(Vec3_Scale(tangent, tangentSpaceNormal.x), Vec3_Scale(bitangent, tangentSpaceNormal.y)), Vec3_Scale(bitangent, tangentSpaceNormal.z));

        normal = Mat3_MulMV(tanToWrld, tangentSpaceNormal);
        normal = Vec3_Normalize(normal);
    }

    float lambert = Float_Clamp01(Vec3_Dot(normal, lightVector));
    Vec3 cameraPos = globals->cameraPos;
    view = Vec3_Sub(in->worldPos, cameraPos);
    view = Vec3_Normalize(view);
    half = Vec3_Add(lightVector, view);
    half = Vec3_Normalize(half);


    float coeff = Vec3_Dot(normal, lightColor);
    float coeffBlin = Vec3_Dot(normal, half) * (lambert > 0);



    float alpha = 4.f;
    coeffBlin = (float) pow(Float_Clamp01(coeffBlin), alpha);

    // Application de la lumi�re ambiante � l'albedo et blinn phong
    albedo = Vec3_Mul(albedo, (Vec3_Add(ambiant,
        Vec3_Scale(lightColor, coeffBlin))));


    // TODO
    // Pour la lumi�re diffuse, il faut utiliser la normale.
    // Pour la lumi�re sp�culaire de Blinn-Phong, il faut :
    // - r�cup�rer l'interpolation de la position dans le monde du pixel ;
    // - calculer le vecteur de vue, le vecteur moiti�
    // Vous devez donc modifier le vertex shader, puis modifier la fonction
    // Graphics_RenderTriangle() pour initialiser l'interpolation puis pour
    // calculer l'interpolation.
    // Utilisez les macros VEC3_INIT_INTERPOLATION() et VEC3_INTERPOLATE().
#endif

    //.............................................................................................
    // Quelques exemples de debug (� d�commenter)

    // Debug : couleur unique en RGBA (bleu ESIEA)
    //return Vec4_Set(0.21f, 0.66f, 0.88f, 1.0f);

    // Debug : coordonn�es (uv) de texture
    //return Vec4_From2(in->textUV, 0.0, 1.0f);

    // Debug : normales
    //return Vec4_From3(in->normal, 1.0f);

    // Debug : normales V2 
    //return Vec4_From3(Vec3_Scale(Vec3_Add(in->normal, Vec3_One), 0.5f), 1.0f);
    //.............................................................................................

    // Retourne la couleur (albedo) associ�e au pixel dans la texture.
    return Vec4_From3(albedo, 1.0f);
}
