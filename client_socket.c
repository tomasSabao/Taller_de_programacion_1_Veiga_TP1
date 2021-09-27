#include "socket.h"


int socket_set_hints(socket_t *sock,char modo){
	if (modo == 's'){
		modo='s';
	} else {
		modo='c';
	}
	memset(&(sock->hints), 0, sizeof(struct addrinfo));
	sock->hints.ai_family=AF_INET; //direcciones IPv4
	sock->hints.ai_socktype= SOCK_STREAM;
	sock->hints.ai_protocol=0;
	//aca depende de que tipo de socket quiero que sea
	if (modo == 's'){//modo servidor
		sock->hints.ai_flags= AI_PASSIVE;
	} else {//modo cliente
		sock->hints.ai_flags= 0;
	}
	return 0;
}

//PRE: set_hints ejecutado de antemano
int socket_set_addrinfo(socket_t* sock, char* host, char* port){
	int s=0;
	if (sock->modo == 's'){
		s=getaddrinfo(NULL, port, &(sock->hints), &(sock->result));
	}
	else{
		s=getaddrinfo(host, port, &(sock->hints), &(sock->result));
	}
	if(s!=0){
		printf("Error in getaddrinfo: %s\n", gai_strerror(s));
		return 1;
	}
	return 0;
}

int socket_crear_y_bindear(socket_t* sock){
	struct addrinfo* ptr;
	for(ptr= sock->result; ptr != NULL; ptr = ptr->ai_next){
		sock->skt= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock->skt == -1){
			continue;
		}
		if (bind(sock->skt, ptr->ai_addr, ptr->ai_addrlen) == 0){
			break;//bindeamos
		}
		close(sock->skt);
	}
	freeaddrinfo(sock->result);
	if (ptr == NULL){
		return 1;
		//lo siguiente no se puede usar
		//fprintf(stderr, "No se pudo bindear\n");
		//exit(EXIT_FAILURE);
	}
	return 0;
}

//funciona
int socket_crear_y_conectar(socket_t* sock){
	struct addrinfo* ptr;
	for(ptr = sock->result; ptr != NULL; ptr=ptr->ai_next){
		sock->skt= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(sock->skt == -1){
			continue;
		}
		if (connect(sock->skt, ptr->ai_addr, ptr->ai_addrlen) != -1){
			break; //logramos la conexion
		}
		close(sock->skt);//no se conecto, cerramos socket
	}
	freeaddrinfo(sock->result);
	if (ptr == NULL){
		return 1;
		//esto no se deberia poder hacer
		fprintf(stderr, "No se pudo conectar\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}

//funciona
int socket_enviar(socket_t* sock, char* buffer, int tam_buffer){
	int bytes_enviados=0;
	int contador_aux;
	int socket_todavia_valido = true;
	while(bytes_enviados < tam_buffer && socket_todavia_valido){
		contador_aux=send(sock->skt, &buffer[bytes_enviados],
							 tam_buffer-bytes_enviados, 0);
		if (contador_aux == 0){
			socket_todavia_valido = false;
		}
		if (contador_aux == -1){
			printf("ERROR AL ENVIAR MSG\n");
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
	int largo_cola=6;//TODO: num magico, cambiar despues
	int escucho=listen(sock->skt, largo_cola);
	return escucho;
}
//funciona
int socket_aceptar(socket_t* bound_sock, socket_t* accept_sock){
	int fd=accept(bound_sock->skt, NULL, NULL);
	if (fd == -1){
		return -1;//fallo el accept
	}
	accept_sock->skt = fd;
	return 0;
}

//PRE: sock tiene que estar en estado conectado

int socket_recibir(socket_t* sock, char* buffer, int tamanio_msg){
	int bytes_recibidos = 0;
	int s = 0;
	bool socket_todavia_valido = true;
	while (bytes_recibidos < tamanio_msg && socket_todavia_valido){
		s = recv(sock->skt, &buffer[bytes_recibidos],
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
	close(sock->skt);
}
