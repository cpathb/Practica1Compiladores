#include "gestionErrores.h"
#pragma once

// Función para leer bloque a bloque del fichero y guardarlo en el buffer centinela
struct centinela{
    char * cadena; // Texto contenido en el centinela
    int inicio; // Puntero de inicio de cadena a reconocer
    int fin; // Puntero al final de la cadena a reconocer
    int mitad; // Indica la situación de la cadena que analizamos: 0->izquierda, 1->derecha, 2->ambas  
    int cargado; //Indica cual fue el último bloque que se cargó, para que no se machaque por error: 0->izquierda, 1->derecha
};

// Inicializa las variables del sistema de entrada
void inicializarVariablesSistemaEntrada();

// Destruye el sistema de entrada
void destruirSistemaEntrada();

// Devuelve el siguiente caracter en el centinela 
char siguienteCaracter();

// Devuelve el tamaño máximo del lexema, que será el de tamaño de bloque
int tamañoMaximoLexema();

// Retrasa la posición fin del centinela a la posición anterior, debido a un exceso al tratar caracteres
void devolverCaracter();

// Fija la posicion inicio en la de fin, ya que se encontró un lexema
void nuevoLexema();

