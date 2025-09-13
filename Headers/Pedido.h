#ifndef PEDIDO_H
#define PEDIDO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Inventario.h"
#include "Clientes.h"

#define ARCHIVO_FACTURAS "Data/Facturas.txt"
#define MAX_LINEA 256
// --------------------- ESTRUCTURAS ---------------------
typedef struct LineaPedido {
    char codigoLibro[100];
    int cantidad;
    float precio;
    struct LineaPedido* siguiente;
} LineaPedido;

typedef struct Pedido {
    LineaPedido* lineas;
    int numLineas;
    char cedulaCliente[MAX_CEDULA];
    char fecha[20];
    float subtotal;
    float total;
    struct Pedido* siguiente;  // Para lista de facturas
} Pedido;

// --------------------- FUNCIONES ---------------------

// Construir línea de pedido
LineaPedido* construirLineaPedido(const char* codigoLibro, float precio, int cantidad) {
    LineaPedido* nueva = (LineaPedido*)malloc(sizeof(LineaPedido));
    if (!nueva) { 
        printf("Error al asignar memoria\n"); 
        exit(1); 
    }
    strncpy(nueva->codigoLibro, codigoLibro, sizeof(nueva->codigoLibro) - 1);
    nueva->codigoLibro[sizeof(nueva->codigoLibro) - 1] = '\0';
    nueva->cantidad = cantidad;
    nueva->precio = precio;
    nueva->siguiente = NULL;
    return nueva;
}

// Construir pedido
Pedido* construirPedido(const char* cedulaCliente, const char* fecha) {
    Pedido* p = (Pedido*)malloc(sizeof(Pedido));
    if (!p) { 
        printf("Error al asignar memoria\n"); 
        exit(1); 
    }
    strncpy(p->cedulaCliente, cedulaCliente, sizeof(p->cedulaCliente) - 1);
    p->cedulaCliente[sizeof(p->cedulaCliente) - 1] = '\0';
    strncpy(p->fecha, fecha, sizeof(p->fecha) - 1);
    p->fecha[sizeof(p->fecha) - 1] = '\0';
    p->lineas = NULL;
    p->numLineas = 0;
    p->subtotal = 0;
    p->total = 0;
    p->siguiente = NULL;
    return p;
}
void trim(char* str) {
    // Elimina espacios al inicio
    while(*str == ' ') str++;

    // Elimina espacios y salto de línea al final
    char* end = str + strlen(str) - 1;
    while(end > str && (*end == ' ' || *end == '\n')) {
        *end = '\0';
        end--;
    }
}

void liberarFacturas(Pedido* listaFacturas) {
    Pedido* act = listaFacturas;
    while (act) {
        Pedido* tmp = act;
        act = act->siguiente;
        liberarPedido(tmp);
    }
}
// Liberar toda la lista de facturas
void liberarFacturas(Pedido* listaFacturas) {
    Pedido* act = listaFacturas;
    while (act) {
        Pedido* tmp = act;
        act = act->siguiente;
        liberarPedido(tmp);
    }
}



// Mostrar carrito
void verLineas(Pedido* p) {
    if (!p || !p->lineas) { 
        printf("\n---- Carrito vacío ----\n"); 
        return; 
    }

    printf("\n---- Carrito actual ----\n");
    LineaPedido* aux = p->lineas;
    int num = 1;
    while (aux) {
        printf("%d) Libro: %s | Cantidad: %d | Precio: %.2f | Subtotal: %.2f\n",
               num, aux->codigoLibro, aux->cantidad, aux->precio, aux->cantidad * aux->precio);
        aux = aux->siguiente;
        num++;
    }
}

// Obtener fecha actual
void obtenerFechaActual(char* buffer, int tam) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, tam, "%02d/%02d/%04d",
             tm.tm_mday,
             tm.tm_mon + 1,
             tm.tm_year + 1900);
}

// --------------------- GESTIÓN DEL PEDIDO ---------------------

int codigoValido(const char* code, int cantidad, float* precio, Inventario* inventario) {
    while (inventario) {
        if (strcmp(inventario->libro.codigo, code) == 0) {
            if (cantidad <= inventario->cantidad) {
                *precio = inventario->libro.precio;
                return 1;
            } else {
                printf("Stock insuficiente: %d\n", inventario->cantidad);
                return 0; 
            }
        }
        inventario = inventario->siguiente;
    }
    return 0;
}

void agregarLineaAlPedido(Pedido* pedido, const char* codigo, int cantidad, float precio) {
    if (!pedido) return;

    char codigoLimpio[100];
    strncpy(codigoLimpio, codigo, sizeof(codigoLimpio));
    trim(codigoLimpio);

    LineaPedido* act = pedido->lineas;
    LineaPedido* prev = NULL;
    while (act) {
        char codigoExistente[100];
        strncpy(codigoExistente, act->codigoLibro, sizeof(codigoExistente));
        trim(codigoExistente);

        if (strcmp(codigoExistente, codigoLimpio) == 0) {
            act->cantidad += cantidad;
            return;
        }
        prev = act;
        act = act->siguiente;
    }

    LineaPedido* nueva = construirLineaPedido(codigoLimpio, precio, cantidad);
    if (!prev) pedido->lineas = nueva;
    else prev->siguiente = nueva;
    pedido->numLineas++;
}


