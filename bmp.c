#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
	uint16_t tipo;            // debe ser 0x4D42 ("BM" en ASCII, little-endian)
	uint32_t tamano_archivo;  // tamano total del archivo en bytes
	uint16_t reservado1;
	uint16_t reservado2;
	uint32_t offset_datos;    // posicion (en bytes) donde empiezan los pixeles
} CabeceraArchivoBMP;

typedef struct
{
	uint32_t tamano_cabecera;       // tamano de esta cabecera (40 para BITMAPINFOHEADER)
	int32_t  ancho;                 // ancho en pixeles
	int32_t  alto;                  // alto en pixeles (negativo = almacenado de arriba hacia abajo)
	uint16_t planos;                // siempre 1
	uint16_t bits_por_pixel;        // 1, 4, 8, 24, 32...
	uint32_t compresion;            // 0 = sin compresion (BI_RGB)
	uint32_t tamano_imagen;         // puede ser 0 si compresion == 0
	int32_t  resolucion_horizontal;
	int32_t  resolucion_vertical;
	uint32_t colores_usados;
	uint32_t colores_importantes;
} CabeceraInfoBMP;

#pragma pack(pop)

#define FIRMA_BMP 0x4D42  // "BM" leido como uint16_t little-endian

imagen_bmp* cargar_bmp(const char* nombre_archivo)
{
	CabeceraArchivoBMP cabecera_archivo;
	CabeceraInfoBMP cabecera_info;
	imagen_bmp* imagen;

	FILE* archivo = fopen(nombre_archivo, "rb");
	if (archivo == NULL)
	{
		printf("Error al cargar archivo %s\n", nombre_archivo);
		return NULL;
	}

	// Leemos las dos cabeceras de una sola vez cada una.
	fread(&cabecera_archivo, sizeof(CabeceraArchivoBMP), 1, archivo);
	fread(&cabecera_info, sizeof(CabeceraInfoBMP), 1, archivo);

	// Validaciones basicas: si esto falla, no es un BMP o es una variante
	// que este cargador (a proposito, simple) no soporta.
	if (cabecera_archivo.tipo != FIRMA_BMP)
	{
		printf("El archivo %s no es un BMP valido\n", nombre_archivo);
		fclose(archivo);
		return NULL;
	}

	if (cabecera_info.bits_por_pixel != 24 || cabecera_info.compresion != 0)
	{
		printf("Solo se soportan BMP de 24 bits sin compresion (bits=%u, compresion=%u)\n",
		       cabecera_info.bits_por_pixel, cabecera_info.compresion);
		fclose(archivo);
		return NULL;
	}

	imagen = malloc(sizeof(imagen_bmp));
	imagen->pixeles = NULL;

	// El alto puede venir negativo: eso indica que las filas ya estan
	// almacenadas de arriba hacia abajo en el archivo (top-down), en vez
	// del orden normal de BMP que es de abajo hacia arriba (bottom-up).
	int es_de_arriba_hacia_abajo = cabecera_info.alto < 0;
	imagen->ancho = (uint32_t)cabecera_info.ancho;
	imagen->alto  = (uint32_t)(es_de_arriba_hacia_abajo ? -cabecera_info.alto : cabecera_info.alto);

	uint32_t cantidad_pixeles = imagen->ancho * imagen->alto;
	imagen->pixeles = malloc(cantidad_pixeles * sizeof(color));

	// Cada fila de pixeles en BMP se rellena (padding) para que su tamano
	// en bytes sea multiplo de 4. Como cada pixel ocupe 3 bytes (B,G,R),
	// calculamos cuantos bytes de relleno hay al final de cada fila.
	uint32_t bytes_por_fila_sin_relleno = imagen->ancho * 3;
	uint32_t relleno = (4 - (bytes_por_fila_sin_relleno % 4)) % 4;

	// Nos movemos hasta donde el archivo dice que empiezan los pixeles
	fseek(archivo, cabecera_archivo.offset_datos, SEEK_SET);

	for (uint32_t fila = 0; fila < imagen->alto; fila++)
	{
		// Si el BMP es bottom-up (el caso normal), la primera fila leida
		// del archivo corresponde a la ULTIMA fila de la imagen. Aqui
		// calculamos en que fila del arreglo "pixeles" debe ir.
		uint32_t fila_destino = es_de_arriba_hacia_abajo ? fila : (imagen->alto - 1 - fila);

		for (uint32_t columna = 0; columna < imagen->ancho; columna++)
		{
			uint8_t bgr[3];
			color pixel;

			// Los colores en BMP se almacenan en orden BGR (azul, verde, rojo),
			// igual que en TGA.
			fread(bgr, sizeof(uint8_t), sizeof(bgr), archivo);

			pixel.azul  = bgr[0];
			pixel.verde = bgr[1];
			pixel.rojo  = bgr[2];

			imagen->pixeles[fila_destino * imagen->ancho + columna] = pixel;
		}

		// Saltamos los bytes de relleno al final de la fila, si los hay.
		if (relleno > 0)
		{
			fseek(archivo, relleno, SEEK_CUR);
		}
	}

	fclose(archivo);
	return imagen;
}

void destruir_bmp(imagen_bmp* imagen)
{
	if (imagen == NULL)
	{
		return;
	}

	free(imagen->pixeles);
	free(imagen);
}