# **Importante**
Este documento está generado empleando MarkDown, por lo que necesitarás de un 
intérprete para ver el resultado final. Si no tienes ningún interprete puedes 
utilizar el siguiente enlace para acceder a un interprete online:

> **[https://stackedit.io/](https://stackedit.io/) (25/10/2016)**

<br>
<br>
<br>
<br>
<br>
<br>

# **Practica 1 - Compiladores e Intérpretes**

<br>
<br>
<br>

# _**Analizador Léxico**_

<br>
<br>
<br>

## **Desarrollo**
Esta práctica ha sido desarrollada empleando el IDE [**CLion**](https://www.jetbrains.com/clion/?fromMenu) de jetbrains, por lo tanto, en la entrega se incluye también un archivo "CMakeLists.txt", por si se quiere emplear este IDE, para tener ya configurada la practica al abrir el proyecto.

<br>
<br>
<br>

## **Comando de compilación en terminal**
> **gcc main.c lista.c analizadorLexico.c tablaSimbolos.c tabla_hash_encadenamiento.c gestionErrores.c sistemaEntrada.c -o practica1.out**

<br>
<br>
<br>

## **Comando de ejecución en terminal**
> **./practica1.out**

<br>
<br>
<br>

## **Notas**
- Los archivos no deberán de moverse de la carpeta en la que estarán situados una vez se ha descomprimido el fichero .zip proporcionado en la entrega de la práctica, ya que si no, no se puede garantizar el correcto funcionamiento de la práctica.

- El tamaño de bloque se encuentra definido como una variable global en el sistema de entrada (sistemaEntrada.c), hay una instrucción comentada en la función inicializarSistemaEntrada que permite cambiar el tamaño de bloque a uno deseado.

- Para la detección de exceso de tamaño de lexema en el analizador léxico, he decidido que identificaré el lexema primero, y una vez identificado, comprobaré si su tamaño excede el máximo, de ser así, no lo trataré y buscaré a partir de el un lexema válido.