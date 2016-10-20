#include <stdlib.h>

//Definición de variables
int numLinea=1;

//Inicio de funciones
char * siguienteLexema(){
    char caracter;
    char * lexema;

    caracter=siguienteCaracter();
    while(caracter==' ' || caracter=='\n' || caracter=='\t'){ // Mientras no se encuentre uno de los siguientes delimitadores como inicio de cadena, ya que si son empleados, solo son empleados para contar el número de linea o así...
        if(caracter=='\n'){
            numLinea++;
        }
        nuevoLexema();
        caracter=siguienteCaracter();
    }
    if(caracter=='$'){ // Si se encuentra un $ como inicio de cadena, esto indica que se ha llegado al final del archivo a compilar
        return NULL;
    }
    else{
        // Solicitar primer caracter, entrar en un estado en funcion de si es dígito, letra, / o operador
        // En función del estado del primer caracter realizamos un tipo de comprobación o otro.. ¡¡TODOS LOS LEXEMAS DBEN ESTAR EN LA MISMA LINEA EXCEPTO EL COMENTARIO ANIDADO!!
    }
    /*
            while(comprobarCaracter(centinelaInfo->cadena[centinelaInfo->fin])==0){ // Mientras no se encuentre el caracter delimitador se mueve el puntero final en la cadena de 
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
*/
}


int comprobarCaracter(char caracter){
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