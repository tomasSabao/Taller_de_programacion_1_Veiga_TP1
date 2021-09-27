#include "codificador.h"

void codificador_imprimir_mensaje(codificador_t* cod, ahorcado_t* juego){
	printf("FUNCION IMPRIMIR MENSAJE \n");
	unsigned char intentos = juego->intentos;
	printf("numero de intentos: %d\n", intentos);
	bool victoria = ahorcado_chequear_victoria(juego);
	bool derrota = ahorcado_chequear_derrota(juego);
	unsigned char flag_fin = 0x80;
	if (victoria || derrota){
		intentos = intentos | flag_fin;
	}
	printf("Primer byte: %X\n", intentos);
}


int codificador_generar_mensaje(codificador_t* cod, ahorcado_t* juego,
								void** msj, int* tamanio_ptr, bool fin){
	int tamanio_mensaje = juego->longitud + 3;
	void* aux_ptr = NULL;
	if (*tamanio_ptr != tamanio_mensaje){
		aux_ptr = realloc(*msj, sizeof(char) * tamanio_mensaje);
		if (aux_ptr == NULL){
			//fallo la redimension
			return -1;
		}
		*msj = aux_ptr;
		*tamanio_ptr = tamanio_mensaje;
	}
	unsigned char intentos = juego->intentos;
	bool victoria = ahorcado_chequear_victoria(juego);
	bool derrota = ahorcado_chequear_derrota(juego);
	unsigned char flag_fin = 0x80;
	if (victoria || derrota){
		intentos = intentos | flag_fin;
	}
	uint16_t long_msj;
	long_msj = htons(juego->longitud);
	//copiamos ahora 
	char* msj_ptr = *((char**)msj);
	memcpy(msj_ptr, &intentos, 1);
	memcpy(msj_ptr + 1, &long_msj, 2);
	if (fin == true){
		memcpy(msj_ptr + 3, juego->palabra, juego->longitud);
	}
	else{
		memcpy(msj_ptr + 3, juego->incognita, juego->longitud);
	}
	return 0;
}	