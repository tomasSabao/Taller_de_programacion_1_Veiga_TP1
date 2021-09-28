#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#ifndef __DECODIFICADOR_H__
#define __DECODIFICADOR_H__


typedef struct decodificador{
	int fd;
}decodificador_t;

//bool decodificador_obtener_fin_juego(decodificador_t* deco, void* ptr);

bool decodificador_obtener_juego_en_curso(decodificador_t* deco, void* ptr);

int decodificador_obtener_intentos(decodificador_t* deco, void* ptr);

int decodificador_obtener_longitud(decodificador_t* deco, void* ptr);

void decodificador_imprimir_mensaje(decodificador_t* deco, void* ptr);


void decodificador_imprimir_resultado(decodificador_t* deco, void* ptr);
//se usa solo al comienzo
void decodificador_imprimir_msj_inicio(decodificador_t* deco, void* ptr);

#endif
