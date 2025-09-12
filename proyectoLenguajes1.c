#include <stdio.h>
#include "Headers/Inventario.h"
#include "Headers/Menus.h"

int main()
{
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- MEN� PRINCIPAL ---\n");
		printf("1. Men� General\n");
		printf("2. Men� Administrativo\n");
		printf("3. Salir\n");
		printf("Seleccione una opci�n: ");
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
			printf("Opci�n no v�lida.\n");
		}
	}
	return 0;
}