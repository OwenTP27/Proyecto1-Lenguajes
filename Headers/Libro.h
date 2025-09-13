#ifndef LIBRO
#define	LIBRO
#include <stdlib.h>
#include <string.h>

typedef struct Libro {
	char *codigo;
	char *nombre;
	char *autor;
	float precio;
}Libro;

char* lecturaD();

Libro construirLibro() {
    char* input;
    Libro nuevoLibro;

    printf("Ingresa el codigo del Libro: ");
    input = lecturaD();
    nuevoLibro.codigo = strdup(input);
    free(input);

    printf("Ingresa el nombre del Libro: ");
    input = lecturaD();
    nuevoLibro.nombre = strdup(input);
    free(input);

    printf("Ingresa el autor del Libro: ");
    input = lecturaD();
    nuevoLibro.autor = strdup(input);
    free(input);

    printf("Ingresa el precio del libro: ");
    scanf("%f", &nuevoLibro.precio);
    while (getchar() != '\n');  // limpiar buffer

    return nuevoLibro;
}


#endif // !LIBRO
