#include "tablaSimbolos.h" 
#include "gestionErrores.h"
#include <string.h>

//Definicion de funciones
void insertarDefinicionesValor();

// Definición de variables
TablaHash tablaSimbolos;
int valorIdentificador;

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
    FILE * fichero;
    char *loc, *valor;
    fichero = fopen("definiciones.h","r");
    tipoelem elemento, elementoaux;
    if (fichero == NULL){ // Error al abrir el archivo
        ImprimirError(4);
    }
    else{
        char sobras;
        do{
        loc = (char *) malloc(sizeof(char)*(tamLinea));// Mientras no sea final de archivo
            fscanf(fichero,"%[^\n]",loc);
            if(strlen(loc)>0){ // Leemos del archivo la linea hasta el \n
                if(loc[0]=='#'){ // Si es un #define lo tratamos para obtener clave valor
                    valor = (char *) malloc((sizeof(char)*(strlen(loc)-8))); // Reservamos espacio para la cadena sin "#define "
                    strcpy(valor,&loc[7]);
                    sscanf(valor,"%s %d",elemento.lexema,&elemento.compLex); // Buscar patron en valor "%s %d" y asignar a clave valor para insertar en la tabla de simbolos
                    //printf("Insertado: %s -- %d\n",elemento.lexema,elemento.compLex);
                    InsertarHash(&tablaSimbolos,elemento); // Insertamos el elemento en la tabla hash
                    //ImprimirTablaSimbolos();
                    valorIdentificador=elemento.compLex+1;// Modificamos el valor del número para el identificador al del ultimo insertado en la tabla de símbolos desde el fichero
                    free(valor); //Liberamos la variable valor
                }
                free(loc); // Libero la variable linea de codigo
            }
        sobras= fgetc(fichero); // Consumimos el \n
        } while(sobras!=EOF); // Mientras no llegamos al final del archivo
        fclose(fichero); // Cerramos el archivo
    }
}

// Función que comprueba si ya pertenece a la tabla hash y si está devuelve el componente léxico, si no, lo introduce y es identificador
int obtenerCompLex(char * lexema){
    tipoelem elemento;
    if(MiembroHashClave(tablaSimbolos,lexema)==1){ // Comprobamos si ya está en la tabla de símbolos el lexema
        Busqueda(tablaSimbolos,lexema,&elemento); // Obtenemos el valor del lexema en la tabla de símbolos
        return elemento.compLex; // Devolvemos el componente léxico del lexema
    }
    else{ // Si no está, se inserta en la tabla hash como un identificador
        elemento.lexema=(char *) malloc(sizeof(char)* strlen(lexema)); // Reservamos espacio para el lexema
        strcpy(elemento.lexema,lexema); // Copiamos el valor del lexema dentro del elemento
        elemento.compLex=valorIdentificador; // Introducimos el valor del componente léxico
        InsertarHash(&tablaSimbolos,elemento); // Introducimos el elemento en la tabla de símbolos
        return elemento.compLex; // Devolvemos el componente léxico del lexema
    }
}

// Función para imprimir el contenido de la Tabla de Símbolos
void ImprimirTablaSimbolos(){
    int i=0;
    posicion p;
    tipoelem * elemento;
    elemento= malloc(sizeof(struct tipo));
    
    while(i<TamHash-1){ // Recorremos todos los elementos de la tabla has
        if(!esvacia(tablaSimbolos[i])){ // Comprobamos si la posición de la tabla hash esta vacía
            p=primero(tablaSimbolos[i]);
            while(p->sig!=NULL){ // Si estamos en un elemento que 
                recupera(tablaSimbolos[i],p,elemento);
                printf("%s -- %d\n",elemento->lexema,elemento->compLex);
                p=p->sig;
            }
        }
        i++;
    }
}