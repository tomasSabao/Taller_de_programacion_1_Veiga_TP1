#include "input_manager.h"
//PRE: *str tiene que estar asignado dinamicamente
//size es la longitud en bytes de str
//en teoria size > 0
int in_manager_leer_palabra(in_manager_t* man, void** str, int* size){
	int largo=0;
	void* aux_ptr=NULL;
	int aux;
	while(largo == 0){
		int ch;
		while((ch = getc(stdin)) && ch !=EOF &&  ch !='\n'){
			*((*(char**)str) + largo) = (char)ch;
			//aumentamos el numero de carcteres leidos en uno
			largo++;
			if( (*size) == largo ){//sin espacio
				//decido que quiero duplicar el espacio
				//reviso si puedo redimensionar
				aux=(*size)*2;
				aux_ptr=realloc(*str, sizeof(char) * aux);
				//falla la redimension
				if(aux_ptr == NULL){
					return -1;
				} else{
					*str=aux_ptr;
					(*size) = aux;
				}
			}
		}
	}	
	//puede que me sobre espacio, reduzco el largo
	//al largo en caracteres, sumo uno para la longitud 
	//de la palabra/s ingresada/s
	aux_ptr=realloc(*str, sizeof(char) * largo);
	if (aux_ptr == NULL){;
		return -1;
	}
	*str=aux_ptr;
	(*size)=largo;
	return 0;
}
