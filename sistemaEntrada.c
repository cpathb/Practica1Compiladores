//En este archivo.c se leerá del código fuente bloque a bloque y se introducira en el buffer con doble centinela
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistemaEntrada.h"

// Definición de variables
int tamBloque; // Tamaño del bloque
FILE * source; // Puntero al fichero
struct centinela *centinelaInfo; // Estructura propia para almacenar los datos del doble centinela
int inicio=0;

// Definición de funciones
void rellenarCentinelaIzquierda(char* buffer, struct centinela *cent);
void rellenarCentinelaDerecha(char* buffer, struct centinela *cent);
char * dameBloqueFichero(FILE **fichero);

// Función para inicializar el sistema de entrada
void inicializarVariablesSistemaEntrada(){
    struct stat *fileInformation=malloc(sizeof(struct stat));
    stat("regression.d", fileInformation);
    tamBloque=fileInformation->st_blksize;
    //tamBloque=20; // Cambiar la asignación a esta variable para modificar el tamaño de bloque ¡¡TOCAR BAJO TU RESPONSABILIDAD!!
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
        // rellenamos izquierda de centinela
        char * bloqueFichero =(char *)malloc(sizeof(char)*tamBloque); // Reservamos memoria para el bloque
        strcpy(centinelaInfo->cadena,dameBloqueFichero(&source));
        if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena (EOF)
            centinelaInfo->cadena[strlen(centinelaInfo->cadena)]='$';
        }
        centinelaInfo->cadena[tamBloque]='$';
        centinelaInfo->cadena[(2*tamBloque)+1]='$';
        free(bloqueFichero); // Liberamos la memoria
    }
}

// Función para destruir el sistema de entrada
void destruirSistemaEntrada(){
    fclose(source); // Cerramos el archivo que tenemos abierto
}

// Funcuón para leer desde el fichero un bloque
char * dameBloqueFichero(FILE **fichero){
    char * bloque = (char *) malloc(sizeof(char)*(tamBloque));

    if(!feof(*fichero)){ // Si no se ha llegado a fin de fichero
        // fread(dónde, tamaño de unidad, cuanto tamaño, origen);
        fread(bloque, sizeof(char), (tamBloque), *fichero);
    }
    else{
        bloque=NULL;
    }   
    return bloque;
}

// Función que devuelve el máximo tamaño de lexema que se soportará
int tamMaximoLexema(){
    return tamBloque;
}

// Función que devuelve el caracter a tratar del centinela
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
            caracter=centinelaInfo->cadena[centinelaInfo->fin]; // Solicitamos el siguiente caracter
    }

    if(centinelaInfo->fin==((2*tamBloque)+1)){ // Si antes de incrementar la posición del puntero fin estamos en el final del centinela, volvemos a la posición 0 del centinela
        centinelaInfo->fin=0;
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

// Función para retrasar en uno la posición del puntero que apunta al caracter a tratar en el centinela
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

// Función para que el puntero inicio se mueva hasta la posición del puntero fin en el centinela
void nuevoLexema(){
    centinelaInfo->inicio=centinelaInfo->fin;
    if((centinelaInfo->inicio<tamBloque && centinelaInfo->fin<tamBloque)){ // Si inicio y fin estan en la mitad izquierda
            centinelaInfo->mitad=0;
        }
        else{  // Inicio y fin estan a la derecha
            centinelaInfo->mitad=1;
        }
}

// Función para rellenar el primer bloque del centinela (lado izquierdo)
void rellenarCentinelaIzquierda(char* buffer, struct centinela *cent){
    // Llenamos el lado izquierdo del buffer centinela con el contenido del buffer original
    strncpy(cent->cadena,buffer,tamBloque);
    if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena
        cent->cadena[strlen(buffer)]='$';
    }
    centinelaInfo->cargado=0; // Indicamos donde fué cargado el último bloque cargado en el centinela
    centinelaInfo->cadena[tamBloque]='$';
    centinelaInfo->cadena[(2*tamBloque)+1]='$';

    if(centinelaInfo->mitad==0 && centinelaInfo->cargado!=0){ // Si cargamos en la mitad en la que se encuentra el puntero inicio mostramos el aviso
        ImprimirError(11,0);
    }

}

// Función para rellenar el segundo bloque del centinela (lado derecho)
void rellenarCentinelaDerecha(char* buffer, struct centinela *cent){
    // Llenamos el lado derecho del buffer centinela con el contenido del buffer
        strncpy(&cent->cadena[tamBloque+1],buffer,tamBloque);
        if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena
            cent->cadena[((tamBloque+1)+(strlen(buffer)))]='$';
        }
        centinelaInfo->cargado=1;  // Indicamos donde fué cargado el último bloque cargado en el centinela
        centinelaInfo->cadena[tamBloque]='$';
        centinelaInfo->cadena[(2*tamBloque)+1]='$';

    if(centinelaInfo->mitad==1 && centinelaInfo->cargado!=1){ // Si cargamos en la mitad en la que se encuentra el puntero inicio mostramos el aviso
        ImprimirError(11,0);
    }
}
