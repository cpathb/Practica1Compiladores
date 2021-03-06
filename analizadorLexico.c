#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tablaSimbolos.h"
#include "sistemaEntrada.h"
#include "definiciones.h"

// Definición de variables
int numLinea=1;
int tamMaxLexema=0; // Variable que se inicializa con la función inicializarAnalizadorLexico y se le asigna el valor del tamaño de bloque para comprobaciones de exceso del tamaño de bloque en los lexemas encontrados

// Definiciónd de funciones
void tratarPrimerCaracter(char caracter, tipoelem * elemento);
void buscarDelimitadorCadena(tipoelem * elemento);
void buscarDelimitadorNumero(tipoelem * elemento);
void buscarDelimitadorComillaSimple(tipoelem * elemento);
void buscarDelimitadorComillaDoble(tipoelem * elemento);
int delimitadorCadenaEncontrado(char caracter);
int delimitadorNumeroEncontrado(char caracter, tipoelem * elemento);
int delimitadorNumeroDecimalEncontrado(char caracter, tipoelem * elemento);

// Inicio de funciones

// Función para inicializar el Analizador Léxico
void inicializarAnalizadorLexico(){
    tamMaxLexema=tamMaximoLexema();
}

// Función que devuelve el siguiente lexema del sistema de entrada, si se acabaron los lexemas se devuelve NULL
tipoelem * siguienteLexema(){
    tipoelem *elemento;
    elemento=malloc(sizeof(tipoelem)); // Reservamos memoria para la estructura del elemento
    elemento->lexema=calloc(1,sizeof(char)); // Reservamos memoria para un caracter y lo rellenamos con '\0'
    elemento->compLex=-1; // Inicializamos a un valor que nunca podrá tomar

    char caracter=siguienteCaracter(); // Solicitamos el primer caracter
    elemento->lexema[0]=caracter;
    while(elemento->lexema[0]==' ' || elemento->lexema[0]=='\n' || elemento->lexema[0]=='\t'){ // Mientras no se encuentre uno de los siguientes delimitadores como inicio de cadena, ya que si son empleados, solo son empleados para contar el número de linea o así...
        if(elemento->lexema[0]=='\n'){
            numLinea++;
        }
        nuevoLexema();
        elemento->lexema[0]=siguienteCaracter();
    }
    if(elemento->lexema[0]=='$'){ // Si se encuentra un $ como inicio de cadena, esto indica que se ha llegado al final del archivo a compilar
        elemento->lexema=NULL; // Cambiamos el contenido del puntero a NULL para que se paren de pedir lexemas en el analizador sintáctico
        elemento->compLex= -1;
        return elemento;
    }
    else{ // Si es cualquier otro caracter distinto de $, \n, \t,  
        // Se comprueba el primer caracter y a partir de ahí se busca un delimitador que lo finalice, mientras el primer caracter no se \n, que es lo que emplearemos para recuperarnos de errores
        int problema=1; // Variable que valdrá uno cuando
        while(problema==1){        
            tratarPrimerCaracter(elemento->lexema[0],elemento); // Se pasa un puntero a la cadena para realojar memoría cuando sea necesario
            if(elemento->lexema[0]=='\n'){ // Si hay un error en la funcion previamente invocada, se pondra el valor del caracter inicial a \n, por lo que podrémos repetir el proceso a partir de donde sucedió el error
                problema=1;
                elemento->lexema[0]=siguienteCaracter();
                elemento->compLex=-1;
            }
            else{
                problema=0;
            }
        }
        if(elemento->lexema[0]=='$'){ // Si se encuentra un $ como inicio de cadena, esto indica que se ha llegado al final del archivo a compilar
            elemento->lexema=NULL; // Cambiamos el contenido del puntero a NULL para que se paren de pedir lexemas en el analizador sintáctico
            elemento->compLex= -1;
            return elemento;
        }
        
    }
    if(elemento->compLex==-1){ // Si no se a encontrado algún caso definido en definiciones.h
        elemento->compLex=obtenerCompLex(elemento->lexema); // Se solicita el valor del lexema a la tabla de símbolos
    }

    return elemento;
}

