#include "ahorcado.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#ifndef __CODIFICADOR_H__
#define __CODIFICADOR_H__

//Para uso del servidor
typedef struct codificador{
	int fd;
}codificador_t;


void codificador_imprimir_mensaje(codificador_t* cod, ahorcado_t* juego);


int codificador_generar_mensaje(codificador_t* cod, ahorcado_t* juego,
								void** msj, int* tamanio_ptr, bool fin);	

#endif
