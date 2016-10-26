//En este archivo.c se inicializarán las cosas
#include "sistemaEntrada.h"
#include "analizadorLexico.h"
#include "tablaSimbolos.h"

int main( int argc, const char* argv[] ){
    char * lex=(char*) malloc(sizeof(char));
    int compLex=-1;
    inicializarTablasimbolos(); // Primero tabla de simbolos porque abre un archivo y lo cierra al acabar
    inicializarVariablesSistemaEntrada(); // Abre el archivo y no lo cierra hasta que acaba de leerlo
    
    //while(lex!=NULL){
    compLex=siguienteLexema(&lex); 
    printf("%s -- %d\n",lex,compLex);
    free(lex);
    //}
    
    printf("\nContenido de la Tabla de Símbolos:\n");
    ImprimirTablaSimbolos();
    //destruirTablasimbolos();
}
