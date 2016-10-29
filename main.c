//En este archivo.c se inicializarán las cosas
#include "sistemaEntrada.h"
#include "analizadorLexico.h"
#include "tablaSimbolos.h"

int main( int argc, const char* argv[] ){
    inicializarTablasimbolos(); // Primero tabla de simbolos porque abre un archivo y lo cierra al acabar
    inicializarVariablesSistemaEntrada(); // Abre el archivo y no lo cierra hasta que acaba de leerlo
    inicializarAnalizadorLexico();

    tipoelem * elemento;
    elemento = siguienteLexema(); // Solicitamos el primer lexema
    while(elemento->lexema!=NULL && elemento->compLex!=-1){ // Mientras no se haya llegado al final de archivo y se nos devuelva ese lexema y ese complemento léxico
        printf("%s -- %d\n", elemento->lexema, elemento->compLex); // Imprimimos el lexema y el complemento léxico
        free(elemento->lexema); // Liberamos el lexema del elemento
        free(elemento); // Liberamos el elemento
        elemento = siguienteLexema(); // Solicitamos el siguiente lexema
    }
    free(elemento->lexema); // Liberamos el lexema del elemento
    free(elemento); // Liberamos el elemento

    printf("\n\nContenido de la Tabla de Símbolos:\n");
    printf("----------------------------------\n");
    ImprimirTablaSimbolos(); // Imprimimos la tabla de símbolos en el orden creado por la función hash
    destruirSistemaEntrada(); // Destruimos el sistema de entrada
    destruirTablasimbolos(); // Destruimos la tabla de símbolos
}