// Función que se emplea para analizar el primer caracter de
void tratarPrimerCaracter(char caracter, tipoelem * elemento){
    if(isalpha(caracter) || caracter=='_'){ // Es una cadena
        buscarDelimitadorCadena(elemento);
    }
    else{
        if(isdigit(caracter)){ // Es un número
            buscarDelimitadorNumero(elemento);
        }
        else{
            if(caracter=='\"'){ // Es una cadena literal de comillas dobles
                buscarDelimitadorComillaDoble(elemento);
            }
            else{
                if(caracter=='\''){ // Es una cadena literal de comillas simples
                    buscarDelimitadorComillaSimple(elemento);
                }
                else{
                    if((caracter>32 && caracter<48) || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){ // Se ha encontrado un token o el comienzo de un operador doble
                        if(caracter==33 || caracter==42 || caracter==43 || caracter==45 || caracter==47 || caracter==60 || caracter==61 || caracter==62){ // Si es alguno de los operadores que pueden tener otro operador a continuación: ! * + - / < = >
                            int encontrado=0,anidado=0, tam=strlen(elemento->lexema);
                            char caract=siguienteCaracter(); // Almacenamos el siguiente siguiente caracter en el
                            switch(caracter){
                                case 33: // ! --> Comprobamos si el siguiente caracter es =, si es así lo insertamos en la cadena (!=)
                                    if(caract==61){
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=DISTINTO;
                                        nuevoLexema();
                                    }
                                    else{
                                        elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                        devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                        nuevoLexema();
                                    }
                                    break;

                                case 42: // * --> Comprobamos si el siguiente caracter es =, si es así lo insertamos en la cadena (*=)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=PORIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                        devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                        nuevoLexema();
                                    }
                                    break;

                                case 43: // + --> Comprobamos si el siguiente caracter es = o +, si es así lo insertamos en la cadena (+=) (++)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=MASIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        if(caract==43){ // +
                                            tam++;
                                            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                            elemento->lexema[tam-1]=caract; // Insertamos el número
                                            elemento->compLex=MASMAS;
                                            nuevoLexema();
                                        }
                                        else{
                                            elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                            devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                            nuevoLexema();
                                        }
                                    }
                                    break;

                                case 45: // - --> Comprobamos si el siguiente caracter es = o -, si es así lo insertamos en la cadena (-=) (--)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=MENOSIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        if(caract==45){ // -
                                            tam++;
                                            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                            elemento->lexema[tam-1]=caract; // Insertamos el número
                                            elemento->compLex=MENOSMENOS;
                                            nuevoLexema();
                                        }
                                        else{
                                            elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                            devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                            nuevoLexema();
                                        }
                                    }
                                    break;

                                case 47: // / --> Comprobamos si el siguiente caracter es =, si es así lo insertamos en la cadena (/=), tambien comprobamos por comentarios
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=BARRAIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        if(caract==47){ // Es un comentario de línea, por lo que descartamos toda la entrada hasta final de línea o final de fichero
                                            while(caract!='\n' && caract!='$'){
                                                caract=siguienteCaracter();
                                            }
                                            devolverCaracter(); // Devolvemos el /n (salto de línea) para que sean tratado el caracter
                                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                            elemento->lexema=calloc(1, sizeof(char));
                                            elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                            nuevoLexema();
                                        }
                                        else{
                                            if(caract==42) { // Es un comentario de bloque, por lo que descartamos toda la entrada hasta encontrar */ o final de fichero (si se encuentra final de fichero se muestra error de cierre de comentario
                                                caract=siguienteCaracter();
                                                tam++;
                                                if(caract==42){ // Es un comentario de documentación, descartamos toda la entrada hasta encontrar */ o final de fichero (si se encuentra final de fichero se muestra error de cierre de comentario
                                                    caract=siguienteCaracter();
                                                    tam++;
                                                    if(caract==47){ // Era un comentario de bloque que se cerro de golpe
                                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                        elemento->lexema=calloc(1, sizeof(char));
                                                        elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                                        nuevoLexema();
                                                    }
                                                    else{
                                                        if(caract=='\n'){
                                                            numLinea++;
                                                        }
                                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter
                                                        elemento->lexema[1]='*';
                                                        elemento->lexema[2]='*';
                                                        while(encontrado==0 && caract!='$'){ // Mientras no encontremos el final de fichero o */
                                                            caract=siguienteCaracter();
                                                            if(caract==42){ // Aparece un *
                                                                tam++;
                                                                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                                                elemento->lexema[tam-1]=caract;
                                                                caract=siguienteCaracter();
                                                                if(caract==47){ // Aparece un /
                                                                    tam++;
                                                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                                                    elemento->lexema[tam-1]=caract;
                                                                    encontrado=1;
                                                                    if(strlen(elemento->lexema)<=tamMaxLexema){ // Si no excede el tamaño máximo de lexema
                                                                        elemento->compLex=COMENTARIODOCUMENTACION; // asignamos el componente léxico
                                                                        nuevoLexema();
                                                                    }
                                                                    else{
                                                                        ImprimirError(10,numLinea);
                                                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                                        elemento->lexema = calloc(1, sizeof(char));
                                                                        elemento->lexema[0] = '\n';
                                                                    }
                                                                }
                                                                else{
                                                                    if(caract=='\n'){ // Si aparece un salto de línea incrementamos el contador del programa
                                                                        numLinea++;
                                                                    }
                                                                    tam++;
                                                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                                                    elemento->lexema[tam-1]=caract;
                                                                }
                                                            }
                                                            else{
                                                                if(caract=='\n'){ // Si aparece un salto de línea incrementamos el contador del programa
                                                                    numLinea++;
                                                                }
                                                                else{
                                                                    if(caract=='$'){ //final de fichero
                                                                        ImprimirError(9,numLinea);
                                                                        encontrado=1;
                                                                        devolverCaracter(); // Devolvemos el /n (salto de línea) para que sea tratado el caracter
                                                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                                        elemento->lexema=calloc(1, sizeof(char));
                                                                        elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                                                        nuevoLexema();
                                                                    }
                                                                    else{
                                                                        tam++;
                                                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                                                        elemento->lexema[tam-1]=caract;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                else{
                                                    while(encontrado==0 && caract!='$'){ // Mientras no encontremos el final de fichero o */
                                                        caract=siguienteCaracter();
                                                        if(caract==42){ // Aparece un *
                                                            caract=siguienteCaracter();
                                                            if(caract==47){ // Aparece una /
                                                                encontrado=1;
                                                                devolverCaracter();
                                                                elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                                elemento->lexema=calloc(1, sizeof(char));
                                                                elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                                                nuevoLexema();
                                                            }
                                                            else{
                                                                if(caract=='\n'){ // Si aparece un salto de línea incrementamos el contador del programa
                                                                    numLinea++;
                                                                }
                                                            }
                                                        }
                                                        if(caract=='\n'){ // Si aparece un salto de línea incrementamos el contador del programa
                                                            numLinea++;
                                                        }

                                                        if(caract=='$'){ //final de fichero
                                                            ImprimirError(9,numLinea);
                                                            encontrado=1;
                                                            devolverCaracter(); // Devolvemos el /n (salto de línea) para que sea tratado el caracter
                                                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                            elemento->lexema=calloc(1, sizeof(char));
                                                            elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                                            nuevoLexema();
                                                        }
                                                    }
                                                }
                                            }
                                            else{
                                                if(caract==43){ // Es un comentario de anidado, por lo que descartamos toda la entrada hasta encontrar tantos +/ como final de fichero (si se encuentra final de fichero se muestra error de cierre de comentario
                                                    anidado++;
                                                    while(anidado!=0 && caract!='$'){
                                                        caract=siguienteCaracter();
                                                        if(caract==43){ // Aparece un +
                                                            caract=siguienteCaracter();
                                                            if(caract==47){ // Aparece un /
                                                                anidado--;
                                                            }
                                                        }
                                                        if(caract==47){ // Aparece un /
                                                            caract=siguienteCaracter();
                                                            if(caract==43){ // Aparece un +
                                                                anidado++;
                                                            }
                                                        }
                                                        if(caract=='\n'){ // Si aparece un salto de línea incrementamos el contador del programa
                                                            numLinea++;
                                                        }
                                                    }

                                                    if(caract=='$'){ //final de fichero
                                                        ImprimirError(9,numLinea);
                                                        devolverCaracter(); // Devolvemos el /n (salto de línea) para que sea tratado el caracter
                                                    }
                                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                                    elemento->lexema=calloc(1, sizeof(char));
                                                    elemento->lexema[0]='\n'; // Insertamos el \n para que no se trate como lexema
                                                    nuevoLexema();
                                                }
                                                else{
                                                    elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                                    devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                                    nuevoLexema();
                                                }
                                            }
                                        }
                                    }
                                    break;

                                case 60: // < --> Comprobamos si el siguiente caracter es =, si es así lo insertamos en la cadena (<=)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=MENORIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                        devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                    }
                                    break;

                                case 61: // = --> Comprobamos si el siguiente caracter es =, > o -, si es así lo insertamos en la cadena (==) (=>) (=<)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=IGUALIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        if(caract==62){ // >
                                            tam++;
                                            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                            elemento->lexema[tam-1]=caract; // Insertamos el número
                                            elemento->compLex=IGUALMAYOR;
                                            nuevoLexema();
                                        }
                                        else{
                                            if(caract==60){ // <
                                                tam++;
                                                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                                elemento->lexema[tam-1]=caract; // Insertamos el número
                                                elemento->compLex=IGUALMENOR;
                                                nuevoLexema();
                                            }
                                            else {
                                                elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                                devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                                nuevoLexema();
                                            }
                                        }
                                    }
                                    break;

                                case 62: // > --> Comprobamos si el siguiente caracter es =, si es así lo insertamos en la cadena (>=)
                                    if(caract==61){ // =
                                        tam++;
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                        elemento->lexema[tam-1]=caract; // Insertamos el número
                                        elemento->compLex=MAYORIGUAL;
                                        nuevoLexema();
                                    }
                                    else{
                                        elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                                        devolverCaracter(); // Volvemos a atras el puntero de fin para que vuelva a tratar el caracter que debemos tratar
                                        nuevoLexema();
                                    }
                                    break;
                            }
                        }
                        else{ // Es un operador normal, insertamos su valor en la tabla asci como componente léxico
                            elemento->compLex=(int)elemento->lexema[0]; // Como es un token, introducimos su valor ascii como componente léxico
                            nuevoLexema();
                        }

                    }
                    else{
                        if(caracter=='\n'){ // Si es una nueva línea aumentamos el contador de línea
                            numLinea++;
                        }
                        else{
                            if(caracter==9 || caracter == 10 || caracter==32){ // Si es un tab, line feed o un espacio no se tratan
                                elemento->lexema[0]='\n';
                            }
                        }

                    }
                }
            }
        }

    }
}

