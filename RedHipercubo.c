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
 *                    y guardarlos en el archivo datos.dat para la red hipercubo
 * 
 * 
 * ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define D 3

#define MAX_ITEMS  1024
#define FILENAME   "datos.dat"
#define TRUE   1
#define FALSE  0

/* Declaracion de funciones */
int* vecinosHipercubo(int node);

/*MAIN*/
int main(int argc, char* argv){
    double *datos;


}

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
