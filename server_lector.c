#include "lector.h"

bool lector_abrir_archivo_lectura(lector_t* lector, char* stream){
    FILE* aux_ptr = fopen(stream, "r");
    if (aux_ptr == NULL){
        return false;
    }
    lector->ptr = aux_ptr;
    return true;
}

void lector_cerrar_archivo(lector_t* lector){
	fclose(lector->ptr);
}


FILE* lector_get_archivo(lector_t* lector){
	return lector->ptr;
}


ssize_t lector_leer_linea(lector_t* lector, char** line_buff, ssize_t* largo){
	ssize_t nread = getline(line_buff, (size_t*)largo, lector->ptr);
	return nread;
}
