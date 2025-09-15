#ifndef PEDIDO_H
#define PEDIDO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Inventario.h"
#include "Clientes.h"
#include "LectorJSON.h"
#include <ctype.h>


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
    int id;  
    struct Pedido* siguiente;  // Para lista de facturas
} Pedido;
  
void agregarLibro(Inventario* inventario, Pedido* pedidoActual);

// --------------------- FUNCIONES ---------------------
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}
void mostrarencabezado(Config info) {
    printf("---- %s ----\n", info.libreria.nombre);
    printf("Teléfono: %s | Cédula Jurídica: %s\n", info.libreria.telefono, info.libreria.cedula_juridica);
    printf("Horario: %s\n", info.libreria.horario);
}
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
    p->id = 0; 
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

void liberarPedido(Pedido* p) {
    if (!p) return;
    LineaPedido* linea = p->lineas;
    while (linea) {
        LineaPedido* tmp = linea;
        linea = linea->siguiente;
        free(tmp);
    }
    free(p);
}

void liberarFacturas(Pedido* listaFacturas) {
    Pedido* act = listaFacturas;
    while (act) {
        Pedido* tmp = act;
        act = act->siguiente;
        liberarPedido(tmp);
    }
}



const char* obtenerNombreLibro(Inventario* inv, const char* codigo) {
    while (inv) {
        if (strcmp(inv->libro.codigo, codigo) == 0) return inv->libro.nombre;
        inv = inv->siguiente;
    }
    return "Desconocido";
}
// Mostrar carrito
void verLineas(Pedido* p, Inventario* inventario) {
    if (!p || !p->lineas) {
        printf("\n---- Carrito vacío ----\n");
        return;
    }

    printf("\n---- Carrito actual ----\n");
    LineaPedido* aux = p->lineas;
    int num = 1;
    
    while (aux) {
        const char* nombre = obtenerNombreLibro(inventario, aux->codigoLibro);
        
        printf("%d) Libro: %s | Nombre: %s | Cantidad: %d | Precio: %.2f | Subtotal: %.2f\n",
               num, aux->codigoLibro, nombre, aux->cantidad, aux->precio, aux->cantidad * aux->precio);
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
void mostrarInventarioPorAutor(Inventario* inv, const char* autor) {
    while (inv) {
        const char* autorActual = inv->libro.autor;
        toLowerCase((char*)autorActual);
        if (strcmp(autorActual, autor) == 0) {
            printf("%s | %s | %.2f | Stock: %d\n",
                   inv->libro.codigo, inv->libro.nombre, inv->libro.precio, inv->cantidad);
        }
        inv = inv->siguiente;
    }
    printf("\n");
}
void mostrarAutores(Inventario* inv) {
    if (!inv) {
        printf("Inventario vacío.\n");
        return;
    }

    printf("\n--- Autores Disponibles ---\n");
    Inventario* aux = inv;
    char autores[100][100];
    int numAutores = 0;

    while (aux) {
        int existe = 0;
        for (int i = 0; i < numAutores; i++) {
            if (strcmp(autores[i], aux->libro.autor) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe && numAutores < 100) {
            strncpy(autores[numAutores], aux->libro.autor, sizeof(autores[numAutores]) - 1);
            autores[numAutores][sizeof(autores[numAutores]) - 1] = '\0';
            numAutores++;
        }
        aux = aux->siguiente;
    }

    for (int i = 0; i < numAutores; i++) {
        printf("%d. %s\n", i + 1, autores[i]);
    }
}
void filtrarLibros(Inventario* inventario, Pedido* pedidoActual) {
     char autor[100];
    if (!inventario) {
        printf("Inventario vacío.\n");
        return;
    }
    if (!pedidoActual) {
        printf("No hay un pedido activo. Cree un pedido primero.\n");
        return;
    }
    limpiarPantalla();
    mostrarencabezado(leer_config());
    printf("\n--- Filtrar Libros por Autor ---\n");
   
    mostrarAutores(inventario);
    printf("Ingrese el autor para filtrar: ");
    scanf(" %99[^\n]", autor);
    toLowerCase(autor);
    limpiarPantalla();
    printf("\n--- Libros del autor: %s ---\n", autor);
    mostrarInventarioPorAutor(inventario, autor);
    int opcion;
    printf("1. Agregar libro al pedido\n0. Volver\nSeleccione opción: ");
    scanf("%d", &opcion);
    switch (opcion) {
    case 1:
        agregarLibro(inventario, pedidoActual);
        break;
    default:
        printf("Opción no válida.\n");
        break;
    }
}
int codigoValido(const char* code, int cantidad, float* precio, Inventario* inventario) {
    if (cantidad <= 0) {
        printf("Cantidad debe ser mayor que cero.\n");
        return 0;
    }   
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
        if (CantidadL <= 0) {
            limpiarPantalla();
            printf("Cantidad debe ser mayor que cero.\n");
            return;
        }   
        int cantidadTotal = 0; 
        LineaPedido* lp = pedidoActual->lineas;
        while (lp) {
            if (strcmp(lp->codigoLibro, CodigoL) == 0) {
                cantidadTotal += lp->cantidad;
            }
            lp = lp->siguiente;
        }
        if (codigoValido(CodigoL, CantidadL+cantidadTotal, &precio, inventario)  == 1) {
            agregarLineaAlPedido(pedidoActual, CodigoL, CantidadL, precio);
            limpiarPantalla();
            printf("Línea agregada al pedido.\n");
        } else {
            limpiarPantalla();
            printf("Código no existe o stock insuficiente.\n");
        }
    } else {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        limpiarPantalla();
        printf("Entrada inválida. Use el formato codigo,cantidad (ej: A123,5).\n");
    }
    
}


void eliminarLibro(Pedido* pedidoActual, Inventario* inventario) {
    if (!pedidoActual || !pedidoActual->lineas) {
        printf("No hay líneas.\n");
        return;
    }
        limpiarPantalla();
    printf("Líneas actuales:\n");
    verLineas(pedidoActual, inventario);
    //mostrarInventario(inventario);

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
            limpiarPantalla();
            printf("Línea %d eliminada.\n", linea);
            return;
        }
        prev = act;
        act = act->siguiente;
        cont++;
    }
    limpiarPantalla();
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
    fprintf(f, "  \"id\": %d,\n", pedido->id);
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
void generarPedido(Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas, Config** info) {
    limpiarPantalla();
    if (!*pedidoActual) return;

    if (!(*pedidoActual)->lineas) {
            printf("No se pueden procesar compras vacías\n");
            return;
        }

    float subtotal = 0;
    LineaPedido* aux = (*pedidoActual)->lineas;

    while (aux) {
        subtotal += aux->cantidad * aux->precio;
        cambiarCantidad(inventario, aux->codigoLibro, -aux->cantidad);
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
     int id = (*info)->libreria.siguiente_pedido++;
    (*pedidoActual)->id = id;
    (*pedidoActual)->subtotal = subtotal;
    (*pedidoActual)->total = subtotal * 1.13f;\
    cambiar(&((*info)->libreria.siguiente_pedido));
    mostrarencabezado(leer_config());
    printf("Fecha: %s | Cliente: %s\n", (*pedidoActual)->fecha, (*pedidoActual)->cedulaCliente);
    verLineas(*pedidoActual, inventario);
    printf("Subtotal: %.2f | Total: %.2f\n", (*pedidoActual)->subtotal, (*pedidoActual)->total);
    printf("Presione Enter para continuar...\n");
    getchar();
    getchar(); 

    guardarFactura(*pedidoActual);
    printf("Factura generada con éxito.\n");

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
        else if (strstr(linea, "\"id\"")) {
            int valor = 0;
            sscanf(linea, " \"id\": %d", &valor);
            pedido->id = valor;
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
void registrarPedido(Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas, Config** info) {
    char cedula[MAX_CEDULA];
    char fecha[20];
    limpiarPantalla();
    obtenerFechaActual(fecha, sizeof(fecha));
    printf("\n--- Registro de Pedido ---\n");
    mostrarencabezado(**info);
    printf("Ingrese cédula del cliente: ");
    scanf("%19s", cedula);
    limpiarPantalla();

    if (existe(cedula)) {
        *pedidoActual = construirPedido(cedula, fecha);
        int salir = 0, opcion;
       
        while (!salir) {
            

            mostrarencabezado(**info);
            printf("Fecha del pedido: %s |  Cliente: %s\n", fecha, cedula);
            verLineas(*pedidoActual, inventario);
            printf("\n--- Menú de Pedido ---\n");
            printf("1. Agregar libro\n2. Eliminar línea\n3. Generar pedido\n4. Filtrar libros\nSeleccione opción: ");
            scanf("%d", &opcion);

            switch(opcion){
                case 1: 
                limpiarPantalla();
                mostrarInventario(inventario);
                agregarLibro(inventario, *pedidoActual); break;
                case 2: eliminarLibro(*pedidoActual, inventario); break;
                case 3: generarPedido(inventario, pedidoActual, listaFacturas, info ); salir = 1; break;
                case 4: filtrarLibros(inventario, *pedidoActual); break;
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
        printf("\n--- Factura %d ---\n", p->id);
        printf("Cliente: %s\n", p->cedulaCliente);
        printf("Fecha: %s\n", p->fecha);
       

        LineaPedido* linea = p->lineas;
        int numLinea = 1;
        while (linea) {
            printf("  %d) Código: %s | Cantidad: %d | Precio: %.2f | Subtotal: %.2f\n",
                   numLinea, linea->codigoLibro, linea->cantidad, linea->precio, linea->cantidad * linea->precio);
            linea = linea->siguiente;
            numLinea++;
        }
        printf("\n");
        printf("Subtotal: %.2f | Total: %.2f\n", p->subtotal, p->total);

        p = p->siguiente;
        numPedido++;
    }
}


int comparar (char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}

void consultarCLiente(Pedido* listaFacturas){
    if (!listaFacturas) {
        printf("Ningun cliente ha facturado.\n");
        return;
    }
    printf("\n");
    printf("\n--- Consulta de Clientes y sus Facturas ---\n");
    int total = 0;
    Clientes *lista = LeerClientes(&total);
    for (int i = 0; i < total; i++) {
        printf("\n");
        printf("Cédula: %s  Nombre: %s Teléfono: %s\n", lista[i].Cedula, lista[i].Nombre, lista[i].Telefono);
        lista[i].Cedula;
        int vacio = 0;
        Pedido* factura = listaFacturas;
        while (factura) {
            if (comparar(factura->cedulaCliente, lista[i].Cedula)) {
                vacio = 1;
                int total = 0;
                LineaPedido* linea = factura->lineas;
                while (linea) {
                    total += linea->cantidad;
                    linea = linea->siguiente;
                }
                printf("  - Factura ID: %d | Fecha: %s | Cantidad de libros: %d | Subtotal: %.2f | Total: %.2f\n", factura->id, factura->fecha,total,factura->subtotal ,factura->total);
            }
            factura = factura->siguiente;   
        }
        if (vacio == 0) {
            printf("  - No ha realizado facturas.\n");
        }

    }
    free(lista); 
}



void EliminarCliente(Pedido* listaFacturas){

    int total = 0;
    Clientes *lista = LeerClientes(&total);

    if (total == 0 ){
        printf("No hay clientes registrados.\n");
        return;
    }
    printf("\n--- Eliminar Cliente ---\n");

    for (int i = 0; i < total; i++) {
        printf("%d) Cédula: %s  Nombre: %s Teléfono: %s\n", i+1, lista[i].Cedula, lista[i].Nombre, lista[i].Telefono);
    }
    printf("Seleccione un numero del 1 al %d: ", total);
    int opcion;
    scanf("%d", &opcion);
    printf("\n");

    if (opcion < 1 || opcion > total) {
        printf("Opción inválida.\n");
        free(lista);
        return;
    }
    if (!listaFacturas) {
        BorrarCliente(lista[opcion-1].Cedula);
        printf("Cliente eliminado con exito.\n");
        return;
    }
    Pedido* factura = listaFacturas;
    int encontro = 0;
    while (factura) {
            if (comparar(factura->cedulaCliente, lista[opcion-1].Cedula)) {
                encontro = 1;
                printf("Cliente no se pudo eliminar debido a que ha facturado.\n");
                return;
            }
            factura = factura->siguiente;   
        }
    BorrarCliente(lista[opcion-1].Cedula);
    printf("Cliente eliminado con exito.\n");
    free(lista);
}



#endif // PEDIDO_H
