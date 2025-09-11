#include <stdio.h>
#include "Headers/Inventario.h"

#include "Headers/Menus.h"
#define _CRT_SECURE_NO_WARNINGS

int main()
{
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- MENÚ PRINCIPAL ---\n");
		printf("1. Menú General\n");
		printf("2. Menú Administrativo\n");
		printf("3. Salir\n");
		printf("Seleccione una opción: ");
		scanf_s("%d", &opcion);

		switch (opcion) {
		case 1:
			menuGeneral();
			break;
		case 2:
			if (login()) {
				menuAdministrativo();
			}
			else {
				printf("Acceso denegado.\n");
			}
			break;
		case 3:
			salir = 1;
			break;
		default:
			printf("Opción no válida.\n");
		}
	}
	return 0;
}