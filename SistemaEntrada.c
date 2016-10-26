//En este archivo.c se leerá del código fuente bloque a bloque y se introducira en el buffer con doble centinela
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistemaEntrada.h"

// Definición de variables
int tamBloque;
FILE * source;
struct centinela *centinelaInfo;

// Definición de funciones
void rellenarCentinelaIzquierda(char* buffer, struct centinela *cent);
void rellenarCentinelaDerecha(char* buffer, struct centinela *cent);
char * dameBloqueFichero(FILE **fichero);
int comprobarCaracter(char caracter);

void inicializarVariablesSistemaEntrada(){
    struct stat *fileInformation=malloc(sizeof(struct stat));
    stat("regression.d", fileInformation);
    tamBloque=fileInformation->st_blksize;
    //tamBloque=4;
    source = fopen("regression.d","rb");
    if(source == NULL){ // Comprobamos si no se pudo abrir el archivo en modo lectura
        ImprimirError(1,0); // No tiene un número de línea asociado, ya que es de inicialización del compilador
    }
    else{
        centinelaInfo=malloc(sizeof(struct centinela));
        centinelaInfo->cadena = (char *) malloc(sizeof(char)*((tamBloque*2)+2));
        centinelaInfo->inicio=0;
        centinelaInfo->fin=0;
        centinelaInfo->mitad=0; // Estamos en la mitad izquierda
        centinelaInfo->cargado=3; // Ninguna de las opciones para que se pueda cargar la primera vez
        // Marcamos mitad y fin de centinela
        int i=0;
        while(i<((2*tamBloque)+2)){
            centinelaInfo->cadena[i]=0;
            i++;
        }
        centinelaInfo->cadena[tamBloque]='$';
        centinelaInfo->cadena[(2*tamBloque)+1]='$';
        // rellenamos izquierda de centinela
        char * bloqueFichero =(char *)malloc(sizeof(char)*tamBloque); // Reservamos memoria para el bloque
        bloqueFichero=dameBloqueFichero(&source);
        rellenarCentinelaIzquierda(bloqueFichero,centinelaInfo); 
        free(bloqueFichero); // Liberamos la memoria
    }
}

char * dameBloqueFichero(FILE **fichero){
    char * bloque = (char *) malloc(sizeof(char)*(tamBloque));

    if(!feof(*fichero)){
        // fread(dónde, tamaño de unidad, cuanto tamaño, origen);
        fread(bloque, sizeof(char), (tamBloque), *fichero);
    }
    else{
        bloque=NULL;
    }   
    return bloque;
}

char siguienteCaracter(){
    // Comprobación inicial de la posición de final para saber si cargar un bloque en el centinela a izquierda o derecha
    char caracter= centinelaInfo->cadena[centinelaInfo->fin]; //Obtenemos el caracter que enviaremos
    if(centinelaInfo->fin==tamBloque){ // Si el puntero del final de la cadena llega a la mitad del buffer centinela cargamos el siguiente bloque a la derecha
        rellenarCentinelaDerecha(dameBloqueFichero(&source),centinelaInfo);
    }

    if(centinelaInfo->fin==((2*tamBloque)+1)){ // Si el puntero del final llega al final del buffer centinela se carga el siguiente bloque a la izquierda
        rellenarCentinelaIzquierda(dameBloqueFichero(&source),centinelaInfo);
    }

    if(caracter=='$' && (centinelaInfo->fin==tamBloque || centinelaInfo->fin==((2*tamBloque)+1))){ // Si es un $ y fin está en la posición tamBloque o (2*tamBloque) ese caracter no lo devolvemos, saltamos al siguiente
        if(centinelaInfo->fin==(2*tamBloque)+1){ // Si estamos en la ultima posicion del centinela, la nueva posición es 0
            centinelaInfo->fin=0;
        }
        else{
            centinelaInfo->fin++;
        }
            caracter=siguienteCaracter(); // Solicitamos el siguiente caracter
    }

    if(centinelaInfo->fin==((2*tamBloque)+1)){ // Si antes de incrementar la posición del puntero fin estamos en el final del centinela, volvemos a la posición 0 del centinela
        centinelaInfo->fin=0;
        /* // QUIZAS NO ES NECESARIO PARA NADA
        if(centinelaInfo->mitad==2){ // Está en ambas mitades
            // ERROR PORQUE NO SE PUEDE VOLVER A CARGAR, DEVOLVEMOS UN DELIMITADOR PARA QUE ACABE DE BUSCARLO Y PODAMOS CARGAR
            caracter=" ";
            centinelaInfo->mitad=0;
        }
        */
    }
    else{
        centinelaInfo->fin++;
        if((centinelaInfo->inicio<tamBloque && centinelaInfo->fin<tamBloque)){ // Si inicio y fin estan en la mitad izquierda
            centinelaInfo->mitad=0;
        }
        else{
            if((centinelaInfo->inicio>tamBloque && centinelaInfo->fin>tamBloque)){ // Si inicio y fin estan a la derecha
                centinelaInfo->mitad=1;
            }
            else{ // Están en distintas mitades
                centinelaInfo->mitad=2;
            }
        }
    }
    return caracter; // Devolvemos el caracter
}

void devolverCaracter(){
    if(centinelaInfo->fin==tamBloque+1){ // Si está situado en el primer elemento del segundo bloque (antes esta el $ de la mitad centinela retrasamos dos posiciones)
        centinelaInfo->fin=tamBloque-1;
        centinelaInfo->mitad=0;
    }
    if(centinelaInfo->fin==0){ // Si está situado en el primer elemento del primer bloque (antes está el $ del final centinela, por lo que lo retrasamos dos posiciones y vuelve al ultimo caracter dle centinela)
        centinelaInfo->fin=(2*tamBloque);
        centinelaInfo->mitad=1;
    }
    else{
        centinelaInfo->fin--;
    }

}

void nuevoLexema(){
    centinelaInfo->inicio=centinelaInfo->fin;
    if((centinelaInfo->inicio<tamBloque && centinelaInfo->fin<tamBloque)){ // Si inicio y fin estan en la mitad izquierda
            centinelaInfo->mitad=0;
        }
        else{  // Inicio y fin estan a la derecha
                centinelaInfo->mitad=1;  
        }
}

void rellenarCentinelaIzquierda(char* buffer, struct centinela *cent){
    //Llenamos el lado izquierdo del buffer centinela con el contenido del buffer original
    if(centinelaInfo->cargado!=0){ // Si el bloque de la izquierda no fue el último bloque en rellenarse
        //strncpy(cent->cadena,buffer,tamBloque);
        strcpy(cent->cadena,buffer);
        if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena 
            cent->cadena[strlen(buffer)]='$';
        }
        centinelaInfo->cargado=0;
    }
}

void rellenarCentinelaDerecha(char* buffer, struct centinela *cent){
    //Llenamos el lado derecho del buffer centinela con el contenido del buffer
    if(centinelaInfo->cargado!=1){ // Si el bloque de la derecha no fue el último bloque en rellenarse
        int i=tamBloque, enc=0;
        if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena 
            //strncpy(&cent->cadena[tamBloque+1],buffer,tamBloque);
            strcpy(&cent->cadena[tamBloque+1],buffer);
            cent->cadena[((tamBloque+1)+(strlen(buffer)))]='$';
        }
        centinelaInfo->cargado=1;
    }
}