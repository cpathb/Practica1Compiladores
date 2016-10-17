//En este archivo.c se inicializarán las cosas

#include "SistemaEntrada.h"

int main( int argc, const char* argv[] ){
    char * lex=NULL;
    inicializarVariablesSistemaEntrada();
    int i=0;
    do{
        lex=siguienteLexema();
        if(lex!=NULL){ //Comprobamos que leemos bien la primera vez
            i++;
            printf("Lexema %d: %s\n", i,lex);
        }
    }
    while(lex!=NULL);
}
