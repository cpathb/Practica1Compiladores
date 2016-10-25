#include <stdlib.h>
#include <string.h>
#include "tablaSimbolos.h"
#include "sistemaEntrada.h"

// Definición de variables
int numLinea=1;

// Definiciónd de funciones
void tratarPrimerCaracter(char caracter, char ** lexema);
void buscarDelimitadorCadena(char ** lexema);
void buscarDelimitadorNumero(char ** lexema);
int delimitadorCadenaEncontrado(char caracter);
int delimitadorNumeroEncontrado(char caracter, char ** lexema);
int delimitadorNumeroDecimalEncontrado(char caracter, char ** lexema);

// Inicio de funciones

// Función que devuelve el siguiente lexema del sistema de entrada, si se acabaron los lexemas se devuelve NULL
int siguienteLexema(char ** lexema){
    *lexema[0]=siguienteCaracter(); // Solicitamos el primer caracter
    while(*lexema[0]==' ' || *lexema[0]=='\n' || *lexema[0]=='\t'){ // Mientras no se encuentre uno de los siguientes delimitadores como inicio de cadena, ya que si son empleados, solo son empleados para contar el número de linea o así...
        if(*lexema[0]=='\n'){
            numLinea++;
        }
        nuevoLexema();
        *lexema[0]=siguienteCaracter();
    }
    if(*lexema[0]=='$'){ // Si se encuentra un $ como inicio de cadena, esto indica que se ha llegado al final del archivo a compilar
        *lexema=NULL; // Cambiamos el contenido del puntero a NULL para que se paren de pedir lexemas en el analizador sintáctico
        return 0;
    }
    else{ // Si es cualquier otro caracter distinto de $, \n, \t,  
        // Se comprueba el primer caracter y a partir de ahí se busca un delimitador que lo finalice
        tratarPrimerCaracter(*lexema[0],lexema); // Se pasa un puntero a la cadena para realojar memoría cuando sea necesario
        if(*lexema[0]=='$'){ // Si se encuentra un $ como inicio de cadena, esto indica que se ha llegado al final del archivo a compilar
            *lexema=NULL; // Cambiamos el contenido del puntero a NULL para que se paren de pedir lexemas en el analizador sintáctico
            return 0;
        }
    }
    return obtenerCompLex(*lexema);
}

/* Se tarata en función del primer caracter del lexema y en función de lo que sea, se entra en una función de busqueda de delimitador distinto
    Estado 1: Empieza por letra o _
        * Acaba al encontrar un operador o delimitador

    Estado 2: Empieza por un número
        * Acaba al encontrar un operador o delimitador (Excepto punto)
        * Si se encuentra punto se entra en un nuevo estado en el que se continua hasta alcanzar un delimitador (No puede ser punto, si es error)
        * Si es un binario se acaba al llegar a un delimitador o algo no esperado
        * Si es un hexadecimal se acaba al llegar a un delimitador o algo no esperado

    Estado 3: Empieza por comillas dobles "
        * Acaba al encontrar las siguientes comillas dobles ", EXCEPTO SI ANTES HAY UN \
        * Si no acaba en la misma línea es un error

    Estado 4: Empieza por comillas simples '
        * Acaba al encontrar las siguientes comillas simples ', EXCEPTO SI ANTES HAY UN \
        * Si no acaba en la misma lína es un error

    Estado 5: Empieza por /
        * Se acaba si aparece un número
        * Si aparece un = es un operador conjunto al siguiente se comprueba, acaba con 
        * Si se encuentra un +, se entra en busqueda de + / aumentando contadores si aparecen más / +, no se guarda el lexema, se descartan los caracteres
        * Si se encuentra un *, se comprueba si el siguente caracter es un *
            * Si no se encuentra, se entra en búsqueda de * /, no se guarda el lexema, se decartan los caracteres
            * Si se encuentra, se entra en busqueda de un * /, se guarda todo como lexema __COMENTARIO_COMPILACIÓN

    Estado 6: Empieza por un operador, probable que sean separados
*/
void tratarPrimerCaracter(char caracter, char ** lexema){
    switch(caracter){
        // Rango de _ , letras Mayusculas/minusculas, también ñ acentos y cedillas
        case 65 ... 90:
        case 95: // _
        case 97 ... 122: 
        case 128 ... 144:
        case 147 ... 154:
        case 160 ... 165:
        case 181 ... 183:
        case 198 ... 199:
        case 210 ... 212:
        case 214 ... 216:
        case 222:
        case 224:
        case 226 ... 229:
        case 233 ... 237:
            buscarDelimitadorCadena(lexema);
            break;
        
        // Rango Números
        case 48 ... 57:
            buscarDelimitadorNumero(lexema);
            break;
        
        // Comillas dobles
        case 34:
            //buscarDelimitadorComillaSimple
            break;

        // Comillas simples
        case 39:
            //buscarDelimitadorComillaSimple
            break;

        // Dolar
        case 36: // No se trata nada, final de fichero
            break;

        // Punto
        case 46:
            break;

        default:
            ImprimirError(5,numLinea); // Avisamos de ERROR de símbolo inesperado
            *lexema[0]=siguienteCaracter(); // Continuamos analizando el siguiente caracter
            tratarPrimerCaracter(*lexema[0],lexema);
            break;
    }
}