void agregarLibro(Inventario* inventario, Pedido* pedidoActual) {
    int CantidadL;
    char CodigoL[100];
    float precio;

    printf("Ingrese código y cantidad separados por coma (ej: A123,5): ");
    if (scanf(" %99[^,],%d", CodigoL, &CantidadL) == 2) {
        if (codigoValido(CodigoL, CantidadL, &precio, inventario)  == 1) {
            agregarLineaAlPedido(pedidoActual, CodigoL, CantidadL, precio);
            printf("Línea agregada al pedido.\n");
        } else {
            printf("Código no existe o stock insuficiente.\n");
        }
    } else {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        printf("Entrada inválida. Use el formato codigo,cantidad (ej: A123,5).\n");
    }
}

void eliminarLibro(Pedido* pedidoActual) {
    if (!pedidoActual || !pedidoActual->lineas) { 
        printf("No hay líneas.\n"); 
        return; 
    }

    int linea;
    printf("Ingrese número de línea a eliminar: ");
    if (scanf("%d", &linea) != 1) { while (getchar() != '\n'); printf("Entrada inválida.\n"); return; }

    LineaPedido* act = pedidoActual->lineas;
    LineaPedido* prev = NULL;
    int cont = 1;
    while (act) {
        if (cont == linea) {
            if (!prev) pedidoActual->lineas = act->siguiente;
            else prev->siguiente = act->siguiente;
            free(act);
            pedidoActual->numLineas--;
            printf("Línea %d eliminada.\n", linea);
            return;
        }
        prev = act;
        act = act->siguiente;
        cont++;
    }
    printf("Línea %d no encontrada.\n", linea);
}

// --------------------- FACTURACIÓN ---------------------

void guardarFactura(Pedido* pedido) {
    if (!pedido) { printf("ERROR: pedido es NULL\n"); return; }
    printf("---- FACTURA ----\n");
    fflush(stdout); // Asegura que se imprima inmediatamente

    FILE* f = fopen(ARCHIVO_FACTURAS, "a");
    if (!f) { 
        printf("Error al abrir archivo\n"); 
        fflush(stdout);
        return; 
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"cliente\": \"%s\",\n", pedido->cedulaCliente);
    fprintf(f, "  \"fecha\": \"%s\",\n", pedido->fecha);
    fprintf(f, "  \"lineas\": [\n");

    LineaPedido* aux = pedido->lineas;
    while (aux) {
        fprintf(f, "    {\"codigo\": \"%s\", \"cantidad\": %d, \"precio\": %.2f, \"subtotal\": %.2f}",
                aux->codigoLibro, aux->cantidad, aux->precio, aux->cantidad * aux->precio);
        if (aux->siguiente) fprintf(f, ",\n");
        else fprintf(f, "\n");
        aux = aux->siguiente;
    }

    fprintf(f, "  ],\n");
    fprintf(f, "  \"subtotal\": %.2f,\n", pedido->subtotal);
    fprintf(f, "  \"total\": %.2f\n", pedido->total);
    fprintf(f, "}\n\n");

    fclose(f); // Muy importante cerrar el archivo
}


// Generar pedido y agregar a la lista de facturas
void generarPedido(Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas) {
    if (!*pedidoActual) return;

    float subtotal = 0;
    LineaPedido* aux = (*pedidoActual)->lineas;

    while (aux) {
        subtotal += aux->cantidad * aux->precio;

        Inventario* libro = inventario;
        while (libro) {
            if (strcmp(libro->libro.codigo, aux->codigoLibro) == 0) {
                libro->cantidad -= aux->cantidad;
                break;
            }
            libro = libro->siguiente;
        }
        aux = aux->siguiente;
    }

    (*pedidoActual)->subtotal = subtotal;
    (*pedidoActual)->total = subtotal * 1.13f;

    printf("Subtotal: %.2f | Total: %.2f\n", (*pedidoActual)->subtotal, (*pedidoActual)->total);
    printf("DEBUG: llamando a guardarFactura...\n");
    guardarFactura(*pedidoActual);
    printf("DEBUG: se lo paso por el culo...\n");

    // Insertar en lista de facturas
    (*pedidoActual)->siguiente = *listaFacturas;
    *listaFacturas = *pedidoActual;

    *pedidoActual = NULL;
}


