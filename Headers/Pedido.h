#ifndef PEDIDO_H
#define PEDIDO_H
#include <stdio.h>
#include <string.h>
void registrarPedido();
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

#endif

static void RegistrarPedido() {
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- Opciones Administrativas ---\n");
		printf("1.Consulta de catálogo\n");
		printf("2. Consulta de cliente\n");
		printf("0. Volver\n");
		printf("Seleccione una opción: ");
		scanf_s("%d", &opcion);

		switch (opcion) {
		case 1:

			break;
		case 2:

			break;
		case 0:
			salir = 1;
			break;
		default:
			printf("Opción no válida.\n");
		}
	}
}