// Funciones de busqueda de delimitadores

//  Función que busca el delimitador de un lexema que empieza por una letra o _
void buscarDelimitadorCadena(tipoelem * elemento){
    char caracter=siguienteCaracter();
    int encontrado=0, tam=strlen(elemento->lexema); // Variable para saber si se encontró el delimitador y variable para reservar el tamaño del lexema

    while(encontrado==0){ // Repetiremos el proceso hasta que se encuentre el delimitador
        encontrado=delimitadorCadenaEncontrado(caracter);
        tam++;
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter;
            caracter=siguienteCaracter();
        }
        else{ // Si se encuentra el delimitador
            if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                ImprimirError(10,numLinea);
                elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                elemento->lexema = calloc(1, sizeof(char));
                elemento->lexema[0] = '\n';
            }
            devolverCaracter();
            nuevoLexema(); // Actualizamos los punteros del centinela
        }
    }
}

//  Función que busca el delimitador de un lexema que empieza por un número
void buscarDelimitadorNumero(tipoelem * elemento){
    char caracter=siguienteCaracter();
    int encontrado=0, tam=strlen(elemento->lexema);
    // Se comprueba si el siguiente caracter es otro número, punto o letra b/B (Binario) -> Solo 0's y 1's
    if (elemento->lexema[0]=='0' && (caracter=='b' || caracter=='B')){ // Si empieza por 0 y le sigue una b es binario
        tam++;
        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
        elemento->lexema[tam-1]=caracter;
        while(encontrado==0){
            tam++;
            caracter=siguienteCaracter();
            if(caracter=='0' || caracter=='1'){ // Mientras sea 0 o 1 se inserta en la cadena de binario
                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                elemento->lexema[tam-1]=caracter;
            }
            else{
                if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<48) || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127) ){ // Si es delimitador de número se ha encontrado delimitador
                    encontrado=1;
                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                        ImprimirError(10,numLinea);
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                        elemento->lexema = calloc(1, sizeof(char));
                        elemento->lexema[0] = '\n';
                    }
                    else{
                        elemento->compLex = NUMEROBINARIO;
                    }
                    devolverCaracter();
                    nuevoLexema();
                }
                else{ // Entrada inesperada, avisamos y ya analizamos posteriormente el símbolo como inicio de otro lexema
                    encontrado=1;
                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                        ImprimirError(10,numLinea);
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                        elemento->lexema = calloc(1, sizeof(char));
                        elemento->lexema[0] = '\n';
                    }
                    else {
                        elemento->compLex = NUMEROBINARIO;
                        ImprimirError(6, numLinea);
                    }
                    devolverCaracter();
                    nuevoLexema();
                }
            }
        }
    }
    else{
        // Se comprueba si el siguiente caracter es otro número, punto o letra x/X (Hexadecimal) -> Solo 0..9a..fA..F
        if (elemento->lexema[0]=='0' && (caracter=='x' || caracter=='X')){ // Si empieza por 0 y le sigue una X es Hexadecimal
            tam++;
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter;
            while(encontrado==0){
                tam++;
                caracter=siguienteCaracter();
                if((caracter>47 && caracter<58) || (caracter>64 && caracter<71) || (caracter>96 && caracter<103)){ // Mientras sea 0..9, A..F o a..f se inserta en el hexadecimal
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=caracter;
                }
                else{
                    if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<48) || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127) ){ // Si es delimitador de número se ha encontrado delimitador
                        encontrado=1;
                        if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                            ImprimirError(10,numLinea);
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                            elemento->lexema = calloc(1, sizeof(char));
                            elemento->lexema[0] = '\n';
                        }
                        else{
                            elemento->compLex=NUMEROHEXADECIMAL;
                            devolverCaracter();
                        }
                        nuevoLexema();
                    }
                    else{ // Entrada inesperada, avisamos y ya analizamos posteriormente el símbolo como inicio de otro lexema
                        encontrado=1;
                        if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                            ImprimirError(10,numLinea);
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                            elemento->lexema = calloc(1, sizeof(char));
                            elemento->lexema[0] = '\n';
                        }
                        else{
                            elemento->compLex=NUMEROHEXADECIMAL;
                            ImprimirError(6,numLinea);
                        }
                        devolverCaracter();
                        nuevoLexema();
                    }
                }
            }
        }
        else{
            if(caracter=='.'){ // Se comprueba si el siguiente caracter es un punto (Decimal), si aparece el punto se buscará delimitador para decimal
                tam++;
                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                elemento->lexema[tam-1]=caracter;
                while(encontrado==0){
                    tam++;
                    caracter=siguienteCaracter();
                    encontrado=delimitadorNumeroDecimalEncontrado(caracter,elemento);
                    if(encontrado==0) {
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=caracter; // Insertamos el número
                    }
                    else{
                        if(elemento->compLex!=NUMERODECIMAL && elemento->compLex!=NUMEROCIENTIFICO && elemento->compLex!=NUMEROENTERO){ // Si no se transformo en la comprobación en decimal ni científico ni tiene ya valor de entero
                            if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                ImprimirError(10,numLinea);
                                elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                elemento->lexema = calloc(1, sizeof(char));
                                elemento->lexema[0] = '\n';
                            }
                            else {
                                elemento->compLex = NUMEROENTERO;
                            }
                        }
                        devolverCaracter();
                        nuevoLexema();
                    }
                }
            }
        }
    }
    while(encontrado==0){ // Si no hemos entrado en un caso especial de los número buscamos delimitador normal
        tam++;
        encontrado=delimitadorNumeroEncontrado(caracter,elemento);
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter;
            caracter=siguienteCaracter();
        }
        else{
            devolverCaracter();
            nuevoLexema(); // Actualizamos los punteros del centinela
            if(elemento->compLex!=NUMERODECIMAL && elemento->compLex!=NUMEROCIENTIFICO){ // Si no se transformo en la comprobacion en decimal ni científico es un decimal
                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                    ImprimirError(10,numLinea);
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                    elemento->lexema = calloc(1, sizeof(char));
                    elemento->lexema[0] = '\n';
                }
                else{
                    elemento->compLex = NUMEROENTERO;
                }
            }
        }
    }
}

