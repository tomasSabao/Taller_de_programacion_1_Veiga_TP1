#include "lector.h"
#include <string.h>
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

int lector_obtener_num_clientes(lector_t* lector, char* stream){
    int contador = 0;
    FILE* aux_ptr =fopen(stream, "r");
    if (aux_ptr == NULL){
        return 0;
    }
    size_t largo =200;
    char* line_buff=NULL;
    while (true){
        ssize_t leyo=getline(&line_buff, &largo, aux_ptr);
        if (leyo == -1){
            break;
        }
        if (lector_obtener_caracteres(lector, line_buff, (int)largo) == 0){
            break;
        }
        contador++;
    }
    free(line_buff);
    fclose(aux_ptr);
    return contador;
}

ssize_t lector_leer_linea(lector_t* lector, char** line_buff, ssize_t* largo){
	ssize_t nread = getline(line_buff, (size_t*)largo, lector->ptr);
	return nread;
}


int lector_obtener_caracteres(lector_t* lector, char* ptr,int len_ptr){
    int caracteres=0;
    for (int i=0; i<len_ptr; i++){
        if (ptr[i] == '\n'){
            break;
        }
        caracteres++;
    }
    return caracteres;
}
