#include <stdio.h>
#include "Headers/Inventario.h"
#include "Headers/Menus.h"
#define _CRT_SECURE_NO_WARNINGS

int main()
{ 	
	Inventario* inventario = NULL;
	Pedido* pedidoActual = NULL;
	Pedido* listaFacturas = NULL;
	cargarInventario(&inventario);
	printf("DEBUG: listaFacturas=%p\n", (void*)listaFacturas);
	cargarFacturas(&listaFacturas);
	
	imprimirFacturas(listaFacturas);
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- MENÚ PRINCIPAL ---\n");
		printf("1. Menú General\n");
		printf("2. Menú Administrativo\n");
		printf("3. Salir\n");
		printf("Seleccione una opción: ");
		scanf("%d", &opcion);

		switch (opcion) {
		case 1:
			menuGeneral();
			break;
		case 2:
			if (login()) {
				menuAdministrativo(inventario,&pedidoActual, &listaFacturas);
			}
			else {
				printf("Acceso denegado.\n");
			}
			break;
		case 3:
			salir = 1;
			     if (pedidoActual) liberarPedido(pedidoActual);
                liberarFacturas(listaFacturas);
			break;
		default:
			printf("Opci�n no v�lida.\n");
		}
	}
	return 0;
}