//  Función que busca el delimitador de un lexema que empieza por '
void buscarDelimitadorComillaSimple(tipoelem * elemento){
    char caracter=siguienteCaracter();
    int encontrado=0, tam=strlen(elemento->lexema); // Variable para saber si se encontró el delimitador y variable para reservar el tamaño del lexema

    while(encontrado==0){ // Repetiremos el proceso hasta que se encuentre el delimitador
        tam++;
        if(caracter=='\''){ // Si encontramos comilla simple se ha llegado al delimitador, sino puede aparecer cualquier cosa
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter;
            encontrado=1;
        }
        else{
            if(caracter=='\n' || caracter=='$'){ // Si encontramos el \n o un $ (fin de fichero) error al delimitar el literal
                devolverCaracter();
                ImprimirError(7,numLinea);
                elemento->lexema=realloc(elemento->lexema, sizeof(char));  // Reservamos espacio en la cadena para un caracter
                elemento->lexema=calloc(1, sizeof(char));  // Lo inicializamos
                elemento->lexema[0]='\n';
                encontrado=1;
            }
            else{
                if(caracter=='\\'){ // Si aparece barra invertida y el siguiente es ' no cuenta como final
                    caracter=siguienteCaracter();
                    if(caracter=='\''){
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]='\\';// Insertamos la \ ya que abajo insertamos la '
                        tam++;
                        encontrado=0;
                    }
                }
            }
        }
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter; // Concatenamos el siguiente caracter a la cadena
            caracter=siguienteCaracter();
        }
        else{ // Si se encuentra el delimitador
            if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                ImprimirError(10,numLinea);
                elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                elemento->lexema = calloc(1, sizeof(char));
                elemento->lexema[0] = '\n';
            }
            else {
                elemento->compLex = CADENALITERAL;
            }
            nuevoLexema(); // Actualizamos los punteros del centinela
        }
    }
}

