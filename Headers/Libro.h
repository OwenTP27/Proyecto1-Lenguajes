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


char* generarCodigo() {
    FILE* archivo = fopen("Data/HistorialLibros.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo\n");
        return NULL;
    }

    char linea[256];
    int numFilas = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        //Por si llega a haber lineas vacias
        if (strlen(linea) > 1) {
            numFilas++;
        }
    }
    fclose(archivo);

    numFilas++;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "L%d", numFilas);

    // memoria dinamica del buffer+1
    char* codigo = (char*)malloc(strlen(buffer) + 1);
    strcpy(codigo, buffer);

    return codigo;
}

Libro construirLibro() {
    char* input;
    Libro nuevoLibro;
    nuevoLibro.codigo = generarCodigo();

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
