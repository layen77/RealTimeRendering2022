#include "Settings.h"
#include "Window.h"
#include "Camera.h"
#include "Matrix.h"
#include "Vector.h"
#include "Scene.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Shader.h"
#include "Tools.h"
#include "Mesh.h"
#include "Material.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    float randR = 0.00f, randG = 0.00f, randB = 0.00f, randA = 0.00f;
    bool rotationCam = 0;
    bool sensRotationCam = 0;
    bool MeGaBaCkFliPdElAmOrTqUiTuE = 0;
    int numPerso = 1;

    //Choix du personnage
    printf("=========================================================================\n=                         CHOIX DU PERSONNAGE                           =\n=========================================================================\n");
    printf("1 - Bob l'Eponge\n2 - CaptainToad\n3 - Jaxy\n4 - Donald Trump (pas de normal map)\n5 - Shrek\n");
    printf("Entrez le numero du personnage que vous desirez\n");
    
    scanf("%d", &numPerso);

    Window *window = NULL;
    Renderer *renderer = NULL;
    Scene *scene = NULL;
    Mesh *mesh = NULL;

    // Initialise la SDL et crée la fenêtre
    int exitStatus = Settings_InitSDL();
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    window = Window_New();
    if (!window) goto ERROR_LABEL;

    renderer = Window_getRenderer(window);

    g_time = Timer_New();
    if (!g_time) goto ERROR_LABEL;

    // Crée la scène
    scene = Scene_New(window);
    if (!scene) goto ERROR_LABEL;

    switch(numPerso)//charge l'obj correspondant au choix réalisé plus haut
    {
        case(1):
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Bob", "spongebob.obj");
        break;

        case(2):
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/CaptainToad", "CaptainToad.obj");
        break;

        case(3):
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Jaxy", "Jaxy.obj");
        break;

        case(4):
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Trump", "Trump.obj");
        break;

        case(5):
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Shrek", "shrek.obj");
        break;

        default:
        mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Bob", "spongebob.obj");
        break;
    }
    //mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/CaptainToad", "CaptainToad.obj");
    if (!mesh) goto ERROR_LABEL;

    // Arbre de scène
    Object *root   = Scene_GetRoot(scene);
    Camera *camera = Scene_GetCamera(scene);

    // Crée l'objet
    Object *object = Scene_CreateObject(scene, sizeof(Object));
    if (!object) goto ERROR_LABEL;

    exitStatus = Object_Init(object, scene, Mat4_Identity, root);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    Object_SetMesh(object, mesh);

    // Calcule une échelle normalisée pour l'objet
    Vec3 meshCenter = mesh->m_center;
    Vec3 meshMin = mesh->m_min;
    Vec3 meshMax = mesh->m_max;
    float xSize = fabsf(meshMax.x - meshMin.x);
    float ySize = fabsf(meshMax.y - meshMin.y);
    float zSize = fabsf(meshMax.z - meshMin.z);
    float objectSize = fmaxf(xSize, fmaxf(ySize, zSize));
    float scale = 3.0f / objectSize;

    // Centre l'objet en (0,0,0) et applique l'échelle
    Mat4 objectTransform = Mat4_Identity;
    objectTransform = Mat4_GetTranslationMatrix(Vec3_Neg(mesh->m_center));
    objectTransform = Mat4_MulMM(Mat4_GetScaleMatrix(scale), objectTransform);
    Object_SetLocalTransform(object, objectTransform);

    // Lancement du temps global
    Timer_Start(g_time);

    // Paramètre initiaux de la caméra
    float camDistance = 7.3f;
    float camPos = 0.f;
    float angleY = 0.f;
    float angleX = 0.f;
    float angleZ = 0.f;

    float fpsAccu = 0.0f;
    int frameCount = 0;
    scene->m_normalMapOnOff = 1;
    bool quit = false;
    while (!quit)
    {
        SDL_Event evt;
        SDL_Scancode scanCode;

        // Met à jour le temps global
        Timer_Update(g_time);

        SDL_SetWindowGrab(window, 0);

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEWHEEL: // Scroll de la molette
                camDistance += (float)evt.wheel.y;
                break;

            case SDL_MOUSEMOTION: // déplacement relatif de la souris
                if (!MeGaBaCkFliPdElAmOrTqUiTuE)//le personnage va suivre les mouvements de la souris sauf dans le mode MegaBackflipDeLaMortQuiTue
                {
                    angleY -= (float)evt.motion.xrel * 0.25f;
                    angleX -= (float)evt.motion.yrel * 0.25f;
                    break;
                }

            case SDL_MOUSEBUTTONDOWN://boutons de la souris
                switch (evt.button.button)
                {
                case SDL_BUTTON_LEFT://Active ou non la NormalMap
                    scene->m_normalMapOnOff = !scene->m_normalMapOnOff;
                    break;
                case SDL_BUTTON_RIGHT://Active ou non la vue en arêtes
                    Scene_SetWireframe(scene, !Scene_GetWireframe(scene));
                    break;
                default:
                break;
                }

            case SDL_KEYDOWN:
                scanCode = evt.key.keysym.scancode;
                /*if (evt.key.repeat)
                    break;*/
                switch (scanCode)
                {
                case SDL_SCANCODE_ESCAPE://quitte le programme
                    quit = true;
                    break;
                case SDL_SCANCODE_LEFT://décale la caméra sur la gauche
                    camPos -= 0.2f;
                    break;
                case SDL_SCANCODE_RIGHT://décale la caméra sur la droite
                    camPos += 0.2f;
                    break;
                case SDL_SCANCODE_UP://augmente la luminosité de la scene
                    scene->m_lightColor.x += 1.f;
                    scene->m_lightColor.y += 1.f;
                    scene->m_lightColor.z += 1.f;
                    break;
                case SDL_SCANCODE_DOWN://diminue la luminosité de la scene
                    scene->m_lightColor.x -= 1.f;
                    scene->m_lightColor.y -= 1.f;
                    scene->m_lightColor.z -= 1.f;
                    break;
                case SDL_SCANCODE_C://On/Off de la rotation automatique du personnage
                    rotationCam = !rotationCam;
                    break;
                case SDL_SCANCODE_V://détermine le sens de rotation de la caméra en mode automatique
                    sensRotationCam = !sensRotationCam;
                    break;
                case SDL_SCANCODE_R://Réinitialise l'orientation du personnage
                    angleX = 0;
                    angleY = 0;
                    angleZ = 0;
                    break;
                case SDL_SCANCODE_SPACE://On/Off du mode MegaBackFlipDeLaMortQuiTue
                    MeGaBaCkFliPdElAmOrTqUiTuE = !MeGaBaCkFliPdElAmOrTqUiTuE;
                    break;
                default:
                    break;
            }
            break;

            default:
                break;
        }
    }
        if (MeGaBaCkFliPdElAmOrTqUiTuE)
        {
            angleX -= randB * 15.f;
            angleY -= randG * 15.f;
            angleZ -= randR * 15.f;
        }


         // Calcule la rotation de la caméra selon la direction choisie
        if(rotationCam == 1 && sensRotationCam == 0)
            angleY -= 360.f / 40.f * 3 * Timer_GetDelta(g_time);

        if (rotationCam == 1 && sensRotationCam == 1)
            angleY += 360.f / 40.f * 3 * Timer_GetDelta(g_time);

        // Calcule la matrice locale de la caméra
        Mat4 cameraModel = Mat4_Identity;
        cameraModel = Mat4_MulMM(
            Mat4_GetTranslationMatrix(Vec3_Set(camPos, 0.f, camDistance)),
            cameraModel
        );
        cameraModel = Mat4_MulMM(
            Mat4_GetYRotationMatrix(angleY),
            cameraModel
        );
        cameraModel = Mat4_MulMM(
            Mat4_GetXRotationMatrix(angleX),
            cameraModel
        );
        cameraModel = Mat4_MulMM(
            Mat4_GetZRotationMatrix(angleZ),
            cameraModel
        );

        // Applique la matrice locale de la caméra
        Object_SetTransform(((Object *)camera), Scene_GetRoot(scene), cameraModel);

        // Calcule le rendu de la scène dans un buffer
         Scene_Render(scene, randR, randG, randB, randA);

        // Met à jour le rendu (affiche le buffer précédent)
        Renderer_Update(renderer);

        // Calcule les FPS
        fpsAccu += Timer_GetDelta(g_time);
        frameCount++;
        if (fpsAccu > 1.0f)
        {
            printf("FPS = %.1f\n", (float)frameCount / fpsAccu);
            fpsAccu = 0.0f;
            frameCount = 0;

            //generation de floats randoms pour la couleur aléatoire du fond
            randR = ((float)rand()) / (float)RAND_MAX;
            randG = ((float)rand()) / (float)RAND_MAX;
            randB = ((float)rand()) / (float)RAND_MAX;
            randA = ((float)rand()) / (float)RAND_MAX;
        }
    }

    Scene_Free(scene);
    Timer_Free(g_time);
    Window_Free(window);

    Settings_QuitSDL();

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - main()\n");
    assert(false);
    Scene_Free(scene);
    Timer_Free(g_time);
    Window_Free(window);
    return EXIT_FAILURE;
}