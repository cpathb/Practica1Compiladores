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
int siguienteCaracter(char caracter);

void inicializarVariablesSistemaEntrada(){
    struct stat *fileInformation=malloc(sizeof(struct stat));
    stat("regression.d", fileInformation);
    tamBloque=fileInformation->st_blksize;
    source = fopen("regression.d","rb");
    centinelaInfo= malloc(sizeof(struct centinela));
    centinelaInfo->cadena = (char *) malloc(sizeof(char)*((tamBloque*2)+2));
    centinelaInfo->inicio=0;
    centinelaInfo->fin=0;
    // Marcamos mitad y fin de centinela
    centinelaInfo->cadena[tamBloque]='$';
    centinelaInfo->cadena[(2*tamBloque)+1]='$';
}

char * siguienteLexema(){
    char * buffer = (char *) malloc(sizeof(char)*(tamBloque*2));
    
    if(source == NULL){ // Comprobamos si no se pudo abrir el archivo en modo lectura
        ImprimirError(1);
        return NULL;
    }
    else{
        if(!feof(source) || (centinelaInfo->inicio!=(tamBloque*2+1) || centinelaInfo->fin!=(tamBloque*2+1))){ // Comprobamos si llegamos al final del archivo y si el centinela tiene los punteros en el final
            if(!feof(source) && centinelaInfo->inicio==0 && centinelaInfo->fin==0){ // Si no hemos acabado de leer el archivo y nos encontramos en la posicion inicial leemos un bloque y lo cargamos en la izquierda
                buffer=dameBloqueFichero(&source);
                rellenarCentinelaIzquierda(buffer, centinelaInfo);
            }
            // Solicitamos siguiente caracter y comprobamos si es un espacio, un operador, llaves, parentesis, una coma, un punto y coma, barra n, barra t o corchetes. Si es así, hemos encontrado un lexema
            while(centinelaInfo->cadena[centinelaInfo->inicio]==' '||centinelaInfo->cadena[centinelaInfo->inicio]=='\n'||centinelaInfo->cadena[centinelaInfo->inicio]=='\t'){
                centinelaInfo->fin=centinelaInfo->fin+1;
                centinelaInfo->inicio=centinelaInfo->fin;
            }

            while(siguienteCaracter(centinelaInfo->cadena[centinelaInfo->fin])==0){ // Mientras no se encuentre el caracter delimitador se mueve el puntero final en la cadena de 
                centinelaInfo->fin++;
            }
            
            if(centinelaInfo->cadena[centinelaInfo->inicio]=='$'){
                return NULL;
            }

            if(centinelaInfo->inicio!=centinelaInfo->fin){ // Cuando hay mas de un caracter implicado en la comprobación
                char * lex= (char *) malloc(sizeof(char)*(((centinelaInfo->fin)-centinelaInfo->inicio)+1));
                strncpy(lex,&centinelaInfo->cadena[centinelaInfo->inicio],(((centinelaInfo->fin)-centinelaInfo->inicio)));
                centinelaInfo->inicio=centinelaInfo->fin;
                return lex;
            }
            else{ // El elemento es de los anteriores, hay que comprobar su siguiente y ver si es parte del operador o no
                char * lex= (char *) malloc(sizeof(char)*(((centinelaInfo->fin)-centinelaInfo->inicio)+1));
                strncpy(lex,&centinelaInfo->cadena[centinelaInfo->inicio],(((centinelaInfo->fin)-centinelaInfo->inicio))+1);
                centinelaInfo->fin=centinelaInfo->fin+1;
                centinelaInfo->inicio=centinelaInfo->fin;
                return lex;
            }
        }
        else{
            fclose(source); // Cerramos el achivo ya que lo hemos leido completamente y no nos hace falta
            return NULL;
        }
    }
    free(buffer);
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

int siguienteCaracter(char caracter){
    int finLexema=0;
    switch (caracter){ 
        case 9: // Conjunto de caracteres que delimitan la entrada
        case 10:
        case 13:
        case 32:
        case 33:
        case 34:
        case 35:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 47:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 91:
        case 92:
        case 93:
        case 94:
        case 96:
        case 123:
        case 124:
        case 125:
        case 126:
            finLexema=1;
            break;
        case 36: // Simbolo $, dependiendo de donde aparezca sera delimitador o no.
            if(centinelaInfo->fin==tamBloque || centinelaInfo->fin==((tamBloque*2)+1)){
                finLexema=0;
            }
            else{
                finLexema=1;   
            }
        default:
            finLexema=0;
            break;
    }
    return finLexema;
}

void rellenarCentinelaIzquierda(char* buffer, struct centinela *cent){
    //Llenamos el lado izquierdo del buffer centinela con el contenido del buffer original
    strncpy(cent->cadena,buffer,tamBloque);
    if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena 
        cent->cadena[strlen(buffer)]='$';
    }
}


void rellenarCentinelaDerecha(char* buffer, struct centinela *cent){
    //Llenamos el lado derecho del buffer centinela con el contenido del buffer
    int i=tamBloque, enc=0;
    if(feof(source)){ // Si se ha llegado al final del fichero introducimos al final del buffer el simbolo $ como terminal de la cadena 
        strncpy(&cent->cadena[tamBloque+1],buffer,tamBloque);
        cent->cadena[((tamBloque+1)+(strlen(buffer)))]='$';
    }
}