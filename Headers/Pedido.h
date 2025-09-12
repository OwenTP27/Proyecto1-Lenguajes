#ifndef PEDIDO_H
#define PEDIDO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h> //validadr si es valida
#include <time.h>
#include "Inventario.h"
#include "Clientes.h"
#define ARCHIVO_FACTURAS "Data/Facturas.txt"
#define _CRT_SECURE_NO_WARNINGS

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
} Pedido;

typedef struct NodoPedido {
	Pedido* pedido;
	struct NodoPedido* siguiente;
} NodoPedido;

Inventario* inv = NULL;
Pedido* pedidoActual = NULL;
NodoPedido* listaFacturas = NULL;
// Construcción de línea de pedido
LineaPedido* construirLineaPedido(char* codigoLibro, float precio, int cantidad) {
	LineaPedido* nueva = (LineaPedido*)malloc(sizeof(LineaPedido));
	if (!nueva) { printf("Error al asignar memoria\n"); exit(1); }
	strcpy_s(nueva->codigoLibro, sizeof(nueva->codigoLibro), codigoLibro);
	nueva->cantidad = cantidad;
	nueva->precio = precio;
	nueva->siguiente = NULL;
	return nueva;
}

// Construcción de pedido
Pedido* construirPedido(char* cedulaCliente, char* fecha) {
	Pedido* p = (Pedido*)malloc(sizeof(Pedido));
	if (!p) { printf("Error al asignar memoria\n"); exit(1); }
	strcpy_s(p->cedulaCliente, sizeof(p->cedulaCliente), cedulaCliente);
	strcpy_s(p->fecha, sizeof(p->fecha), fecha);
	p->lineas = NULL;
	p->numLineas = 0;
	p->subtotal = 0;
	p->total = 0;
	return p;
}
void agregarLibro(void);
void eliminarLibro(void);
bool codigoValido(const char* code, int cantidad, float* precio);
void agregarLineaAlPedido(Pedido* pedido, const char* codigo, int cantidad, float precio);

// Liberar memoria de líneas
void liberarLineas(LineaPedido* linea) {
	while (linea) {
		LineaPedido* tmp = linea;
		linea = linea->siguiente;
		free(tmp);
	}
}

// Liberar memoria de un pedido completo
void liberarPedido(Pedido* pedido) {
	if (!pedido) return;
	liberarLineas(pedido->lineas);
	free(pedido);
}

// Mostrar carrito
void verLineas(Pedido* p) {
	LineaPedido* aux = p->lineas;
	if (!aux) { printf("\n---- Carrito vacio ----\n"); return; }

	printf("\n---- Carrito actual ----\n");
	int num = 1;
	while (aux) {
		printf("%d) Libro: %s | Cantidad: %d | Precio: %.2f | Subtotal: %.2f\n",
			num, aux->codigoLibro, aux->cantidad, aux->precio, aux->cantidad * aux->precio);
		aux = aux->siguiente;
		num++;
	}
}

void obtenerFechaActual(char* buffer, int tam) {
	time_t t = time(NULL);            // obtiene tiempo actual
	struct tm tm = *localtime(&t);    // lo convierte a fecha local

	// Formato dd/mm/aaaa
	snprintf(buffer, tam, "%02d/%02d/%04d",
		tm.tm_mday,
		tm.tm_mon + 1,   // meses van de 0 a 11
		tm.tm_year + 1900); // años desde 1900
}
/*Se debe permitir la selección de los elementos del catálogo y colocarlos en una lista visible al
cliente (código, nombre y precio),

se debe poder filtrar por autor.

Se debe permitir elegir varios

libros e indicar la cantidad (detalle).

Además, se puede seleccionar algún libro agregado al pedido y removerlo de la misma.

El cliente puede elegir la cantidad de libros que desee. Si al agregar un nuevo libro al pedido éste
ya existe, lo que se hace es que se aumenta la cantidad.

Se debe validar que exista la cantidad de
libros en stock, si no, no se agrega la línea y se informa al usuario (informar al usuario el stock
existente).

La información debe incluir la lista de libros, con las cantidades y precios.

Después de agregar cada línea se debe preguntar si desea agregar línea, eliminar línea (elimina por
número de línea), generar o salir sin guardar. Se deben mostrar las líneas del pedido con su
número.

Cuando se genera el pedido se solicita la cédula del cliente y fecha, se deben descontar de stock la
cantidad facturada de cada libro. Y mostrar todo el detalle del pedido con el encabezado
(información del comercio y cliente) con el identificador generado. Se debe hacer el cálculo del
impuesto de venta del pedido.*/

