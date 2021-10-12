#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"
#include "input_manager.h"
#include "decodificador.h"
#include "cliente.h"
int main(int argc, char* argv[]){
	if (argc !=3){
		cliente_cerrar_fd();
		return 1;
	}
	char* host= argv[1];
	char* port= argv[2];
	in_manager_t manager;
	socket_t socket;
	decodificador_t deco;
	struct addrinfo hints;
	cliente_set_hints(&hints);
	struct addrinfo* result = NULL;
	if (cliente_set_addrinfo(&hints, &result, host, port) == 1){
    	cliente_cerrar_fd();
    	return 1;
	}
	/*
	socket_set_hints(&socket, 'c');
	socket_set_addrinfo(&socket, host, port);
	if (socket_crear_y_conectar(&socket) == 1){
		cliente_cerrar_fd();
		return 1;
	}*/
	if (socket_conectar(&socket, result) == 1){
		cliente_cerrar_fd();
		//cliente_liberar_addrinfo(result);
		return 1;
	}
	//cliente_liberar_addrinfo(result);
	char mensaje;
	void* msg_ptr=malloc(sizeof(char)*1);
	int tam_buffer=1;
	int long_mensaje;
	int bytes_enviados;
	bool juego_en_curso = true;
	char msg_recv[100];
	bool no_conectado=true;
	char datos_msj[3];
	int len_recv_msg;
	while (juego_en_curso){
		if (no_conectado == true){	
			int recibio_primer = cliente_recibir_primer_msj(&socket,
			datos_msj, &len_recv_msg);
			if (recibio_primer == -1){
				socket_cerrar(&socket);
				free(msg_ptr);
				cliente_cerrar_fd();
				return 1;
			}
			no_conectado = false;
		}
		int caracteres_mandados=0;
		int flag_recibio =in_manager_leer_palabra(&manager, &msg_ptr, &tam_buffer);
		if (flag_recibio == -1){
			break;
		}
		//mientras no mande todos los caracteres de lo que entro
		while(caracteres_mandados < tam_buffer){
			bytes_enviados = 0;
			long_mensaje = 1;
			mensaje=*((char*)msg_ptr + caracteres_mandados);
			bytes_enviados=socket_enviar(&socket, &mensaje, long_mensaje);
			if (bytes_enviados == -1){
				free(msg_ptr);
				socket_cerrar(&socket);
				cliente_cerrar_fd();
				return 1;
			}
			caracteres_mandados++;
			int bytes_recibidos = 0;
			bytes_recibidos = socket_recibir(&socket, msg_recv, len_recv_msg+3);
			if (bytes_recibidos == -1){
				free(msg_ptr);
				socket_cerrar(&socket);
				cliente_cerrar_fd();
				return 1;
			}	
			juego_en_curso = decodificador_obtener_juego_en_curso(&deco, 
				msg_recv);
			if (juego_en_curso == false){
				break;
			}
			decodificador_imprimir_mensaje(&deco, (char*)msg_recv);
		}
	}
	socket_recibir(&socket, (char*) msg_recv, len_recv_msg+3);
	decodificador_imprimir_resultado(&deco, msg_recv);
	free(msg_ptr);
	socket_cerrar(&socket);
	cliente_cerrar_fd();
	return 0;
}
