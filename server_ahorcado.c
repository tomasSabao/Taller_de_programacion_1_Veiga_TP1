#include "ahorcado.h"

int ahorcado_get_intentos(ahorcado_t* juego){
	return juego->intentos;
}

void ahorcado_set_intentos(ahorcado_t* juego, int intentos){
	juego->intentos=intentos;
}

void ahorcado_reducir_intentos(ahorcado_t* juego){
	juego->intentos=juego->intentos - 1;
}

void ahorcado_set_longitud(ahorcado_t* juego,int longitud){
	juego->longitud=longitud;
}

void ahorcado_imprimir_estado(ahorcado_t* juego){
	printf("\n");
    printf("Palabra secreta: %.*s\n", juego->longitud, juego->incognita);
    printf("Le quedan %d intentos\n", juego->intentos);
    printf("Ingresar letra: ");
}

bool ahorcado_chequear_letra(ahorcado_t* juego, char c){
	bool esta=false;
	char* palabra=juego->palabra;
	int largo=juego->longitud;
	for (int j=0; j<largo; j++){
        if (palabra[j] == c){
            esta=true;
            break;
        }
    }
    return esta;
}

void ahorcado_reemplazar_letra(ahorcado_t* juego, char letra){
	char* palabra=juego->palabra;
	char* incognita=juego->incognita;
	for (int i=0; i<(juego->longitud); i++){
		if(palabra[i] == letra){
			incognita[i]=letra;
		}
	}
}


bool ahorcado_quedan_letras(ahorcado_t* juego){
	char* incognita=juego->incognita;
	int largo=juego->longitud;
	for (int j=0; j<largo; j++){
        if (incognita[j] == '_'){
            return true;
        }
    }
    return false;
}

void ahorcado_set_palabra(ahorcado_t* juego, char* palabra, int largo){
	juego->palabra=palabra;
	juego->longitud=largo;
}

//PRE:haber llamado a ahorcado set palabra 
void ahorcado_set_incognita(ahorcado_t* juego, char* ptr){
	juego->incognita=ptr;
}

void ahorcado_formatear_incognita(ahorcado_t* juego){
	for (int i=0; i<juego->longitud; i++){
		(juego->incognita)[i]='_';
	}
}

bool ahorcado_chequear_derrota(ahorcado_t* juego){
	return (juego->intentos == 0);
}

bool ahorcado_chequear_victoria(ahorcado_t* juego){
	bool quedan_letras = ahorcado_quedan_letras(juego);
	return !quedan_letras;
}

void ahorcado_aumentar_victorias(ahorcado_t* juego){
	juego->victorias++;
}
void ahorcado_aumentar_derrotas(ahorcado_t* juego){
	juego->derrotas++;
}

void ahorcado_imprimir_resultados(ahorcado_t* juego){
	printf("Resumen:\n");
	printf("\tVictorias: %d\n",juego->victorias);
	printf("\tDerrotas: %d\n", juego->derrotas);
}