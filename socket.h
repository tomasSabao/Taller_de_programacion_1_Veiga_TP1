#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__


//skt: el filedescriptor del socket creado
//modo: si es cliente o servidor
typedef struct socket{
	int skt;
	struct addrinfo hints;
	struct addrinfo *result;
	char modo;
}socket_t;



int socket_set_hints(socket_t *sock,char modo);

//PRE: set_hints ejecutado de antemano
int socket_set_addrinfo(socket_t* sock, char* host, char* port);

//PRE: socket seteado en modo servidor
int socket_crear_y_bindear(socket_t* sock);

//PRE: se tuvo que haber llamado a socket_set_addrinfo
//socket en modo cliente
int socket_crear_y_conectar(socket_t* sock);

int socket_enviar(socket_t* sock, char* buffer, int tam_buffer);

int socket_escuchar(socket_t* sock);

int socket_aceptar(socket_t* bound_sock, socket_t* accept_sock);

int socket_recibir(socket_t* sock, char* buffer, int tamanio_msg);

//int socket_recibir(socket_t* sock, char* buffer);

void socket_cerrar(socket_t *sock);




#endif 