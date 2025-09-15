#ifndef LIBROVENTAS
#define LIBROVENTAS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pedido.h"
#include "Inventario.h"

typedef struct LibroVenta {
    char codigo[100];
    char nombre[200];
    char autor[100];
    int cantidadVendida;
    float totalVendido;
    struct LibroVenta* siguiente;
} LibroVenta;

// Buscar libro en la lista de ventas acumuladas
LibroVenta* buscarLibroVenta(LibroVenta* ventas, const char* codigo) {
    while (ventas != NULL) {
        if (strcmp(ventas->codigo, codigo) == 0) {
            return ventas;
        }
        ventas = ventas->siguiente;
    }
    return NULL;
}

// Leer historial y buscar nombre y autor por código
void buscarDatosEnHistorial(const char* codigo, char* nombre, char* autor) {
    FILE* archivo = fopen("Data/HistorialLibros.txt", "r");
    if (!archivo) {
        perror("Error al abrir historial");
        strcpy(nombre, "Desconocido");
        strcpy(autor, "Desconocido");
        return;
    }

    char linea[512];
    int encontrado = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        char cod[50], nom[200], aut[100];
        if (sscanf(linea, "%49[^,], %199[^,], %99[^\n]", cod, nom, aut) == 3) {
            if (strcmp(codigo, cod) == 0) {
                strcpy(nombre, nom);
                strcpy(autor, aut);
                encontrado = 1;
                break;
            }
        }
    }

    if (!encontrado) {
        strcpy(nombre, "Desconocido");
        strcpy(autor, "Desconocido");
    }

    fclose(archivo);
}

// Comparador para qsort (de mayor a menor por total vendido)
int compararVentas(const void* a, const void* b) {
    LibroVenta* lvA = *(LibroVenta**)a;
    LibroVenta* lvB = *(LibroVenta**)b;
    if (lvB->totalVendido > lvA->totalVendido) return 1;
    if (lvB->totalVendido < lvA->totalVendido) return -1;
    return 0;
}

void top5LibrosMasVendidos(Pedido* listaFacturas) {
    LibroVenta* ventas = NULL;

    // Recorrer todos los pedidos
    Pedido* pedido = listaFacturas;
    while (pedido != NULL) {
        LineaPedido* linea = pedido->lineas;
        while (linea != NULL) {
            // Buscar si ya está en ventas
            LibroVenta* lv = buscarLibroVenta(ventas, linea->codigoLibro);
            if (lv == NULL) {
                // Crear nuevo nodo
                lv = malloc(sizeof(LibroVenta));
                strcpy(lv->codigo, linea->codigoLibro);
                // Obtener nombre y autor del historial
                buscarDatosEnHistorial(linea->codigoLibro, lv->nombre, lv->autor);

                lv->cantidadVendida = 0;
                lv->totalVendido = 0;
                lv->siguiente = ventas;
                ventas = lv;
            }

            // Acumular ventas
            lv->cantidadVendida += linea->cantidad;
            lv->totalVendido += linea->cantidad * linea->precio;

            linea = linea->siguiente;
        }
        pedido = pedido->siguiente;
    }

    // Contar cuántos libros distintos hay
    int n = 0;
    LibroVenta* actual = ventas;
    while (actual != NULL) {
        n++;
        actual = actual->siguiente;
    }

    if (n == 0) {
        printf("No hay ventas registradas.\n");
        return;
    }

    // Pasar a array para ordenar
    LibroVenta** array = malloc(n * sizeof(LibroVenta*));
    actual = ventas;
    for (int i = 0; i < n; i++) {
        array[i] = actual;
        actual = actual->siguiente;
    }

    // Ordenar por total vendido
    qsort(array, n, sizeof(LibroVenta*), compararVentas);

    // Mostrar top 5
    printf("\n--- Top 5 Libros más vendidos ---\n");
    for (int i = 0; i < n && i < 5; i++) {
        printf("%d) Codigo: %s | Nombre: %s | Autor: %s | Cantidad: %d | Total: %.2f\n",
               i + 1,
               array[i]->codigo,
               array[i]->nombre,
               array[i]->autor,
               array[i]->cantidadVendida,
               array[i]->totalVendido);
    }

    free(array);
    // 🔹 Aquí puedes liberar la lista ventas si quieres
}


#endif //!LIBROVENTAS
