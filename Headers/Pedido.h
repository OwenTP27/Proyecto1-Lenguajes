#ifndef PEDIDO_H
#define PEDIDO_H
#include <stdio.h>
#include <string.h>
void registrarPedido();
/*Se debe permitir la selecci�n de los elementos del cat�logo y colocarlos en una lista visible al
cliente (c�digo, nombre y precio),

se debe poder filtrar por autor.

Se debe permitir elegir varios

libros e indicar la cantidad (detalle).

Adem�s, se puede seleccionar alg�n libro agregado al pedido y removerlo de la misma.

El cliente puede elegir la cantidad de libros que desee. Si al agregar un nuevo libro al pedido �ste
ya existe, lo que se hace es que se aumenta la cantidad.

Se debe validar que exista la cantidad de
libros en stock, si no, no se agrega la l�nea y se informa al usuario (informar al usuario el stock
existente).

La informaci�n debe incluir la lista de libros, con las cantidades y precios.

Despu�s de agregar cada l�nea se debe preguntar si desea agregar l�nea, eliminar l�nea (elimina por
n�mero de l�nea), generar o salir sin guardar. Se deben mostrar las l�neas del pedido con su
n�mero.

Cuando se genera el pedido se solicita la c�dula del cliente y fecha, se deben descontar de stock la
cantidad facturada de cada libro. Y mostrar todo el detalle del pedido con el encabezado
(informaci�n del comercio y cliente) con el identificador generado. Se debe hacer el c�lculo del
impuesto de venta del pedido.*/

#endif

static void RegistrarPedido() {
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- Opciones Administrativas ---\n");
		printf("1.Consulta de cat�logo\n");
		printf("2. Consulta de cliente\n");
		printf("0. Volver\n");
		printf("Seleccione una opci�n: ");
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
			printf("Opci�n no v�lida.\n");
		}
	}
}