//  Función que busca el delimitador de un lexema que empieza por "
void buscarDelimitadorComillaDoble(tipoelem * elemento){
    char caracter=siguienteCaracter();
    int encontrado=0, tam=strlen(elemento->lexema); // Variable para saber si se encontró el delimitador y variable para reservar el tamaño del lexema

    while(encontrado==0){ // Repetiremos el proceso hasta que se encuentre el delimitador
        tam++;
        if(caracter=='\"'){ // Si encontramos comilla doble se ha llegado al delimitador, sino puede aparecer cualquier cosa
            elemento->lexema = realloc(elemento->lexema,(sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter;
            encontrado=1;
        }
        else{
            if(caracter=='\n' || caracter=='$'){ // Si encontramos el \n o un $ (fin de fichero) error al delimitar el literal
                devolverCaracter();
                ImprimirError(8,numLinea);
                elemento->lexema=realloc(elemento->lexema, sizeof(char));  // Reservamos espacio en la cadena para un caracter
                elemento->lexema=calloc(1, sizeof(char));  // Lo inicializamos
                elemento->lexema[0]='\n';
                encontrado=1;
            }
            else{
                if(caracter=='\\'){ // Si aparece barra invertida y el siguiente es " no cuenta como final
                    caracter=siguienteCaracter();
                    if(caracter=='\"'){
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]='\\';// Insertamos la \ ya que abajo insertamos la "
                        tam++;
                        encontrado=0;
                    }
                }
            }
        }
        if(encontrado==0){ // Si no se encuentra el delimitador, añadimos el caracter al lexema
            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
            elemento->lexema[tam-1]=caracter; // Concatenamos el siguiente caracter a la cadena
            caracter=siguienteCaracter();
        }
        else{ // Si se encuentra el delimitador
            if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                ImprimirError(10,numLinea);
                elemento->lexema = realloc(elemento->lexema,(sizeof(char))); // Reservamos espacio en la cadena para un caracter
                elemento->lexema = calloc(1, sizeof(char));
                elemento->lexema[0] = '\n';
            }
            else {
                elemento->compLex = CADENALITERAL;
            }
            nuevoLexema(); // Actualizamos los punteros del centinela
        }
    }
}