// Cargar facturas desde archivo
void cargarFacturas(Pedido** listaFacturas) {
    FILE* f = fopen(ARCHIVO_FACTURAS, "r");
    if (!f) {
        printf("No se pudo abrir archivo %s\n", ARCHIVO_FACTURAS);
        return;
    }

    char linea[MAX_LINEA];
    Pedido* pedido = NULL;
    LineaPedido* ultimaLinea = NULL;
    int dentroLineas = 0;

    while (fgets(linea, MAX_LINEA, f)) {
        trim(linea);
        if (strlen(linea) == 0 || strcmp(linea, "{") == 0 || strcmp(linea, "}") == 0)
            continue;

        if (strstr(linea, "\"cliente\"")) {
            pedido = (Pedido*)malloc(sizeof(Pedido));
            memset(pedido, 0, sizeof(Pedido));
            char valor[100];
            sscanf(linea, " \"cliente\": \"%[^\"]\"", valor);
            strncpy(pedido->cedulaCliente, valor, sizeof(pedido->cedulaCliente)-1);
            pedido->lineas = NULL;
            pedido->numLineas = 0;
            ultimaLinea = NULL;
        }
        else if (strstr(linea, "\"fecha\"")) {
            char valor[20];
            sscanf(linea, " \"fecha\": \"%[^\"]\"", valor);
            strncpy(pedido->fecha, valor, sizeof(pedido->fecha)-1);
        }
        else if (strstr(linea, "\"lineas\"")) {
            dentroLineas = 1;
        }
        else if (dentroLineas && strstr(linea, "\"codigo\"")) {
            LineaPedido* lp = (LineaPedido*)malloc(sizeof(LineaPedido));
            memset(lp, 0, sizeof(LineaPedido));
            char codigo[100];
            int cantidad = 0;
            float precio = 0.0f;
            sscanf(linea,
                   " {\"codigo\": \"%[^\"]\", \"cantidad\": %d, \"precio\": %f",
                   codigo, &cantidad, &precio);
            strncpy(lp->codigoLibro, codigo, sizeof(lp->codigoLibro)-1);
            lp->cantidad = cantidad;
            lp->precio = precio;
            lp->siguiente = NULL;

            if (!pedido->lineas) pedido->lineas = lp;
            else ultimaLinea->siguiente = lp;
            ultimaLinea = lp;
            pedido->numLineas++;
        }
        else if (strstr(linea, "\"total\"")) {
            char valor[50];
            sscanf(linea, " \"total\": %f", &pedido->total);
            // Insertamos el pedido en la lista
            pedido->siguiente = *listaFacturas;
            *listaFacturas = pedido;
            pedido = NULL;
            dentroLineas = 0;
        }
        else if (strstr(linea, "\"subtotal\"")) {
            sscanf(linea, " \"subtotal\": %f", &pedido->subtotal);
        }
    }

    fclose(f);
}





// Registrar pedido completo
void registrarPedido(Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas) {
    char cedula[MAX_CEDULA];
    char fecha[20];
    obtenerFechaActual(fecha, sizeof(fecha));

    printf("Fecha del pedido: %s\n", fecha);
    printf("Ingrese cédula del cliente: ");
    scanf("%19s", cedula);

    if (existe(cedula)) {
        *pedidoActual = construirPedido(cedula, fecha);
        int salir = 0, opcion;
        while (!salir) {
            printf("\n--- Menú de Pedido ---\n");
            mostrarInventario(inventario);
            verLineas(*pedidoActual);

            printf("1. Agregar libro\n2. Eliminar línea\n3. Generar pedido\n0. Salir\nSeleccione opción: ");
            scanf("%d", &opcion);

            switch(opcion){
                case 1: agregarLibro(inventario, *pedidoActual); break;
                case 2: eliminarLibro(*pedidoActual); break;
                case 3: generarPedido(inventario, pedidoActual, listaFacturas); salir = 1; break;
                case 0: 
                    if (*pedidoActual) {
                        liberarPedido(*pedidoActual);
                        *pedidoActual = NULL;
                    }
                    salir = 1;
                    break;
                default: printf("Opción no válida.\n"); break;
            }
        }
    } else {
        printf("No existe el cliente.\n");
    }
}

void imprimirFacturas(Pedido* listaFacturas) {
    if (!listaFacturas) {
        printf("No hay facturas cargadas.\n");
        return;
    }

    Pedido* p = listaFacturas;
    int numPedido = 1;

    while (p) {
        printf("\n--- Factura %d ---\n", numPedido);
        printf("Cliente: %s\n", p->cedulaCliente);
        printf("Fecha: %s\n", p->fecha);
        printf("Subtotal: %.2f | Total: %.2f\n", p->subtotal, p->total);

        LineaPedido* linea = p->lineas;
        int numLinea = 1;
        while (linea) {
            printf("  %d) Código: %s | Cantidad: %d | Precio: %.2f | Subtotal: %.2f\n",
                   numLinea, linea->codigoLibro, linea->cantidad, linea->precio, linea->cantidad * linea->precio);
            linea = linea->siguiente;
            numLinea++;
        }

        p = p->siguiente;
        numPedido++;
    }
}

#endif // PEDIDO_H
