#include <stdbool.h>
#include "socket.h"
#include "decodificador.h"
#ifndef __CLIENTE_H__
#define __CLIENTE_H__

int cliente_recibir_primer_msj(socket_t* sock, char* datos_msj, int* len_pal){
	int bytes_recibidos = 0;
	int contador_aux;
	decodificador_t deco;
	while (bytes_recibidos < 3){
		contador_aux = socket_recibir(sock, datos_msj, 3);
		if (contador_aux == -1){
			return -1;
		}
		 bytes_recibidos +=contador_aux;
	}
	*len_pal = decodificador_obtener_longitud(&deco, datos_msj);
	decodificador_imprimir_msj_inicio(&deco, (void*)datos_msj);
	bytes_recibidos = 0;
	while (bytes_recibidos < *len_pal){
		contador_aux = socket_recibir(sock, datos_msj, *len_pal);
		if (contador_aux == -1){
			return -1;
		}
		bytes_recibidos += contador_aux;
	}
	return 0;
}

void cliente_cerrar_fd(){
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}

void cliente_set_hints(struct addrinfo* hints){
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = 0;
	hints->ai_flags = 0;
}

int cliente_set_addrinfo(struct addrinfo* hints, struct addrinfo** result,
	char* host, char* port){
	int s = 0;
	s = getaddrinfo(host, port, hints, result);
	if (s != 0){
		return 1;
	}
	return 0;
}


/*
void cliente_liberar_addrinfo(struct addrinfo* result){
	freeaddrinfo(result);
}*/

//bool servidor_obtener_juego_en_curso()



#endif
