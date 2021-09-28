#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef __LECTOR_H__
#define __LECTOR_H__



typedef struct lector{
	FILE* ptr;
}lector_t;


bool lector_abrir_archivo_lectura(lector_t* lector, char* stream);

void lector_cerrar_archivo(lector_t* lector);


FILE* lector_get_archivo(lector_t* lector);


ssize_t lector_leer_linea(lector_t* lector, char** line_buff, ssize_t* largo);

int lector_obtener_num_clientes(lector_t* lector, char* stream);

#endif
