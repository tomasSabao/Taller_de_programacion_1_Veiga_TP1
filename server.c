#include "socket.h"
#include "lector.h"
#include "ahorcado.h"
#include "codificador.h"
#include "servidor.h"
int main(int argc, char *argv[]){
  int parametros_validos = servidor_verificar_parametros(argc, argv);
  if (parametros_validos != 0){
    servidor_cerrar_fd();
    return 1;
  }
	char* port= argv[1];
  int intentos = atoi((const char*)argv[2]);
	socket_t socket;
	ahorcado_t juego;
	lector_t lector;
	char* linea=NULL;
  int max_conexiones = lector_obtener_num_clientes(&lector, argv[3]);
  ssize_t len=0;
  ssize_t nread;
  if (!lector_abrir_archivo_lectura(&lector, argv[3])){
    servidor_cerrar_fd();
    return 1;
  }
  struct addrinfo hints;
  servidor_set_hints(&hints);
  struct addrinfo* result;
  if (servidor_set_addrinfo(&hints, &result, port) == 1){
    servidor_cerrar_fd();
    lector_cerrar_archivo(&lector);
    return 1;
  }

  if (socket_enlazar_y_escuchar(&socket, result) == 1){
    servidor_liberar_addrinfo(result);
    lector_cerrar_archivo(&lector);
    servidor_cerrar_fd();
    return 1;
  }
  servidor_liberar_addrinfo(result);
	int conexiones=0;
	char* incognita;
	char buffer;
	void* rta;
	int tamanio_rta;
	bool primer_mensaje;
	socket_t aceptado[1];
  ahorcado_iniciar_historial(&juego);
	while(conexiones != max_conexiones){
    int acepto_y_leyo = servidor_leer_linea_aceptar_cliente(&lector, &linea,
      len, &nread, &socket, aceptado);
    if (acepto_y_leyo == 1){
      servidor_cerrar_fd();
      return 1;
    }
    if (acepto_y_leyo == 0){
      conexiones ++;
      nread--;
      incognita=malloc(sizeof(char)*nread);
      ahorcado_set_partida(&juego, intentos, linea, nread, incognita);
      rta = malloc(sizeof(char) * 1);
      tamanio_rta=1;
      primer_mensaje = true;
      while (servidor_juego_en_curso(&juego)){
        if (primer_mensaje){
          servidor_generar_mensaje(&juego, &rta, &tamanio_rta);
          int envio_mensaje = servidor_enviar_mensaje(aceptado, rta, 
            tamanio_rta, true);
          if (envio_mensaje == 1){
            break;
          }
          primer_mensaje = false;   
        }
        if (servidor_recibir_mensaje(aceptado, &buffer) == 1){
          break;
        }
        servidor_actualizar_partida(&juego, buffer);
        servidor_generar_mensaje(&juego, &rta, &tamanio_rta);
        int envio_mensaje = servidor_enviar_mensaje(aceptado, rta, 
          tamanio_rta, false);
        if (envio_mensaje == 1){
          break;
        }
      }
      free(rta);
      free(incognita);
      free(linea);
    }
    socket_cerrar(aceptado);
  }
  ahorcado_imprimir_resultados(&juego);
	lector_cerrar_archivo(&lector);
	socket_cerrar(&socket);
  servidor_cerrar_fd();
	return 0;
}
