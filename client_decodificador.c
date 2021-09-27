#include "decodificador.h"

bool decodificador_obtener_fin_juego(decodificador_t* deco, void* ptr){
	unsigned char estado;
	memcpy(&estado, ptr, 1);
	estado = estado >> 4;
	if(estado == 8){
		return false;
	}
	return true;
}

//lado servidor

bool decodificador_obtener_juego_en_curso(decodificador_t* deco, void* ptr){
	unsigned char estado;
	memcpy(&estado, ptr, 1);
	estado = estado >> 4;
	if(estado == 8){
		return false;
	}
	return true;
}

int decodificador_obtener_intentos(decodificador_t* deco, void* ptr){
	unsigned char intentos;
	memcpy(&intentos, ptr, 1);
	intentos = intentos << 4;
	intentos = intentos >> 4;
	return intentos;
}

int decodificador_obtener_longitud(decodificador_t* deco, void* ptr){
	uint16_t longitud;
	memcpy(&longitud, (char*)ptr + 1, 2);
	longitud = ntohs(longitud);
	return longitud;
}
//lado servidor

void decodificador_imprimir_mensaje(decodificador_t* deco, void* ptr){
	//int fin_juego=decodificador_obtener_fin_juego(deco,ptr);
	int intentos = decodificador_obtener_intentos(deco, ptr);
	int longitud = decodificador_obtener_longitud(deco, ptr);
	printf("Valor de longitud: %d\n",longitud);
	char* aux_ptr=(char*)ptr;
	printf("Palabra secreta: ");
	for (int i=0; i<longitud; i++){
		if (i == (longitud -1)){
			printf("%c\n", aux_ptr[3+i]);
		} else{
			printf("%c", aux_ptr[3+i]);
		}
	}
	printf("Te quedan %d intentos\n",intentos);
	printf("Ingrese letra:");
}


void decodificador_imprimir_resultado(decodificador_t* deco, void* ptr){
	int intentos = decodificador_obtener_intentos(deco, ptr);
	if (intentos > 0){
		printf("Ganaste!\n");
		return;
	}
	char* aux_ptr = (char*)ptr;
	int longitud = decodificador_obtener_longitud(deco, ptr);
	printf("Perdiste! La palabra secreta era: ");
	for (int i=0; i<longitud; i++){
		if (i == (longitud-1)){
			printf("%c\n",aux_ptr[3+i]);
		} else {
			printf("%c",aux_ptr[3+i]);
		}
	}
	return;
}
//se usa solo al comienzo
void decodificador_imprimir_msj_inicio(decodificador_t* deco, void* ptr){
	int intentos = decodificador_obtener_intentos(deco, ptr);
	int longitud = decodificador_obtener_longitud(deco, ptr);
	printf("Palabra secreta: ");
	for (int i=0; i<longitud; i++){
		if (i == (longitud-1)){
			printf("_\n");
		} else {
			printf("_");
		}
	}
	printf("Te quedan %d intentos\n",intentos);
	printf("Ingrese letra:");
}