#ifndef MENUS_H
#define MENUS_H
#include <stdio.h>
#include <string.h>
void menuGeneral(void);
void menuAdministrativo(void);
int login(void);

#endif
static void menuGeneral(void) {
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

static void menuAdministrativo(void) {
	int opcion, salir = 0;
	while (!salir) {
		printf("\n--- Opciones Administrativas ---\n");
		printf("1. Registrar libros\n");
		printf("2. Manejo de inventario\n");
		printf("3. Registrar clientes\n");
		printf("4. Crear pedido\n");
		printf("5. Estadísticas\n");
		printf("0. Volver\n");
		printf("Seleccione una opción: ");
		scanf_s("%d", &opcion);

		switch (opcion) {
		case 1:

			break;
		case 2:
			if (login()) {
				menuAdministrativo();
			}
			else {
				printf("Acceso denegado.\n");
			}
			break;
		case 0:
			salir = 1;

		default:
			printf("Opción no válida.\n");
		}
	}
}

static int login(void) {
	char usuario[50], contrasena[50];
	printf("Usuario: ");
	scanf_s("%49s", usuario, (unsigned)sizeof(usuario));
	printf("Contraseña: ");
	scanf_s("%49s", contrasena, (unsigned)sizeof(contrasena));
	if (strcmp(usuario, "admin") == 0 && strcmp(contrasena, "1234") == 0) {
		return 1;
	}
	return 0;
}