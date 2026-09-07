//
// Created by avidal on 9/6/26.
//

#ifndef BMP_FILE_READER_BMP_H
#define BMP_FILE_READER_BMP_H

#pragma once

#include "pintor.h"
#include <stdint.h>

// Estructura que representa una imagen BMP ya cargada en memoria.
// Es el equivalente a "imagen_tga" pero para el formato BMP.
typedef struct
{
    uint32_t ancho;
    uint32_t alto;
    color* pixeles;   // arreglo de ancho*alto colores, guardado fila por fila,
    // de arriba hacia abajo (ya normalizado, sin importar
    // como estaba realmente ordenado en el archivo)
} imagen_bmp;

// Carga un archivo .bmp de 24 bits sin compresion desde disco.
// Devuelve NULL (e imprime un mensaje de error) si el archivo no existe,
// no es un BMP valido, o usa una variante no soportada por este cargador.
imagen_bmp* cargar_bmp(const char* nombre_archivo);

// Libera toda la memoria reservada por cargar_bmp.
void destruir_bmp(imagen_bmp* imagen);

#endif //BMP_FILE_READER_BMP_H
