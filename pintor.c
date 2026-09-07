#include "pintor.h"
#include <SDL2/SDL.h>



void iniciar(sdl_config *config, int ancho, int alto)
{
    SDL_Init(SDL_INIT_VIDEO);
    config->ventana = SDL_CreateWindow("Mi Imagen",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        ancho, alto, 0);

    config->render = SDL_CreateRenderer(config->ventana, -1, 0);


}


void pintar(sdl_config *config, int x, int y, color* mi_color)
{
    SDL_SetRenderDrawColor(config->render,
            mi_color->rojo,
            mi_color->verde,
            mi_color->azul,
            255);

    SDL_RenderDrawPoint(config->render, x, y);

}