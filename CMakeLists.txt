cmake_minimum_required(VERSION 2.8.4)
project(Practica1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES main.c gestionErrores.c analizadorLexico.c lista.c tabla_hash_encadenamiento.c tablaSimbolos.c sistemaEntrada.c)
add_executable(dbsg ${SOURCE_FILES})