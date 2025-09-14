#include "Libro.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef INVENTARIO
#define INVENTARIO

// Inicializacion: Inventario* inventario = NULL;
// Inicializacion: cargarInventario(&inventario);
// Para agregar al inventario se usa la funcion agregarAlInventario() usando la referencia al inventario &inventario
// Se debe construir el libro usando construirLibro() antes de intentar construir un inventario usando la funcion construirInventario
// El resto de funciones actuales solo necesitan inventario como parametro excepto construirInventario que necesita un libro y su cantidad

typedef struct Inventario {
	Libro libro;
	int cantidad;
	struct Inventario *siguiente;
} Inventario;

void agregarAlInventario(Inventario** inventario) {
    Inventario *nuevo = malloc(sizeof(Inventario));
    if (!nuevo) {
        perror("malloc");
        exit(1);
    }
    nuevo->siguiente = NULL;

    nuevo->libro = construirLibro();

    printf("Ingresa la cantidad en Inventario: ");
    scanf("%d", &nuevo->cantidad);
    while (getchar() != '\n');  // limpiar el buffer

    if (*inventario == NULL) {
        *inventario = nuevo;
        return;
    }

    Inventario* actual = *inventario;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo;
}



void mostrarInventario(Inventario* inventario) {
	if (inventario == NULL){
		printf("Inventario Vacio\n");
	}
	while (inventario != NULL) {
		printf("\nTitulo: %s\nAutor: %s\nPrecio: %.2f\nCantidad: %d\nCodigo: %s\n",
			inventario->libro.nombre,
			inventario->libro.autor,
			inventario->libro.precio,
			inventario->cantidad,
			inventario->libro.codigo);

		inventario = inventario->siguiente;
	}
}

void guardarInventarioEnArchivo(Inventario* inventario) { 
    FILE* archivo = fopen("Data/Inventario.txt", "w"); 
    if (archivo == NULL) { 
        perror("Error al abrir el archivo"); 
        return; 
    } 
    while (inventario != NULL) { 
        fprintf(archivo, "Titulo: %s\nAutor: %s\nPrecio: %.2f\nCantidad: %d\nCodigo: %s\n\n", 
            inventario->libro.nombre, 
            inventario->libro.autor, 
            inventario->libro.precio, 
            inventario->cantidad, 
            inventario->libro.codigo); 
            inventario = inventario->siguiente; 
    } 
    fclose(archivo); 
}

void cambiarCantidad(Inventario* inventario) {
    if (inventario == NULL){
        printf("Inventario Vacio\n");
        return;
    }

    int cantidad;
    char* codigo;
    Inventario* inicio = inventario;//puntero al inicio de inventario

    printf("Ingresa el codigo del Libro: ");
    codigo = lecturaD();

    printf("Ingresa la cantidad del movimiento: ");
    scanf("%d", &cantidad);
    while (getchar() != '\n');//limpiar buffer

    while (inventario != NULL) {
        if (strcmp(codigo, inventario->libro.codigo) == 0) {
            if (inventario->cantidad + cantidad < 0) {
                printf("Stock insuficiente\n");
            } else {
                inventario->cantidad += cantidad;
                printf("Cambio en stock realizado\n");
                if (inicio !=NULL){
                    guardarInventarioEnArchivo(inicio);
                }
            }
            free(codigo);
            return;
        }
        inventario = inventario->siguiente;
    }

    printf("Libro con código %s no encontrado\n", codigo);
    free(codigo);
}

void cargaInventarioPorArchivo(Inventario* inventario) {
    FILE* archivo = fopen("Data/Carga.txt", "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return;
    }

    char linea[256];
    int numeroLinea = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        numeroLinea++;

        char codigo[100];
        int cantidad;

        if (sscanf(linea, "%99[^,],%d", codigo, &cantidad) != 2) {
            printf("Línea %d: Formato inválido -> %s", numeroLinea, linea);
            continue;
        }

        Inventario* actual = inventario;
        int encontrado = 0;

        while (actual != NULL) {
            if (strcmp(actual->libro.codigo, codigo) == 0) {
                encontrado = 1;
                if (actual->cantidad + cantidad < 0) {
                    printf("Línea %d: Stock insuficiente para código %s (actual %d, intento %d)\n",
                           numeroLinea, codigo, actual->cantidad, cantidad);
                } else {
                    actual->cantidad += cantidad;
                    printf("Línea %d: Actualizado %s, nueva cantidad %d\n",
                           numeroLinea, codigo, actual->cantidad);
                }
                break;
            }
            actual = actual->siguiente;
        }

        if (!encontrado) {
            printf("Línea %d: Código %s no encontrado en inventario\n", numeroLinea, codigo);
        }
    }

    fclose(archivo);
    guardarInventarioEnArchivo(inventario);
}


void cargarInventario(Inventario** inventario) {
    FILE* archivo = fopen("Data/Inventario.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        Inventario* nuevo = malloc(sizeof(Inventario));
        if (!nuevo) {
            perror("malloc");
            fclose(archivo);
            return;
        }
        nuevo->siguiente = NULL;

        Libro libro;

        //Lectura Titulo
        if (strncmp(linea, "Titulo:", 7) == 0) {
            char* valor = linea + 7; // saltar "Titulo:"
            while (*valor == ' ') valor++; // quitar espacios
            valor[strcspn(valor, "\n")] = 0; // quitar salto de línea
            libro.nombre = strdup(valor);
        }

        //Lectura Autor
        fgets(linea, sizeof(linea), archivo);
        if (strncmp(linea, "Autor:", 6) == 0) {
            char* valor = linea + 6;
            while (*valor == ' ') valor++;
            valor[strcspn(valor, "\n")] = 0;
            libro.autor = strdup(valor);
        }

        //Lectura Precio
        fgets(linea, sizeof(linea), archivo);
        if (strncmp(linea, "Precio:", 7) == 0) {
            char* valor = linea + 7;
            libro.precio = atof(valor);
        }

        //Lectura Cantidad
        fgets(linea, sizeof(linea), archivo);
        if (strncmp(linea, "Cantidad:", 9) == 0) {
            char* valor = linea + 9;
            nuevo->cantidad = atoi(valor);
        }

        //Lectura Codigo
        fgets(linea, sizeof(linea), archivo);
        if (strncmp(linea, "Codigo:", 7) == 0) {
            char* valor = linea + 7;
            while (*valor == ' ') valor++;
            valor[strcspn(valor, "\n")] = 0;
            libro.codigo = strdup(valor);
        }

        nuevo->libro = libro;

        // saltar línea en blanco entre libros
        fgets(linea, sizeof(linea), archivo);

        // insertar al final de la lista
        if (*inventario == NULL) {
            *inventario = nuevo;
        } else {
            Inventario* actual = *inventario;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }

    fclose(archivo);
}

char* lecturaD() {
    int c;
    char *string = malloc(1);
    string[0] = '\0';
	int i = 0;
	while ((c = getchar()) != '\n' && c != EOF) {
		string = realloc(string, i + 2);
		string[i] = (char)c;
		string[i+1] = '\0';
		i++;
	}

    return string;
}


#endif // !"INVENTARIO"
