#ifndef LIBRO
#define	LIBRO
#include <stdlib.h>
#include <string.h>

typedef struct Libro {
	char *codigo;
	char *nombre;
	char *autor;
	float precio;
} Libro;

/**
 * lecturaD
 * Función auxiliar para leer una cadena desde la entrada estándar.
 * Entradas: ninguna.
 * Retorno: puntero a char con la cadena leída (dinámicamente asignada).
 */
char* lecturaD();

/**
 * generarCodigo
 * Genera un código único para un libro basado en el número de registros en el archivo HistorialLibros.txt.
 * Entradas: ninguna.
 * Retorno: puntero a char con el código generado (dinámicamente asignado) o NULL si ocurre un error.
 */
char* generarCodigo() {

    FILE* archivo = fopen("Data/HistorialLibros.txt", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo\n");
        return NULL;
    }

    char linea[256];
    int numFilas = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        // Por si llega a haber líneas vacías
        if (strlen(linea) > 1) {
            numFilas++;
        }
    }
    fclose(archivo);

    numFilas++;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "L%d", numFilas);

    // memoria dinámica del buffer+1
    char* codigo = (char*)malloc(strlen(buffer) + 1);
    strcpy(codigo, buffer);

    return codigo;
}

/**
 * construirLibro
 * Construye un libro pidiendo al usuario que ingrese nombre, autor y precio.
 * Entradas: ninguna.
 * Retorno: estructura Libro con los datos ingresados y código generado automáticamente.
 */
Libro construirLibro() {

    char* input;
    Libro nuevoLibro;
    nuevoLibro.codigo = generarCodigo();
    // Limpiar buffer por si hay restos de scanf anteriores
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

    printf("\nEl libro se regitro con exito.\n");
    printf("Presione Enter para continuar...\n");
    getchar();
    getchar();

    

    return nuevoLibro;
}

#endif // !LIBRO
