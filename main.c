//En este archivo.c se inicializarán las cosas
#include "sistemaEntrada.h"
#include "analizadorLexico.h"
#include "tablaSimbolos.h"

int main( int argc, const char* argv[] ){
    inicializarTablasimbolos(); // Primero tabla de simbolos porque abre un archivo y lo cierra al acabar
    inicializarVariablesSistemaEntrada(); // Abre el archivo y no lo cierra hasta que acaba de leerlo
    tipoelem * elemento;
    //do{
    elemento=siguienteLexema();
    printf("%s -- %d\n",elemento->lexema,elemento->compLex);
    free(elemento);
    //}while(lex!=NULL){
    //printf("\nContenido de la Tabla de Símbolos:\n");
    //ImprimirTablaSimbolos();
    //destruirTablasimbolos();
}