//  Función que busca el delimitador de una cadena
void buscarDelimitadorCadena(char ** lexema){
    char caracter=siguienteCaracter();
    int encontrado=0;
    while(encontrado==0){ // Repetiremos el proceso hasta que se encuentre el delimitador
        encontrado=delimitadorCadenaEncontrado(caracter);
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            *lexema = realloc(*lexema,strlen(*lexema)+1); // Reservamos espacio en la cadena para un caracter más
            strcat(*lexema,&caracter); // Concatenamos el siguiente caracter a la cadena
            caracter=siguienteCaracter();
        }
        else{ // Si se encuentra el delimitador
            nuevoLexema(); // Actualizamos los punteros del centinela
        }
    }
}

void buscarDelimitadorNumero(char ** lexema){
    int encontrado=0;
    char caracter=siguienteCaracter();
    // Se comprueba si el siguiente caracter es otro número, punto o letra b/B (Binario) -> Solo 0's y 1's
    if (*lexema[0]=='0' && (caracter=='b' || caracter=='B')){ // Si empieza por 0 y le sigue una b es binario
        
    }
    else{
        // Se comprueba si el siguiente caracter es otro número, punto o letra x/X (Hexadecimal) -> Solo 0..9a..fA..F
        if (*lexema[0]=='0' && (caracter=='x' || caracter=='X')){ // Si empieza por 0 y le sigue una X es Hexadecimal

        }
        else{
            // Se comprueba si el siguiente caracter es un punto (Decimal)
            if(caracter=='.'){ // Si aparece el punto se buscará delimitador para decimal

            }
        }
    }
    while(encontrado==0){ // Si no hemos entrado en un caso especial de los número buscamos delimitador normal
        encontrado=delimitadorNumeroEncontrado(caracter,lexema);
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            *lexema = realloc(*lexema,strlen(*lexema)+1); // Reservamos espacio en la cadena para un caracter más
            strcat(*lexema,&caracter); // Concatenamos el siguiente caracter a la cadena
            caracter=siguienteCaracter();
        }
        else{
            devolverCaracter();
            nuevoLexema(); // Actualizamos los punteros del centinela
        }
    }
}

// Función que busca un delimitador válido para un identificador
int delimitadorCadenaEncontrado(char caracter){
    switch (caracter){ 
        // Conjunto de caracteres que delimitan la entrada
        case 9 ... 10: // Tabulación, line feed 
        case 13: // Retorno de carro
        case 32 ... 47: // Espacio ! " # $ % & ' ( ) * + , - . / 
        case 58 ... 64: // : ; < = > ? @
        case 91 ... 94: // [ \ ] ^
        case 96: // `
        case 123 ... 126: // { | } ~
            return 1;
            break; 
        default:
            return 0;
            break;
    }
}

// Función que busca un delimitador válido para un entero
int delimitadorNumeroEncontrado(char caracter, char ** lexema){
    int encontrado=0;
    switch (caracter){
         // Conjunto de caracteres que delimitan la entrada 
        case 9 ... 10: // Tabulación, line feed 
        case 13: // Retorno de carro
        case 32 ... 45: // Espacio ! " # $ % & ' ( ) * + , -
        case 47: // /
        case 58 ... 64: // : ; < = > ? @
        case 91 ... 94: // [ \ ] ^
        case 96: // `
        case 123 ... 126: //{ | } ~
            return 1;
            break;
        case 46: // .
            while(encontrado==0){ // Buscamos un delimitador de decimal a partir de ahora el numero será decimal
                encontrado=delimitadorNumeroDecimalEncontrado(caracter, lexema);
                if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
                    *lexema = realloc(*lexema,strlen(*lexema)+1); // Reservamos espacio en la cadena para un caracter más
                    strcat(*lexema,&caracter); // Concatenamos el siguiente caracter a la cadena
                    caracter=siguienteCaracter();
                }
                else{
                    devolverCaracter();
                    nuevoLexema(); // Actualizamos los punteros del centinela
                }
            }
        default:
            return 0;
            break;
    }
}

// Función que busca un delimitador válido para un decimal
int delimitadorNumeroDecimalEncontrado(char caracter, char ** lexema){
    int encontrado=0;
    switch (caracter){
         // Conjunto de caracteres que delimitan la entrada 
        case 9 ... 10: // Tabulación, line feed 
        case 13: // Retorno de carro
        case 32 ... 45: // Espacio ! " # $ % & ' ( ) * + , -
        case 47: // /
        case 58 ... 64: // : ; < = > ? @
        case 91 ... 94: // [ \ ] ^
        case 96: // `
        case 123 ... 126: //{ | } ~
            return 1;
            break;
        case 48 ... 57:
        case 95: // _ 
            return 0;
            break;
        default:
            ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
            break;
    }
}