/*************************************************************************
 * 
 * Project          : Practica 1 de Diseño de infraestructuras de Red
 * 
 * Program name     : DataGenerator.c 
 * 
 * Author           : David Carneros Prado
 * 
 * Date created     : 12/03/2019
 * 
 * Purpose          : Programa encargado de generar NUM numeros reales aleatorios
 *                  y guardarlos en el archivo datos.dat para la red hipercubo
 *                  El algoritmo consiste en enviar y recibir el dato de una dimension D
 *                  y quedarnos con el valor maximo. Esto se repite en las D dimensiones
 * 
 *                   Desde 1 hasta D:
 *                     Enviar(vecino[D],Miumero)
 *                     Recibir(vecino[D],SuNumero)
 *                     MiNumero <- max(MiNumero,SuNumero)
 * 
 *                  El algoritmo tendra una complejidad de O(log2(n))
 * 
 * *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define D 3
#define MAX_ITEMS  1024
#define FILENAME   "datos.dat"
#define TRUE   1
#define FALSE  0

/* Declaracion de funciones */
int* vecinosHipercubo(int node);
void obtenerDatos(double* datos,int *continuar,int *cantidadNumeros);
void calcularMaximo(int rank, double numero);
int maximo(int a, int b);


/*******  MAIN *******/
int main(int argc, char* argv[]){
    double *datos;
    double buffNumero;
    int cantidadNumeros = 0, continuar = TRUE;
    MPI_Status status;
    int rank,size,i;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    /* Para el proceso com rank 0 */
    if (rank==0){
        /* inicializamos el array conlos daots y obetenemos los datos del fichero */
        datos = malloc(MAX_ITEMS*sizeof(double));
        obtenerDatos(datos,&continuar,&cantidadNumeros);
        /*Si el numero de datos no es igual a 2 elevado al numero de dimensiones*/
        if(cantidadNumeros!=((int)pow(2,D))){
            fprintf(stderr,"Error con el número de datos\n");
            continuar = FALSE;
        }
        /* Si la cantidad de nodos no es la misma que 2 elevado al numero de dimensiones*/
        if(size!=((int)pow(2,D))){
            fprintf(stderr,"Error con el úumero de nodos\n");
            continuar = FALSE;
        }
        /* Multidifuncimos para saber si continuar con la ejecucion*/
        MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);
    
        /* Si se continua con la ejecucion mandamos el dato a cada proceso*/
        if(continuar){
            for(i=0;i<size;i++){
                buffNumero = datos[i];
                MPI_Bsend(&buffNumero,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
            }
            /* Liberamos el array, puesto que no lo vamos a volver a usar*/
            free(datos);
        }
            
    }

    /* Recibir del nodo 0 si se ha de continuar o no */
    MPI_Bcast(&continuar,1,MPI_INT,0,MPI_COMM_WORLD);

    if(continuar){
        MPI_Recv(&buffNumero,1,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        calcularMaximo(rank,buffNumero);
    }
    
    MPI_Finalize();

    return EXIT_SUCCESS;

}

/**
 * 
 * Funcion para obtener los vecinos de un nodo en las D dimensiones
 * 
*/
int* vecinosHipercubo(int node){
    int i;
    int aux;
    static int vecinos[D];

    for(i=0;i<D;i++){
        aux = 1 << (i);
        vecinos[i] = node ^ aux;
    }

    return vecinos;
}

/*
Funcion para obtener los datos del fichero "datos.dat"
Se le pasara por referencia el array datos, la variable continuar y 
la cantidad de numeros.
*/
void obtenerDatos(double* datos,int *continuar,int *cantidadNumeros){
    char *linea;
    char *token;
    
    FILE *file;
    /* Creamos la linea que vamos a leer del fichero */
    linea = malloc(MAX_ITEMS*sizeof(char));

    if((file = fopen(FILENAME,"r"))==NULL){
        fprintf(stderr,"Error al abrir el archivo %s\n",FILENAME);
        continuar = FALSE;

    } 
    else{
     /*Hacemos uso de la funcion strtok para leer la cadena separada por comas*/
        fgets(linea,MAX_ITEMS*sizeof(char),file);
        token = strtok(linea,",");
        
        while(token!=NULL){
            datos[(*cantidadNumeros)++]=atof(token);
            token = strtok(NULL,",");
        }
        
        
    }
    fclose(file);
    free(linea);

}
/**
 * 
 * Algoritmo para calcular el máximo en una redHiperCubo
 * El algoritmo consiste en enviar y recibir el dato de una dimension D
 * y quedarnos con el valor maximo. Esto se repite en las D dimensiones
 * 
 *      Desde 1 hasta D:
 *          Enviar(vecino[D],Miumero);
 *          Recibir(vecino[D],SuNumero);
 *          MiNumero <- max(MiNumero,SuNumero);
 * 
 * 
*/
void calcularMaximo(int rank, double numero){
    int i;
    int *vecinos;
    MPI_Status status;
    vecinos = vecinosHipercubo(rank);
    double suNumero;

    for(i=0;i<D;i++){
        MPI_BSend(&numero,1,MPI_DOUBLE,vecinos[i],1,MPI_COMM_WORLD);
        MPI_Recv(&suNumero,1,MPI_DOUBLE,vecinos[i],1,MPI_COMM_WORLD,&status);
        numero = maximo(numero,suNumero);
    }

    if(rank==0){
        printf("El numero maximo es: %.2f\n",numero);
    }

}
/**
 * 
 * Devuelve el maximo de 2 valores pasados por argumentos
 * 
*/
int maximo(int a, int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}