#include "tablaSimbolos.h"
#include <string.h>

//Definicion de funciones
void insertarDefinicionesValor();

// Definición de variables
TablaHash tablaSimbolos;

// Función para inicializar la tabla de simbolos
void inicializarTablasimbolos(){ 
    InicializarTablaHash(tablaSimbolos);
    insertarDefinicionesValor();

}
// Función para destruir la tabla de símbolos
void destruirTablasimbolos(){ 
    DestruirTablaHash(tablaSimbolos);
}

// Función para insertar los lexemas del archivo de definiciones en la tabla de símbolos
void insertarDefinicionesValor(){
    FILE * source;
    char *loc, *valor;
    source = fopen("definiciones.h","r");
    tipoelem elemento, elementoaux;
    if (source == NULL){ // Error al abrir el archivo
        ImprimirError(4);
    }
    else{
        char sobras;
        do{
        loc = (char *) malloc(sizeof(char)*(tamLinea));// Mientras no sea final de archivo
        
        if(fscanf(source,"%[^\n]",loc)!=0){ // Leemos del archivo la linea hasta el \n
            if(loc[0]=='#'){ // Si es un #define lo tratamos para obtener clave valor
                valor = (char *) malloc((sizeof(char)*(strlen(loc)-8))); // Reservamos espacio para la cadena sin "#define "
                strncpy(valor,&loc[7],(strlen(loc)-7));
                sscanf(valor,"%s %d",elemento.lexema,&elemento.compLex);// Buscar patron en valor "%s %d" y asignar a clave valor para insertar en la tabla de simbolos
                InsertarHash(&tablaSimbolos,elemento); // Insertamos el elemento en la tabla hash
                printf("\n");
                free(valor); //Liberamos la variable valor
            }
            free(loc); // Libero la variable linea de codigo
        }
        sobras= fgetc(source); // Consumimos el \n
        }while (sobras!=EOF);
        fclose(source);
    }
}

// OBTENER VALOR -> Comprueba y si esta devuelve el compLex y si no lo introduce y es identificador