#include <stdio.h>
#include <stdlib.h>
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__



typedef struct input_manager{
	int fd;
}in_manager_t;

//PRE: *str tiene que estar asignado dinamicamente
//size es la longitud en bytes de str
//en teoria size > 0
int in_manager_leer_palabra(in_manager_t* man, void** str, int* size);

#endif