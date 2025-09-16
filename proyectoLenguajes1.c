
#include <stdio.h>
#include "Headers/Inventario.h"
#include "Headers/Menus.h"
#include "Headers/LectorJSON.h"
#define _CRT_SECURE_NO_WARNINGS

int main()
{ 	limpiarPantalla();
	Inventario* inventario = NULL;
	Pedido* pedidoActual = NULL;
	Pedido* listaFacturas = NULL;
	cargarInventario(&inventario);
	cargarFacturas(&listaFacturas);
	Config info = leer_config();
	//imprimirFacturas(listaFacturas);
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
			menuGeneral(pedidoActual, &listaFacturas, inventario);
			break;
		case 2:
			if (login(&info)) {
				menuAdministrativo(inventario,&pedidoActual, &listaFacturas,&info);
			}
			else {
				printf("Acceso denegado.\n");
			}
			break;
		case 3:
			printf("Saliendo del programa...\n");
			liberarFacturas(listaFacturas);
			liberarPedido(pedidoActual);
		
			salir = 1;
			
			break;
		default:
			
			printf("Opción no válida.\n");
		}
	}
	return 0;
}