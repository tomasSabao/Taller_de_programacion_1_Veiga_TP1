#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__

#include "ahorcado.h"
#include "codificador.h"
#include "socket.h"
#include "lector.h"
#include "socket.h"
#include <stdbool.h>

void servidor_generar_mensaje(ahorcado_t* juego, void** msj, int* tamanio_ptr){
	codificador_t cod;
	if (ahorcado_chequear_derrota(juego)){
		codificador_generar_mensaje(&cod, juego, msj, tamanio_ptr, true);
	} else {
		codificador_generar_mensaje(&cod, juego, msj, tamanio_ptr, false);
	}
}

int servidor_enviar_mensaje(socket_t* skt, void* buff, int largo, bool primer){
	int bytes_enviados;
	if (primer == true){
		bytes_enviados=socket_enviar(skt, (char*)buff, 3);
		if (bytes_enviados == -1){
			return 1;
		}
		socket_enviar(skt, ((char*)buff)+3, largo-3);
		return 0;
	} else{
		bytes_enviados = socket_enviar(skt, (char*)buff, largo);
		if (bytes_enviados == -1){
			return 1;
		}
	}
	return 0;
}

int servidor_recibir_mensaje(socket_t* skt, char* buffer){
	int bytes_recibidos = socket_recibir(skt, buffer, 1);
	if (bytes_recibidos == -1){
		return 1;
	}
	return 0;
}

int servidor_verificar_parametros(int argc, char** argv){
	if (argc != 4){
		return 1;
	}
	int intentos = atoi((const char*)argv[2]);
	if (intentos <= 0 || intentos >127){
		return 1;
	}
	return 0;
}


void servidor_cerrar_fd(){
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}
//devuelve 1 si falla la lectura del archivo
//devuelve 2 si falla la aceptacion del socket
//devuelve 0 si todo funciona correctamente
int servidor_leer_linea_aceptar_cliente(lector_t* ptr_lec, char** linea, ssize_t len,
	ssize_t* ptr_nread, socket_t* skt, socket_t* aceptado){
	*ptr_nread = lector_leer_linea(ptr_lec, linea, &len);
	if (*ptr_nread == -1){
		lector_cerrar_archivo(ptr_lec);
		socket_cerrar(aceptado);
		socket_cerrar(skt);
		free(*linea);
		return 1;
	}
	int funciono = socket_aceptar(skt, aceptado);
	if (funciono == -1){
		return 2;
	}
	return 0;
}

bool servidor_juego_en_curso(ahorcado_t* juego){
	if (ahorcado_chequear_victoria(juego) || ahorcado_chequear_derrota(juego)){
		return false;
	}
	return true;
}

void servidor_actualizar_partida(ahorcado_t* juego, char c){
	ahorcado_actualizar_partida(juego, c);
}


void servidor_set_hints(struct addrinfo* hints){
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = 0;
	hints->ai_flags = AI_PASSIVE;
}

int servidor_set_addrinfo(struct addrinfo* hints, struct addrinfo** result,
	char* port){
	int s = 0;
	s = getaddrinfo(NULL, port, hints, result);
	if (s != 0){
		return 1;
	}
	return 0;
}

void servidor_liberar_addrinfo(struct addrinfo* results){
	freeaddrinfo(results);
}
#endif
