#include "pintor.h"
#include "bmp.h"

#include <stdio.h>

int main(void)
{
    sdl_config config;
    imagen_bmp* imagen = cargar_bmp("test.bmp");

    if (imagen == NULL)
    {
        fprintf(stderr, "No se pudo cargar test.bmp\n");
        return 1;
    }

    iniciar(&config, imagen->ancho, imagen->alto);

    for (uint32_t y = 0; y < imagen->alto; y++)
        for (uint32_t x = 0; x < imagen->ancho; x++)
            pintar(&config, x, y, &imagen->pixeles[y * imagen->ancho + x]);

    SDL_RenderPresent(config.render);

    uint32_t inicio = SDL_GetTicks();
    int salir = 0;
    while (!salir && SDL_GetTicks() - inicio < 5000)
    {
        SDL_Event evento;
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
                salir = 1;
        }
        SDL_Delay(16); // ~60 veces por segundo, para no consumir CPU al maximo
    }

    SDL_DestroyRenderer(config.render);
    SDL_DestroyWindow(config.ventana);
    SDL_Quit();
    destruir_bmp(imagen);

    return 0;
}