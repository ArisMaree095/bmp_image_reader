//
// Created by avidal on 9/6/26.
//

#ifndef BMP_FILE_READER_PINTOR_H
#define BMP_FILE_READER_PINTOR_H

#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Window* ventana;
    SDL_Renderer* render;
} sdl_config;

typedef struct
{
    uint8_t rojo;
    uint8_t verde;
    uint8_t azul;
} color;

void pintar(sdl_config *config, int x, int y, color* mi_color);

void iniciar(sdl_config *config, int ancho, int alto);

#endif //BMP_FILE_READER_PINTOR_H
