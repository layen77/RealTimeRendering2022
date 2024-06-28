#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Settings.h"
#include "Vector.h"

typedef struct Renderer_s
{
    /// @protected
    /// @brief Le moteur de rendu SDL.
    SDL_Renderer *m_rendererSDL;

    /// @protected
    /// @brief La largeur en pixels de l'image rendue.
    int m_width;

    /// @protected
    /// @brief La hauteur en pixels de l'image rendue.
    int m_height;

    /// @protected
    /// @brief Le z-buffer (buffer de profondeur).
    float **m_zBuffer;

    /// @protected
    /// @brief Texture en acc�s streaming dans laquelle copi� le rendu.
    SDL_Texture *m_streamTex;

    /// @protected
    /// @brief Tableau des pixels.
    Uint32 *m_pixels;
} Renderer;

Renderer *Renderer_New(SDL_Renderer *rendererSDL);
void Renderer_Free(Renderer *renderer);

/// @ingroup Renderer
/// @brief Renvoie la largeur du moteur de rendu.
/// @param[in] renderer le moteur de rendu.
/// @return La largeur du moteur de rendu.
INLINE int Renderer_GetWidth(Renderer *renderer)
{
    return renderer->m_width;
}

/// @ingroup Renderer
/// @brief Renvoie la hauteur du moteur de rendu.
/// @param[in] renderer le moteur de rendu.
/// @return La hauteur du moteur de rendu.
INLINE int Renderer_GetHeight(Renderer *renderer)
{
    return renderer->m_height;
}

/// @ingroup Renderer
/// @brief D�finit la couleur d'un pixel sur le rendu.
/// La position (x = 0, y = 0) d�signe le point en haut � gauche de l'�cran.
/// Le pixel n'est modifi� que si sa profondeur est inf�rieure � la valeur associ�
/// dans le buffer de profondeur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
/// @param pixel position du pixel � d�finir.
/// @param color la couleur du pixel.
//void Renderer_SetPixelPre(Renderer *renderer, Pixel pixel, Vec4 color);
void Renderer_SetPixel(Renderer *renderer, int x, int y, float zValue, Vec4 color, bool zWrite);

void Renderer_DrawLine(Renderer *renderer, Vec3 p0, Vec3 p1, Vec4 color);

/// @ingroup Renderer
/// @brief R�initialise le buffer de profondeur du moteur de rendu.
/// @param[in,out] renderer le moteur de rendu.
void Renderer_ResetDepthBuffer(Renderer *renderer);

/// @ingroup Renderer
/// @brief Remplit le rendu avec une couleur donn�e.
/// @param[in,out] renderer le moteur de rendu.
/// @param[in] color la couleur de remplissage.
void Renderer_Fill(Renderer *renderer, Vec4 color);

/// @ingroup Renderer
/// @brief Met � jour la fen�tre avec le rendu calcul�.
/// @param[in,out] renderer le moteur de rendu.
void Renderer_Update(Renderer *renderer);

#endif
