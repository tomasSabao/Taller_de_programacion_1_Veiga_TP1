#include "socket.h"
#include "lector.h"
#include "ahorcado.h"
#include "codificador.h"
int main(int argc, char *argv[]){
	if(argc != 4){
    return 1;
  }
	char* port= argv[1];
  int intentos = atoi((const char*)argv[2]);
  if (intentos <= 0){
    return 1;
  }
	socket_t socket;
	codificador_t cod;
	ahorcado_t juego;
	lector_t lector;
	char* linea=NULL;
  int max_conexiones = lector_obtener_num_clientes(&lector, argv[3]);
  ssize_t len=0;
  ssize_t nread;
  bool abrio_file = lector_abrir_archivo_lectura(&lector, argv[3]);
  if (!abrio_file){
        return 1;
    }
	socket_set_hints(&socket, 's');
	socket_set_addrinfo(&socket, NULL, port);
	if(socket_crear_y_bindear(&socket) == 1){
		return 1;
	}
	//socket_crear_y_bindear(&socket);
	socket_escuchar(&socket);
	int conexiones=0;

	char* incognita;
	char buffer;
	void* rta;
	int tamanio_rta;
	int bytes_enviados;
	int bytes_recibidos;
	bool victoria;
	bool derrota;
	bool encontro_flag;
	//int funciono=0;
	bool primer_mensaje;
	//aca depende del numero de conexiones cambio el largo del array
	//hecho por si se llega a necesitar multithreading a futuro
	socket_t aceptado[1];
  //int max_conexiones = 1;
  ahorcado_iniciar_historial(&juego);
	while(conexiones != max_conexiones){
		int funciono=socket_aceptar(&socket, aceptado);
		if(funciono != -1){
			conexiones++;
			nread=lector_leer_linea(&lector, &linea, &len);
			if(nread == -1){
        		lector_cerrar_archivo(&lector);
        		free(linea);
    			return 1;
    		}
    		//reduzco en uno el size del nread porque quiero que me den e
    		//cantidad de bytes igual al numero de caracteres de la palabra
    		//en el texto
    	nread--;
    		//fin de lectura de palabras
    	incognita=malloc(sizeof(char)*nread);
			ahorcado_set_intentos(&juego, intentos);
    	ahorcado_set_palabra(&juego, linea, nread);
    	ahorcado_set_incognita(&juego, incognita);
    	ahorcado_formatear_incognita(&juego);
			rta = malloc(sizeof(char) * 1);
   		tamanio_rta=1;
   		victoria=ahorcado_chequear_victoria(&juego);
   		derrota=ahorcado_chequear_derrota(&juego);
   		primer_mensaje = true;
   		//ciclo send receive
   		while (victoria==false && derrota == false){
   			if(primer_mensaje){
   				bytes_enviados = 0;
   				codificador_generar_mensaje(&cod, &juego, &rta,
   				&tamanio_rta, false);
   				bytes_enviados = socket_enviar(aceptado, (char*)rta , 3);
   				if (bytes_enviados == -1){
   					//printf("Fallo el envio de los mensajes\n");
   					break;
   				}
   				primer_mensaje = false;
   			}
   			bytes_recibidos = 0;
   			bytes_recibidos = socket_recibir(aceptado, &buffer, 1);
   			if (bytes_recibidos == -1){
   				//printf("Fallo la recepcion de los mensajes\n");
   				break;
   			}
   			//proceso ahora
   			//chequea cada letra contra la incognita
        encontro_flag=ahorcado_chequear_letra(&juego, buffer);
        if (encontro_flag == false){
					ahorcado_reducir_intentos(&juego);
        } else {
            ahorcado_reemplazar_letra(&juego, buffer);
            }
        //chequeo de si todas las letras fueron descubiertas
        if (ahorcado_quedan_letras(&juego) == false){
          victoria = true;
          ahorcado_aumentar_victorias(&juego);
        }
        if (ahorcado_chequear_derrota(&juego) == true){
          derrota = true;
          ahorcado_aumentar_derrotas(&juego);
        }
        //envio el estado actual del juego
        codificador_generar_mensaje(&cod, &juego, &rta,
   				&tamanio_rta,false);
        bytes_enviados = socket_enviar(aceptado, (char*)rta, 
            	tamanio_rta);
        if (bytes_enviados == -1){
            //printf("Fallo el envio del mensaje\n");
            break;
        }
   		}
   		codificador_generar_mensaje(&cod, &juego, &rta, &tamanio_rta,true);
   		socket_enviar(aceptado, (char*)rta ,tamanio_rta);
   		free(rta);
   		free(incognita);
		}
		ahorcado_imprimir_resultados(&juego);
		socket_cerrar(aceptado);
	}
	lector_cerrar_archivo(&lector);
	free(linea);
	socket_cerrar(&socket);
	return 0;
}
