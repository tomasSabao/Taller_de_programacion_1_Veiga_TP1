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

int main(int argc, char* argv[]){
	//verificacion de datos
	if (argc !=3){
		//printf("Numero de argumentos invalidos\n");
		return 1;
	}
	char* host= argv[1];
	char* port= argv[2];
	in_manager_t manager;
	socket_t socket;
	decodificador_t deco;
	socket_set_hints(&socket, 'c');
	socket_set_addrinfo(&socket, host, port);
	socket_crear_y_conectar(&socket);

	char mensaje;//este va a ser lo que se va a mandar
	void* msg_ptr=malloc(sizeof(char)*1);//tamaño arbitrario
	int tam_buffer=1;
	int long_mensaje;
	int bytes_enviados, contador_aux;
	bool juego_en_curso = true;
	char msg_recv[60];
	bool no_conectado=true;
	char datos_msj[3];
	int len_recv_msg;
	while (juego_en_curso){
		//espero que el servidor me diga que podemos arrancar
		//necesito primero el byte
		int bytes_recibidos = 0;
		if (no_conectado == true){
			while(bytes_recibidos < 3){
				//recibo 3 bytes fijos como header
				contador_aux = socket_recibir(&socket, datos_msj, 3);
				if (contador_aux == -1){
					//hubo errores en la conexion
					//cierro el socket y termino
					close(socket.skt);
					return 1;
				}
				bytes_recibidos += contador_aux;
			}
			//recibi los 3 bytes del header, decodifico
			no_conectado = false;
			len_recv_msg = decodificador_obtener_longitud(&deco, datos_msj);
			decodificador_imprimir_msj_inicio(&deco, (void*)datos_msj);
		}
		//imprimo como esta todo
		//decodificador_imprimir_mensaje(&deco, (void*)msg_recv);
		//recibo input por teclado
		int caracteres_mandados=0;
		int flag_recibio =in_manager_leer_palabra(&manager, &msg_ptr, &tam_buffer);
		if (flag_recibio == -1){
			//printf("Fallo la lectura de input por teclado\n");
			break;
		}
		//mientras no mande todos los caracteres de lo que entro
		while(caracteres_mandados < tam_buffer){
			bytes_enviados = 0;
			long_mensaje = 1; //tengo que mandar solo 1 byte
			//copio el caracter actual al buffer que va a mandar
			mensaje=*((char*)msg_ptr + caracteres_mandados);
			bytes_enviados=socket_enviar(&socket, &mensaje, long_mensaje);
			//paso algo que hizo que los bytes no mandaran
			if (bytes_enviados == -1){
				//printf("Fallo el envio del mensaje\n");
				free(msg_ptr);
				close(socket.skt);
				return 1;
			}
			caracteres_mandados++;
			bytes_recibidos = 0;
			bytes_recibidos = socket_recibir(&socket, msg_recv, len_recv_msg+3);
			if (bytes_recibidos == -1){
				//printf("Fallo la recepcion del mensaje\n");
				free(msg_ptr);
				close(socket.skt);
				return 1;
			}	
			//decodificador_imprimir_mensaje(&deco, msg_recv, len_recv_msg+3);
			decodificador_imprimir_mensaje(&deco, msg_recv);
			juego_en_curso = decodificador_obtener_juego_en_curso(&deco, 
				msg_recv);
			if (juego_en_curso == false){
				break;
			}
		}
	}
	socket_recibir(&socket, msg_recv, len_recv_msg+3);
	decodificador_imprimir_resultado(&deco, msg_recv);
	free(msg_ptr);
	close(socket.skt);
	return 0;
}
