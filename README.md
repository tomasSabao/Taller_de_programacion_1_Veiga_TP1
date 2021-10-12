# Taller_de_programacion_1_Veiga_TP1

## Entidades creadas:
- Ahorcado
- Lector
- Codificador
- Decodificador
- Socket
- Input Manager

### Ahorcado
El juego en si. Maneja toda la lógica del estado actual de la partida, numero de victorias y de derrotas, palabra a adivinar.

### Lector
Encargado de leer el archivo, determinar cuantas palabras hay en el mismo, cerrar el archivo.

### Codificador
Encargado de generar los mensajes siguiendo el formato establecido por el enunciado. Es de uso exclusivo del servidor, dado que el cliente solo manda de a un byte.

### Decodificador 
Encargado de decifrar los mensajes generados por el codificado. De uso exclusivo del cliente.

### Socket
Wrapper para los sockets de unix. 

### Input Manager
Encargado de recibir un input de tamaño arbitrario por teclado

![diagrama_clases](/Diagrama_clases.png)  

## Casos de interes:
### Lectura de input de tamaño arbitrariamente grande 
Se usa un puntero a memoria dinamica, en el cual se van introduciendo los caracteres ingresados por teclado. Si la memoria asignada al puntero no alcanza para poner
el caracter actual, se redimensiona la misma y se procede (el proceso se repite hasta poner los n caracteres ingresados). Una vez que todos los caracteres se encuentran asignados, se hace un ultimo redimensionamiento, para que el puntero tenga memoria asignada igual a la ocupada por los caracteres que se ingresaron.
### Manejo de mensajes de tamaño variable
La longitud en bytes de los mensajes que el servidor le manda al cliente no son constantes, dependiendo del largo de la palabra que se tenga que adivinar. Si se sabe que todo mensaje comparte una "cabecera" de 3 bytes en las cuales se especifican el estado de juego, el número de intentos restantes y el largo de la palabra a adivinar. Se decide que en el mensaje inicial que manda el servidor al cliente para decirle que comienza el juego se reciban solo esos 3 bytes, decodificando el largo del mensaje que el servidor le va a mandar en cada iteracion de la comunicacion, y asignando memoria dinamica suficente para almacaenar el mensaje.<br\>
### Lectura del archivo 
Dado que no se puede meter la totalidad del archivo en memoria, se decide abrir el mismo e ir leyendo de a una linea por cada cliente que se conecta, almacenando la linea en un buffer de tamaño variable que luego sera utilizado para la ejecución del juego.
### Manejo de valores de intentos invalidos
Se define como un valor invalido a números menores o iguales a 0 y mayores a 127 (numeros mayores superan el tamanio de bits disponible para almacenar). En el caso que se ingrese dicho valor, se decidió que la ejecución del servidor se detendria.<br\>



## Pasos de comunicación entre cliente- servidor 
1. Servidor bindea
2. Servidor espera para aceptar
3. Cliente conecta
4. Servidor manda mensaje al cliente para indicar comienzo de partida
5. Cliente manda caracter y servidor responde con estado actual de la partida hasta que se adivina la palabra o se terminan los intentos6. Cliente se desconecta
6. Servidor cierra el socket de comunicacion, y revisa si debe aceptar  mas clientes. Si debe aceptar mas, repite el paso 2
7. No teniendo que aceptar mas clientes, el servidor cierra el socket que escucha conexiones y termina su ejecución

## Entregas realizadas en el sercom

![entregas](/TP1_sercom_entregas.png)  