// Funciones de identificación de delimitadores

// Función que busca un delimitador válido para un identificador
int delimitadorCadenaEncontrado(char caracter){
    if(isalpha(caracter) || isdigit(caracter) || caracter=='_'){ // Si el caracter puede formar parte de la cadena
        return 0;
    }
    else{
        // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , - . /  : ; < = > ? @ [ \ ] ^ ` { | } ~
        if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<48) || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
            return 1;
        }
        else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de elemento->lexema
            ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
            return 1;
        }
    }
}

// Función que busca un delimitador válido para un entero
int delimitadorNumeroEncontrado(char caracter, tipoelem * elemento){
    int encontrado=0, tam=strlen(elemento->lexema), decimal=0;
    if(isdigit(caracter) || caracter=='_'){ // Si el caracter es un número o un _ puede formar parte del numero entero
        return 0;
    }
    else{
        if(caracter=='.'){ // si es un punto pasamos a modo decimal
            caracter=siguienteCaracter();
            if(isdigit(caracter)){
                tam++;
                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                elemento->lexema[tam-1]='.'; // Insertamos el .
                while(decimal==0){
                    decimal=delimitadorNumeroDecimalEncontrado(caracter,elemento);
                    caracter=siguienteCaracter();
                    if(decimal==1){
                        if(elemento->compLex!=NUMEROCIENTIFICO && elemento->compLex!=NUMERODECIMAL){
                            if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                ImprimirError(10,numLinea);
                                elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                elemento->lexema = calloc(1, sizeof(char));
                                elemento->lexema[0]= '\n';
                            }
                            else {
                                elemento->compLex = NUMERODECIMAL;
                            }
                        }
                    }
                }
                return decimal;
            }
            else{
                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                    ImprimirError(10,numLinea);
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                    elemento->lexema = calloc(1, sizeof(char));
                    elemento->lexema[0] = '\n';
                }
                else {
                    elemento->compLex = NUMEROENTERO;
                    ImprimirError(6, numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                }
                devolverCaracter();
                return 1;
            }
        }
        else{
            if(caracter=='e' || caracter=='E'){ // Entramos en modo científico
                char auxiliar=caracter;
                caracter=siguienteCaracter();
                if(caracter=='+' || caracter=='-'){ // si aparece un + o un menos comprobamos otro nivel mas para comprobar si aparece un número o hay que retroceder dos posiciones
                    char auxiliar2=caracter;
                    caracter=siguienteCaracter();
                    if(isdigit(caracter)){ // si aparece un número es un numero científico válido
                        tam++;
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=auxiliar; // Insertamos e o E
                        tam++;
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=auxiliar2; // Insertamos + o -
                        tam++;
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=caracter; // Insertamos el número
                        while(encontrado==0){
                            tam++;
                            caracter=siguienteCaracter();
                            if(isdigit(caracter)){ // si es número lo insertamos
                                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                elemento->lexema[tam-1]=caracter; // Insertamos el número
                            }
                            else{
                                // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
                                if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
                                    encontrado=1;
                                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                        ImprimirError(10,numLinea);
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                        elemento->lexema = calloc(1, sizeof(char));
                                        elemento->lexema[0] = '\n';
                                    }
                                    else {
                                        elemento->compLex = NUMEROCIENTIFICO;
                                    }
                                }
                                else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
                                    encontrado=1;
                                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                        ImprimirError(10,numLinea);
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                        elemento->lexema = calloc(1, sizeof(char));
                                        elemento->lexema[0] = '\n';
                                    }
                                    else{
                                        ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                                        elemento->compLex=NUMEROCIENTIFICO;
                                    }
                                    devolverCaracter();
                                }
                            }
                        }
                    }
                    else{ // Si no aparece un número no era un número científico, por lo que devolvemos el símbolo
                        if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                            ImprimirError(10,numLinea);
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                            elemento->lexema = calloc(1, sizeof(char));
                            elemento->lexema[0] = '\n';
                        }
                        else {
                            ImprimirError(6, numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                            elemento->compLex = NUMEROENTERO;
                        }
                        devolverCaracter();
                    }
                }
                else{
                    if(isdigit(caracter)){ // Si después del e aparece un número entramos en modo científico
                        tam++;
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=auxiliar; // Insertamos el e
                        tam++;
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                        elemento->lexema[tam-1]=caracter; // Insertamos el número
                        while(encontrado==0){
                            tam++;
                            caracter=siguienteCaracter();
                            if(isdigit(caracter)){ // si es número lo insertamos
                                elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                                elemento->lexema[tam-1]=caracter; // Insertamos el número
                            }
                            else{
                                // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
                                if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
                                    encontrado=1;
                                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                        ImprimirError(10,numLinea);
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                        elemento->lexema = calloc(1, sizeof(char));
                                        elemento->lexema[0] = '\n';
                                    }
                                    else {
                                        elemento->compLex = NUMEROCIENTIFICO;
                                    }
                                }
                                else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
                                    encontrado=1;
                                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                        ImprimirError(10,numLinea);
                                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                        elemento->lexema = calloc(1, sizeof(char));
                                        elemento->lexema[0] = '\n';
                                    }
                                    else{
                                        ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                                        elemento->compLex=NUMEROCIENTIFICO;
                                    }

                                }
                            }
                        }
                    }
                    else{
                        if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                            ImprimirError(10,numLinea);
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                            elemento->lexema = calloc(1, sizeof(char));
                            elemento->lexema[0] = '\n';
                        }
                        else {
                            ImprimirError(6, numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                            elemento->compLex = NUMEROENTERO;
                        }
                    }
                }
                return 1;
            }
            // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
            if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
                return 1;
            }
            else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
                ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                return 1;
            }

        }
    }
}

