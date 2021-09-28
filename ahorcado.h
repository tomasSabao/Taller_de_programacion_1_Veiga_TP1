#include <stdbool.h>
#include <stdio.h>
#ifndef __AHORCADO_H__
#define __AHORCADO_H__

typedef struct ahorcado{
	int intentos;
	char* palabra;//es la palabra que se quiere adivinar
	char* incognita;//es el array con _
	int longitud;//longitud de la incognita en caracteres
	int victorias;
	int derrotas;
}ahorcado_t;


//int ahorcado_get_intentos(ahorcado_t* juego);

void ahorcado_set_intentos(ahorcado_t* juego, int intentos);

void ahorcado_reducir_intentos(ahorcado_t* juego);

//void ahorcado_set_longitud(ahorcado_t* juego,int longitud);

//void ahorcado_imprimir_estado(ahorcado_t* juego);

bool ahorcado_chequear_letra(ahorcado_t* juego, char c);

void ahorcado_reemplazar_letra(ahorcado_t* juego, char letra);

void ahorcado_iniciar_historial(ahorcado_t* juego);

bool ahorcado_quedan_letras(ahorcado_t* juego);

void ahorcado_set_palabra(ahorcado_t* juego, char* palabra, int largo);

//PRE:haber llamado a ahorcado set palabra 
void ahorcado_set_incognita(ahorcado_t* juego, char* ptr);

void ahorcado_formatear_incognita(ahorcado_t* juego);

bool ahorcado_chequear_derrota(ahorcado_t* juego);

bool ahorcado_chequear_victoria(ahorcado_t* juego);

void ahorcado_aumentar_victorias(ahorcado_t* juego);

void ahorcado_aumentar_derrotas(ahorcado_t* juego);

void ahorcado_imprimir_resultados(ahorcado_t* juego);

#endif
