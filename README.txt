--------------------------------------------------------------------------------
README

JA'LA: El juego de la vida
PROGRAMACIÓN AVANZADA
Proyecto Final

Autores:
  Luis Alfredo León Villapún A01322275
  Armando Canto García A01322361

Profesor: Alberto Oliart Ros

--------------------------------------------------------------------------------

1. Introducción
2. Documentación
3. Cómo correr el programa
4. Conclusión


1. Introducción

Este proyecto es un programa del "juego de la vida" de Conway. El juego de la
vida consiste en una serie de reglas dentro de un "universo", que determinan si
una población vive o muere.

El universo consiste en una serie de "células" ubicadas dentro de un espacio de
m filas y n columnas. Las células vivas se representan como unos y las muertas
como ceros. Cada generación es un estado del universo.

En cada iteración, se genera una nueva generación. Las reglas son las siguientes:
Cualquier célula viva con 2 vecinos vivos muere por falta de población.
Cualquier célula viva con 2 o 3 vecinos vivos vive en la siguiente generación.
Cualquier célula viva con más de 3 vecinos vivos muere por sobrepoblación.
Cuialquier célula muerta con exactamente tres vecinos vivos se convierte en
una célula viva en la siguiente generación.

Así, se ha generado un programa que simula este conjunto de reglas. Para lograrlo,
se usó el lenguaje C con la librería pthread.


2. Documentación

void printUniverse() : imprime el Universo actual

void bigBang() : genera un Universo aleatoriamente con unos y ceros.

void pasa_estafeta() : hace de la generación más reciente la actual.

char * bfs(int x, int y) : devuelve un arreglo de 9 caracteres, recibe como
  entrada las coordenadas de la célula que se va a analizar.

void * newGeneration(void * args) : método que ejecutan todos los hilos (células)
  y modifican la nueva generación del universo, recibe como argumento una estructura
  cell, que consiste en el estatus de la célula y sus coordenadas.

main() : ejecuta el método principal del programa, contiene un loop infinito que
  itera cada generación del universo.


3. Cómo correr el programa

Para correr el programa simplemente diríjase al directorio del Proyecto en una
ventana nueva de la terminal.
Compile el programa con la instrucción "gcc JaLa.c -pthread"
Ejecute el archivo con la instrucción "./a.out"
Indique las filas y columnas del universo.
Para terminar el programa, ejecute el comando CTRL+C.

4. Conclusiones

El nombre del programa "JaLa" se eligió en honor al juego de la saga de libros:
"La Espada de la Verdad", también llamado el juego de la vida, que determina quién
vive y quién muere.

Este proyecto fue un gran final al curso de Programación Avanzada, dado que se
hizo uso de un tema muy importante visto en clase: threads.

Se puede concluir que la programación de este tipo de programas ayuda a comprender
los conceptos a un nivel práctico y además entretenido.
