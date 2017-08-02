//Luis Alfredo León Villapún A01322275
//Armando Canto García A01322361
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
/*
  Voy a definir los datos para la matriz m*n
*/
char grid[1002][1002]; //matriz
char grid_new_generation[1002][1002]; //matriz nueva
int m = 1001;
int n = 1001;
int it;
int b = 1;
int generations = 0;

typedef struct cell cell;
struct cell{
  char status;
  int x;
  int y;
};

/*
  Método printUniverse imprime el universo
*/
void printUniverse(){
  int i,j;
  int count = 0;
  for(i = 1; i < m+1; i++){
    for(j = 1; j < n+1; j++){
      if(grid[i][j] == '0') printf("%s%c ", KRED ,grid[i][j]);
      if(grid[i][j] == '1') printf("%s%c ", KGRN ,grid[i][j]);
      if(grid[i][j] == '0'){
        count++;
      }
    }
    printf("%s",KNRM);
    printf("\n");
  }
  if(count == m*n){
    printf("La población se ha extinguido en %d generaciones. ", generations);
  }
}

/*
  Método bigBang crea un universo inicial con casillas aleatorias
*/
void bigBang(){
  int i,j;
  for(i = 0; i < 1001; i++){
    for(j = 0; j < 1001; j++){
      grid[i][j] = '0';
      grid_new_generation[i][j] = '0';
    }
  }
  for(i = 1; i < m+1; i++){
    for(j = 1; j < n+1; j++){
      int r = rand()%2; //Devuelve 1 o 0
      if(r == 1) grid[i][j] = '1';
      if(r == 0) grid[i][j] = '0';
    }
  }
}

/*
  Método pasa_estafeta hace de la nueva generación la generación actual
*/
void pasa_estafeta(){
  int i,j;
  for(i = 1; i < m+1; i++){
    for(j = 1; j < n+1; j++){
      grid[i][j] = grid_new_generation[i][j];
      grid_new_generation[i][j] = '0';
    }
  }
}

/*
  Método bfs hace bfs en profundidad 1, devuelve un arreglo con los valores encontrados(9)
*/
char * bfs(int x, int y){
  char * neighbours = malloc(9*sizeof(char));
  /*
  a|b|c
  -----
  d|e|f
  -----
  g|h|i

  a = x-1, y-1
  b = x-1, y
  c = x-1, y+1
  d = x, y-1
  e = x,y se ignora
  f = x, y+1
  g = x+1, y-1
  h = x+1, y
  i == x+1, y+1

  index   0 1 2 3 4 5 6 7 8
  neigh =[a,b,c,d,e,f,g,h,i]
  */
  neighbours[0] = grid[x-1][y-1];
  neighbours[1] = grid[x-1][y];
  neighbours[2] = grid[x-1][y+1];
  neighbours[3] = grid[x][y-1];
  neighbours[4] = grid[x][y];
  neighbours[5] = grid[x][y+1];
  neighbours[6] = grid[x+1][y-1];
  neighbours[7] = grid[x+1][y];
  neighbours[8] = grid[x+1][y+1];

  return neighbours;
}

/*
  Método new generation hace bfs en profundidad 1 para modificarse.
*/
void * newGeneration(void * args){
      //Promuevo a cell apuntador los argumentos recibidos
      cell * c = (cell*)args;
      it--;
      char * neighbours = malloc(9*sizeof(char));


      neighbours=bfs(c->x, c->y); //Obtengo los vecinos

      /*
      Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
      Any live cell with two or three live neighbours lives on to the next generation.
      Any live cell with more than three live neighbours dies, as if by overpopulation.
      Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      */
      int countLiving = 0;
      int countDead = 0;
      int i;

      //Contamos los vecinos vivos y muertos
      for(i = 0; i < 9; i++){
        if(i == 4) continue; //Porque es la misma célula
        else{
          if(neighbours[i] == '0'){
            countDead++;
          }
          if(neighbours[i] == '1'){
            countLiving++;
          }
        }
      }
      free(neighbours);
      //Si la célula está viva
      if(c->status == '1'){
        //Aplicamos las reglas del juego de la vida
        if(countLiving < 2){
          grid_new_generation[c->x][c->y] = '0'; //Muere
          c->status = '0';
        }
        if(countLiving==2 || countLiving==3){
          grid_new_generation[c->x][c->y] = '1'; //Vive
          c->status = '1';
        }
        if(countLiving>3){
          grid_new_generation[c->x][c->y] = '0'; //Muere
          c->status = '0';
        }
      }
      //Si la célula está muerta
      if(c->status == '0'){
        if(countLiving == 3){
          grid_new_generation[c->x][c->y] = '1'; //Vive
          c->status = '1';
        }
      }
      //Al final el grid nuevo se vuelve el grid inicial y el grid nuevo se resetea.
      int j;
      if(it == 0){ //Si todos los threads ya pasaron
        it = m*n;
      }

      return NULL;
}


/*
  Main method
*/
int main(){
  //Input de datos
  printf("Ja'La: El juego de la vida\n");
  printf("Indique las dimensiones del universo (no mayores a 50 x 50).\n");
  do{
    printf("Filas:\n");
    scanf("%d", &m);
    printf("Columnas:\n");
    scanf("%d", &n);
  }while(m > 50 || n > 50);
  //

  it = m*n; //Total de células

  //Genero mi universo inicial, pero sigue muerto, hay que darle vida a las celulas.
  bigBang();
  printUniverse();
  printf("\n");

  //Le damos vida
  while(1){
    int i,j;
    pthread_t celula;
    for(i = 1; i < m+1; i++){
      for(j = 1; j < n+1; j++){
        //Se genera un thread que representa a cada célula, apoyándonos en una estructura
        cell * nueva_celula = (cell *)malloc(sizeof(cell));
        nueva_celula->status = grid[i][j];
        nueva_celula->x = i;
        nueva_celula->y = j;


        pthread_create(&celula, NULL, newGeneration, nueva_celula);

      }
    }
    pthread_join(celula,NULL);
    generations++;
    pasa_estafeta();
    printUniverse();
    printf("\n");
    sleep(1);
    //pthread_exit(&celula);
  }

  return 0;
}
