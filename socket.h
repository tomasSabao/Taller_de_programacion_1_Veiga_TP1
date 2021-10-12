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
}socket_t;

//int socket_set_hints(socket_t *sock,char modo);

//int socket_set_addrinfo(socket_t* sock, char* host, char* port);

//int socket_crear_y_bindear(socket_t* sock);

//int socket_crear_y_conectar(socket_t* sock);

int socket_enviar(socket_t* sock, char* buffer, int tam_buffer);

int socket_escuchar(socket_t* sock);

int socket_aceptar(socket_t* bound_sock, socket_t* accept_sock);

int socket_recibir(socket_t* sock, char* buffer, int tamanio_msg);

void socket_cerrar(socket_t *sock);

int socket_enlazar(socket_t* sock, struct addrinfo* result);

int socket_enlazar_y_escuchar(socket_t* sock, struct addrinfo* result);

int socket_conectar(socket_t* sock, struct addrinfo* result);

#endif 
