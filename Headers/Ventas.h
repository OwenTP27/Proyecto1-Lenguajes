#ifndef VENTAS
#define VENTAS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pedido.h"

void limpiarCadena(char* str) {
    int len = strlen(str);
    while(len > 0 && (str[len-1] == '\n' || str[len-1] == ' ' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

typedef struct LibroVenta {
    char codigo[100];
    char nombre[200];
    char autor[100];
    int cantidadVendida;
    float subtotal;
} LibroVenta;

typedef struct AutorVenta {
    char nombre[100];
    int cantidadVendida;
    float subtotal;
} AutorVenta;

void top5LibrosMasVendidos(Pedido* listaPedidos) {
    FILE* file = fopen("Data/HistorialLibros.txt", "r");
    if (!file) {
        printf("No se pudo abrir HistorialLibros.txt\n");
        return;
    }

    LibroVenta libros[200];
    int totalLibros = 0;
    char linea[256];

    while (fgets(linea, sizeof(linea), file)) {
        char* token = strtok(linea, ",");
        if (!token) continue;
        strcpy(libros[totalLibros].codigo, token);
        limpiarCadena(libros[totalLibros].codigo);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(libros[totalLibros].nombre, token);
        limpiarCadena(libros[totalLibros].nombre);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(libros[totalLibros].autor, token);
        limpiarCadena(libros[totalLibros].autor);

        libros[totalLibros].cantidadVendida = 0;
        libros[totalLibros].subtotal = 0.0f;
        totalLibros++;
    }
    fclose(file);

    // Recorrer pedidos y acumular ventas y subtotal
    Pedido* pedidoActual = listaPedidos;
    while (pedidoActual != NULL) {
        LineaPedido* lineaActual = pedidoActual->lineas;
        while (lineaActual != NULL) {
            limpiarCadena(lineaActual->codigoLibro);
            for (int i = 0; i < totalLibros; i++) {
                if (strcmp(lineaActual->codigoLibro, libros[i].codigo) == 0) {
                    libros[i].cantidadVendida += lineaActual->cantidad;
                    libros[i].subtotal += lineaActual->cantidad * lineaActual->precio;
                    break;
                }
            }
            lineaActual = lineaActual->siguiente;
        }
        pedidoActual = pedidoActual->siguiente;
    }

    // Ordenar libros por cantidadVendida (descendente)
    for (int i = 0; i < totalLibros - 1; i++) {
        for (int j = i + 1; j < totalLibros; j++) {
            if (libros[j].cantidadVendida > libros[i].cantidadVendida) {
                LibroVenta temp = libros[i];
                libros[i] = libros[j];
                libros[j] = temp;
            }
        }
    }

    // Imprimir Top 5 con código y subtotal
    printf("=== Top 5 Libros Más Vendidos ===\n");
    printf("%-10s | %-30s | %-20s | %-10s | %-10s\n", 
           "Código", "Nombre", "Autor", "Cantidad", "Subtotal");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < 5 && i < totalLibros; i++) {
        printf("%-10s | %-30s | %-20s | %-10d | %-10.2f\n",
               libros[i].codigo, libros[i].nombre, libros[i].autor,
               libros[i].cantidadVendida, libros[i].subtotal);
    }
    printf("presione Enter para continuar...");
    getchar();
    getchar();
}



void top5AutoresMasVendidos(Pedido* listaPedidos) {
    FILE* file = fopen("Data/HistorialLibros.txt", "r");
    if (!file) {
        printf("No se pudo abrir HistorialLibros.txt\n");
        return;
    }

    // Leer historial de libros
    char codigos[200][100];
    char autores[200][100];
    int totalLibros = 0;
    char linea[256];

    while (fgets(linea, sizeof(linea), file)) {
        char* token = strtok(linea, ",");
        if (!token) continue;
        strcpy(codigos[totalLibros], token);
        limpiarCadena(codigos[totalLibros]);

        token = strtok(NULL, ","); // nombre, lo ignoramos aquí
        token = strtok(NULL, ",\n"); // autor
        if (!token) continue;
        strcpy(autores[totalLibros], token);
        limpiarCadena(autores[totalLibros]);

        totalLibros++;
    }
    fclose(file);

    AutorVenta listaAutores[200];
    int totalAutores = 0;

    // Recorrer pedidos
    Pedido* pedidoActual = listaPedidos;
    while (pedidoActual != NULL) {
        LineaPedido* lineaActual = pedidoActual->lineas;
        while (lineaActual != NULL) {
            limpiarCadena(lineaActual->codigoLibro);

            // Buscar el autor del libro
            char autorLibro[100] = "";
            for (int i = 0; i < totalLibros; i++) {
                if (strcmp(lineaActual->codigoLibro, codigos[i]) == 0) {
                    strcpy(autorLibro, autores[i]);
                    break;
                }
            }

            if (strlen(autorLibro) > 0) {
                // Buscar si ya está en listaAutores
                int encontrado = 0;
                for (int i = 0; i < totalAutores; i++) {
                    if (strcmp(listaAutores[i].nombre, autorLibro) == 0) {
                        listaAutores[i].cantidadVendida += lineaActual->cantidad;
                        listaAutores[i].subtotal += lineaActual->cantidad * lineaActual->precio;
                        encontrado = 1;
                        break;
                    }
                }
                // Si no estaba, agregarlo
                if (!encontrado) {
                    strcpy(listaAutores[totalAutores].nombre, autorLibro);
                    listaAutores[totalAutores].cantidadVendida = lineaActual->cantidad;
                    listaAutores[totalAutores].subtotal = lineaActual->cantidad * lineaActual->precio;
                    totalAutores++;
                }
            }

            lineaActual = lineaActual->siguiente;
        }
        pedidoActual = pedidoActual->siguiente;
    }

    // Ordenar autores por cantidadVendida (descendente)
    for (int i = 0; i < totalAutores - 1; i++) {
        for (int j = i + 1; j < totalAutores; j++) {
            if (listaAutores[j].cantidadVendida > listaAutores[i].cantidadVendida) {
                AutorVenta temp = listaAutores[i];
                listaAutores[i] = listaAutores[j];
                listaAutores[j] = temp;
            }
        }
    }

    // Imprimir Top 5 autores
    printf("=== Top 5 Autores Más Vendidos ===\n");
    printf("%-25s | %-10s | %-10s\n", "Autor", "Cantidad", "Subtotal");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < 5 && i < totalAutores; i++) {
        printf("%-25s | %-10d | %-10.2f\n",
               listaAutores[i].nombre,
               listaAutores[i].cantidadVendida,
               listaAutores[i].subtotal);
    }
    printf("presione Enter para continuar...");
    getchar();
    getchar();
}


#endif //!VENTAS
