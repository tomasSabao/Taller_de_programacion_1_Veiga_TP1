#include "socket.h"

//funciona
int socket_enviar(socket_t* sock, char* buffer, int tam_buffer){
	int bytes_enviados=0;
	int socket_todavia_valido = true;
	while(bytes_enviados < tam_buffer && socket_todavia_valido){
		int contador_aux=send(sock->skt, &buffer[bytes_enviados],
							 tam_buffer-bytes_enviados, 0);
		if (contador_aux == 0){
			socket_todavia_valido = false;
		}
		if (contador_aux == -1){
			socket_todavia_valido = false;
		}
		bytes_enviados+=contador_aux;
	}
	if (socket_todavia_valido){
		return bytes_enviados;
	} else {
		return -1;
	}
}

//funciona
int socket_escuchar(socket_t* sock){
	int largo_cola=60;
	int escucho=listen(sock->skt, largo_cola);
	return escucho;
}
//funciona
int socket_aceptar(socket_t* bound_sock, socket_t* accept_sock){
	int fd=accept(bound_sock->skt, NULL, NULL);
	if (fd == -1){
		return -1;
	}
	accept_sock->skt = fd;
	return 0;
}

//PRE: sock tiene que estar en estado conectado
int socket_recibir(socket_t* sock, char* buffer, int tamanio_msg){
	int bytes_recibidos = 0;
	bool socket_todavia_valido = true;
	while (bytes_recibidos < tamanio_msg && socket_todavia_valido){
		int s = recv(sock->skt, &buffer[bytes_recibidos],
				 tamanio_msg - bytes_recibidos, 0);
		if (s == 0){
			socket_todavia_valido = false;
		}
		if (s == -1){
			socket_todavia_valido = false;
		}
		bytes_recibidos += s;
	}
	if (socket_todavia_valido){
		return bytes_recibidos;
	} else {
		return -1;
	}
}

void socket_cerrar(socket_t *sock){
	shutdown(sock->skt, SHUT_RDWR);
	close(sock->skt);
}

int socket_enlazar(socket_t* sock, struct addrinfo* result){
	int val = 1;
	int s;
	struct addrinfo* ptr;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next){
		sock->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock->skt == -1){
			continue;
		}
		s = setsockopt(sock->skt, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
		if (s == -1){
			close(sock->skt);
			continue;
		}
		if (bind(sock->skt, ptr->ai_addr, ptr->ai_addrlen) == 0){
			break;
		}
		close(sock->skt);
	}
	if (ptr == NULL){
		return 1;
	}
	return 0;
}


int socket_enlazar_y_escuchar(socket_t* sock, struct addrinfo* result){
	int funciono = socket_enlazar(sock, result);
	if (funciono == 1){
		return 1;
	}
	funciono = socket_escuchar(sock);
	if (funciono == 1){
		return 1;
	}
	return 0;
}

int socket_conectar(socket_t* sock, struct addrinfo* result){
	struct addrinfo* ptr;
	for(ptr = result; ptr != NULL; ptr=ptr->ai_next){
		sock->skt= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(sock->skt == -1){
			continue;
		}
		if (connect(sock->skt, ptr->ai_addr, ptr->ai_addrlen) != -1){
			break; 
		}
		close(sock->skt);
	}
	freeaddrinfo(result);
	if (ptr == NULL){
		return 1;
	}
	return 0;
}