// Función que busca un delimitador válido para un decimal
int delimitadorNumeroDecimalEncontrado(char caracter, tipoelem * elemento){
    int encontrado=0, tam=strlen(elemento->lexema);
    if(isdigit(caracter) || caracter=='_'){ // Si el caracter es un número puede formar parte del decimal
        tam++;
        elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
        elemento->lexema[tam-1]=caracter; // Insertamos el número
        return 0;
    }
    else{
        if(caracter=='e' || caracter=='E'){ // Entramos en modo científico
            char auxiliar=caracter;
            caracter=siguienteCaracter();
            if(caracter=='+' || caracter=='-'){ // si aparece un + o un menos comprobamos otro nivel mas para comprobar si aparece un número o hay que retroceder dos posiciones
                char auxiliar2=caracter;
                caracter=siguienteCaracter();
                if(isdigit(caracter)){ // si aparece un número es un numero científico válido
                    tam++;
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=auxiliar; // Insertamos e o E
                    tam++;
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=auxiliar2; // Insertamos + o -
                    tam++;
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=caracter; // Insertamos el número
                    while(encontrado==0){
                        tam++;
                        caracter=siguienteCaracter();
                        if(isdigit(caracter)){ // si es número lo insertamos
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                            elemento->lexema[tam-1]=caracter; // Insertamos el número
                        }
                        else{
                            // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
                            if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
                                encontrado=1;
                                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                    ImprimirError(10,numLinea);
                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                    elemento->lexema = calloc(1, sizeof(char));
                                    elemento->lexema[0] = '\n';
                                }
                                else {
                                    elemento->compLex = NUMEROCIENTIFICO;
                                }
                            }
                            else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
                                encontrado=1;
                                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                    ImprimirError(10,numLinea);
                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                    elemento->lexema = calloc(1, sizeof(char));
                                    elemento->lexema[0] = '\n';
                                }
                                else {
                                    ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                                    elemento->compLex = NUMEROCIENTIFICO;
                                }
                            }
                        }
                    }
                }
                else{ // Si no aparece un número no era un número científico, por lo que devolvemos el símbolo
                    if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                        ImprimirError(10,numLinea);
                        elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                        elemento->lexema = calloc(1, sizeof(char));
                        elemento->lexema[0] = '\n';
                    }
                    else {
                        ImprimirError(6, numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                        elemento->compLex = NUMERODECIMAL;
                    }
                    devolverCaracter();
                }
            }
            else{
                if(isdigit(caracter)){ // Si después del e aparece un número entramos en modo científico
                    tam++;
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=auxiliar; // Insertamos el e
                    tam++;
                    elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                    elemento->lexema[tam-1]=caracter; // Insertamos el número
                    while(encontrado==0){
                        tam++;
                        caracter=siguienteCaracter();
                        if(isdigit(caracter)){ // si es número lo insertamos
                            elemento->lexema = realloc(elemento->lexema, (sizeof(char)*tam)); // Reservamos espacio en la cadena para un caracter más
                            elemento->lexema[tam-1]=caracter; // Insertamos el número
                        }
                        else{
                            // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
                            if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
                                encontrado=1;
                                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                    ImprimirError(10,numLinea);
                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                    elemento->lexema = calloc(1, sizeof(char));
                                    elemento->lexema[0] = '\n';
                                }
                                else {
                                    elemento->compLex = NUMEROCIENTIFICO;
                                }
                            }
                            else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
                                encontrado=1;
                                if(strlen(elemento->lexema)>tamMaxLexema){ // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                                    ImprimirError(10,numLinea);
                                    elemento->lexema = realloc(elemento->lexema, (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                                    elemento->lexema = calloc(1, sizeof(char));
                                    elemento->lexema[0] = '\n';
                                }
                                else {
                                    ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                                    elemento->compLex = NUMEROCIENTIFICO;
                                }
                            }
                        }
                    }
                }
                else {
                    if (strlen(elemento->lexema) >
                        tamMaxLexema) { // Si se excede el tamaño máximo de lexema no se devolverá el lexema y se intentará devolver el siguiente
                        ImprimirError(10, numLinea);
                        elemento->lexema = realloc(elemento->lexema,
                                                   (sizeof(char))); // Reservamos espacio en la cadena para un caracter
                        elemento->lexema = calloc(1, sizeof(char));
                        elemento->lexema[0] = '\n';
                    }
                    else{
                        ImprimirError(6, numLinea); // Imprimimos que se ha encontrado un caracter no esperado
                        elemento->compLex = NUMERODECIMAL;
                    }
                }
            }
            return 1;
        }
        // Si pertenece al conjunto de caracteres que delimitan la entrada --> Tabulación, line feed, Retorno de carro Espacio ! " # $ % & ' ( ) * + , -  /  : ; < = > ? @ [ \ ] ^ ` { | } ~  (NO ESTÁ EL PUNTO)
        if(caracter==9 || caracter==10 || caracter==13 || (caracter>31 && caracter<46) || caracter==47 || (caracter>57 && caracter<65) || (caracter>90 && caracter<95) || caracter==96 || (caracter>122 && caracter<127)){
            return 1;
        }
        else{ // Se ha encontrado un caracter que no se esperaba, se imprimirá un aviso, se devolverá todo lo que se tiene como lexema y se continuará tratando el caracter inesperado como principio de lexema
            ImprimirError(6,numLinea); // Imprimimos que se ha encontrado un caracter no esperado
            return 1;
        }
    }
}