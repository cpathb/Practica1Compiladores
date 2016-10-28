// Definicion de los tipos literales
#define NUMEROENTERO 409
#define NUMEROBINARIO 410
#define NUMEROHEXADECIMAL 411
#define NUMEROCIENTIFICO 412
#define NUMERODECIMAL 413
#define CADENALITERAL 414
#define COMENTARIODOCUMENTACION 415

// Definición de los operadores dobles ==, !=, >=, <=, =>,=<, +=, -=, *=, /=, ++, --, ..
#define IGUALIGUAL 416
#define DISTINTO 417
#define MAYORIGUAL 418
#define MENORIGUAL 419
#define IGUALMAYOR 420
#define IGUALMENOR 421
#define MASIGUAL 422
#define MENOSIGUAL 423
#define PORIGUAL 424
#define BARRAIGUAL 425
#define MASMAS 426
#define MENOSMENOS 427
#define RANGO 428

// Para aceptar más operadores dobles, triples o en adelante, sería necesario añadir aqui su definición
// y en la función de tratamiento de lexema de la tabla de símbolos, realizar la comprobación oportuna para este operador.