static void registrarPedido() {
	int opcion, salir = 0;
	char cedula[MAX_CEDULA];
	char fecha[20];
	obtenerFechaActual(fecha, sizeof(fecha));

	printf("fecha del pedido : %s\n", fecha);
	printf("Ingrese cédula del cliente: ");
	scanf_s("%s", cedula, (unsigned)_countof(cedula));
	if (existe(cedula)) {
		pedidoActual = construirPedido(cedula, fecha);
		while (!salir) {
			printf("\n--- Menú de Pedido ---\n");
			mostrarInventario(inv);//muestra el inventario antes que el menu
			verLineas(pedidoActual);
			//owen restar catalogo

			printf("1. Agregar libro\n");
			printf("2. Eliminar línea del pedido\n");
			printf("3. Generar pedido\n");
			printf("4.Filtrar por autor\n");

			printf("0. Salir\n");
			printf("Seleccione una opción: ");
			scanf_s("%d", &opcion);

			switch (opcion) {
			case 1: agregarLibro(); break;      // Agregar libro
			case 2: eliminarLibro(); break;     // Eliminar línea
				//case 3: generarPedido(); break;     // Generar pedido (descontar stock, guardar factura)
				//case 4: filtrarPorAutor(); break;   // Opcional: filtrar inventario
			case 0: salir = 1; break;
			default: printf("Opción no válida.\n"); break;
			}
		}
	}
	else
	{
		printf("no existe el cliente.\n");
	}
}

void agregarLibro() {
	int CantidadL;
	char CodigoL[100];
	float precio;

	printf("Ingrese código y cantidad separados por coma (ej: A123,5): ");

	if (scanf_s(" %99[^,],%d", CodigoL, (unsigned)_countof(CodigoL), &CantidadL) == 2) {
		if (codigoValido(CodigoL, CantidadL, &precio)) {
			printf(" El código %s existe en el inventario.\n", CodigoL);
			printf("Cantidad solicitada: %d | Precio unitario: %.2f\n", CantidadL, precio);

			if (pedidoActual) {
				agregarLineaAlPedido(pedidoActual, CodigoL, CantidadL, precio);
				printf(" Línea agregada al pedido.\n");
			}
			else {
				printf(" No hay un pedido activo.\n");
			}
		}
		else {
			printf(" Código no existe o stock insuficiente.\n");
		}
	}
	else {
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		printf(" Entrada inválida. Use el formato codigo,cantidad (ej: A123,5).\n");
	}
}
// Validar código y cantidad
bool codigoValido(const char* code, int cantidad, float* precio) {
	Inventario* act = inv;
	while (act) {
		if (strcmp(act->libro.codigo, code) == 0) {
			if (cantidad <= act->cantidad) { *precio = act->libro.precio; return true; }
			else { printf(" Stock insuficiente: %d\n", act->cantidad); return false; }
		}
		act = act->siguiente;
	}
	return false;
}

// Agregar línea al pedido
void agregarLineaAlPedido(Pedido* pedido, const char* codigo, int cantidad, float precio) {
	if (!pedido) return;
	LineaPedido* act = pedido->lineas;
	LineaPedido* prev = NULL;
	while (act) {
		if (strcmp(act->codigoLibro, codigo) == 0) {
			act->cantidad += cantidad;
			return;
		}
		prev = act;
		act = act->siguiente;
	}
	LineaPedido* nueva = construirLineaPedido((char*)codigo, precio, cantidad);
	if (!prev) pedido->lineas = nueva;
	else prev->siguiente = nueva;
	pedido->numLineas++;
}// Eliminar línea del pedido
void eliminarLibro() {
	if (!pedidoActual || !pedidoActual->lineas) { printf(" No hay líneas.\n"); return; }
	int linea;
	printf("Ingrese número de línea a eliminar: ");
	if (scanf_s("%d", &linea) != 1) { while (getchar() != '\n'); printf("Entrada inválida.\n"); return; }

	LineaPedido* act = pedidoActual->lineas;
	LineaPedido* prev = NULL;
	int cont = 1;
	bool encontrado = false;
	while (act) {
		if (cont == linea) {
			encontrado = true;
			if (!prev) pedidoActual->lineas = act->siguiente;
			else prev->siguiente = act->siguiente;
			free(act);
			pedidoActual->numLineas--;
			printf("Línea %d eliminada.\n", linea);
			break;
		}
		prev = act;
		act = act->siguiente;
		cont++;
	}
	if (!encontrado) printf("Línea %d no encontrada.\n", linea);
}

// Guardar factura
void guardarFactura(Pedido* pedido) {
	if (!pedido) return;
	FILE* f = fopen(ARCHIVO_FACTURAS, "a");
	if (!f) { printf("Error al abrir archivo\n"); return; }

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
	fclose(f);
}

// Generar pedido
void generarPedido() {
	if (!pedidoActual) return;
	float subtotal = 0;
	LineaPedido* aux = pedidoActual->lineas;

	// Calcular subtotal y descontar stock
	while (aux) {
		subtotal += aux->cantidad * aux->precio;

		Inventario* libro = inv;
		while (libro) {
			if (strcmp(libro->libro.codigo, aux->codigoLibro) == 0) {
				libro->cantidad -= aux->cantidad;
				break;
			}
			libro = libro->siguiente;
		}

		aux = aux->siguiente;
	}

	pedidoActual->subtotal = subtotal;
	pedidoActual->total = subtotal * 1.13f;

	printf("Subtotal: %.2f | Total: %.2f\n", pedidoActual->subtotal, pedidoActual->total);

	// Guardar en archivo
	guardarFactura(pedidoActual);

	NodoPedido* nodo = (NodoPedido*)malloc(sizeof(NodoPedido));
	nodo->pedido = pedidoActual;
	nodo->siguiente = listaFacturas;
	listaFacturas = nodo;

	pedidoActual = NULL;
}

#endif