//En este archivo.c se inicializarán las cosas
#include "sistemaEntrada.h"
#include "analizadorLexico.h"
#include "tablaSimbolos.h"


int main( int argc, const char* argv[] ){
    char * lex=(char*) malloc(sizeof(char));
    int compLex=-1;
    inicializarTablasimbolos(); // Primero tabla de simbolos porque abre un archivo y lo cierra al acabar
    printf("\nContenido de la Tabla de Símbolos Inicio:\n");
    ImprimirTablaSimbolos();
    inicializarVariablesSistemaEntrada(); // Abre el archivo y no lo cierra hasta que acaba de leerlo
    printf("\n");
    printf("\n");
    printf("\n");
    //while(lex!=NULL){
    compLex=siguienteLexema(&lex); 
    printf("%s -- %d\n",lex,compLex);
    free(lex);
    //}
    printf("\nContenido de la Tabla de Símbolos Fin:\n");
    ImprimirTablaSimbolos();
    printf("Comenzamos destruccion\n");
    //destruirTablasimbolos();
    printf("Terminamos destruccion\n");
}
