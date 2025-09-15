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

void guardarEnHistorial(Libro libro) {
    FILE* archivo = fopen("Data/HistorialLibros.txt", "a");
    if (archivo == NULL) {
        perror("Error al abrir Historial.txt");
        return;
    }

    fprintf(archivo, "%s, %s, %s\n",
            libro.codigo,
            libro.nombre,
            libro.autor);

    fclose(archivo);
}

void guardarInventarioEnArchivo(Inventario* inventario) { 
    FILE* archivo = fopen("Data/Inventario.txt", "w"); 
    if (archivo == NULL) { 
        perror("Error al abrir el archivo"); 
        return; 
    } 
    while (inventario != NULL) { 
        fprintf(archivo, "%s,%s,%s,%.2f,%d\n", 
            inventario->libro.codigo, 
            inventario->libro.nombre, 
            inventario->libro.autor, 
            inventario->libro.precio, 
            inventario->cantidad); 
        inventario = inventario->siguiente; 
    } 
    fclose(archivo); 
}

void cambiarCantidad(Inventario* inventario, char* codigo, int cantidad) {
    if (inventario == NULL){
        printf("Inventario Vacio\n");
        return;
    }

    Inventario* inicio = inventario;//puntero al inicio de inventario

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
            return;
        }
        inventario = inventario->siguiente;
    }

    printf("Libro con código %s no encontrado\n", codigo);
}

void agregarAlInventario(Inventario** inventario) {
    Inventario *nuevo = malloc(sizeof(Inventario));
    if (!nuevo) {
        perror("malloc");
        exit(1);
    }
    nuevo->siguiente = NULL;

    nuevo->libro = construirLibro();
    guardarEnHistorial(nuevo->libro);

    printf("Ingresa la cantidad en Inventario: ");
    scanf("%d", &nuevo->cantidad);
    while (getchar() != '\n');  // limpiar el buffer

    if (*inventario == NULL) {
        *inventario = nuevo;
        guardarInventarioEnArchivo(*inventario);
        return;
    }

    Inventario* actual = *inventario;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo;
    guardarInventarioEnArchivo(*inventario);
}

void mostrarInventario(Inventario* inventario) {
    if (inventario == NULL) {
        printf("Inventario Vacío\n");
        return;
    }

    // Encabezado de la tabla
    printf("-------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-15s | %-10s | %-8s | %-10s |\n",
           "Título", "Autor", "Precio", "Cantidad", "Código");
    printf("-------------------------------------------------------------------------------------\n");

    // Filas de la tabla
    while (inventario != NULL) {
        printf("| %-20s | %-15s | %-10.2f | %-8d | %-10s |\n",
               inventario->libro.nombre,
               inventario->libro.autor,
               inventario->libro.precio,
               inventario->cantidad,
               inventario->libro.codigo);

        inventario = inventario->siguiente;
    }

    // Línea final
    printf("-------------------------------------------------------------------------------------\n");
}

void eliminarLibroInventario(Inventario** inventario, char* codigo) {
    if (*inventario == NULL) {
        printf("Inventario vacío\n");
        return;
    }

    Inventario* actual = *inventario;
    Inventario* anterior = NULL;

    while (actual != NULL) {
        if (strcmp(codigo, actual->libro.codigo) == 0) {
            //Caso: Se elimina el primer nodo
            if (anterior == NULL) {
                *inventario = actual->siguiente;
            //Caso: Se elimina algun otro nodo
            } else {
                anterior->siguiente = actual->siguiente;
            }

            free(actual->libro.codigo);
            free(actual->libro.nombre);
            free(actual->libro.autor);

            free(actual);

            guardarInventarioEnArchivo(*inventario);
            printf("Libro con código %s eliminado del inventario\n", codigo);
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    printf("Libro con código %s no encontrado en inventario\n", codigo);
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


void cargarInventarioDesdeArchivo(Inventario** inventario) {
    FILE* archivo = fopen("Data/Inventario.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        char codigo[50], nombre[100], autor[100];
        float precio;
        int cantidad;

        if (sscanf(linea, "%49[^,],%99[^,],%99[^,],%f,%d",
                   codigo, nombre, autor, &precio, &cantidad) == 5) {
            //Creacion libro
            Libro nuevoLibro;
            nuevoLibro.codigo = strdup(codigo);
            nuevoLibro.nombre = strdup(nombre);
            nuevoLibro.autor  = strdup(autor);
            nuevoLibro.precio = precio;

            // Creacion Nodo
            Inventario* nuevo = malloc(sizeof(Inventario));
            if (!nuevo) {
                perror("malloc");
                fclose(archivo);
                return;
            }
            nuevo->libro = nuevoLibro;
            nuevo->cantidad = cantidad;
            nuevo->siguiente = NULL;

            // Insertar al final de la lista
            if (*inventario == NULL) {
                *inventario = nuevo;
            } else {
                Inventario* actual = *inventario;
                while (actual->siguiente != NULL) {
                    actual = actual->siguiente;
                }
                actual->siguiente = nuevo;
            }
        } else {
            printf("Línea inválida en archivo: %s\n", linea);
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
