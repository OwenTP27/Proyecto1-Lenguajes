#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <string.h>
#include "Pedido.h"
#include "Inventario.h"
#include "Clientes.h"

// ---------------- MENÚ GENERAL ----------------
static void menuGeneral(Pedido* pedidoActual, Pedido** listaFacturas) {
   
    int opcion, salir = 0;
    while (!salir) {
         limpiarPantalla();
    mostrarencabezado(leer_config());
        printf("\n--- Opciones Administrativas ---\n");
        printf("1. Consulta de catálogo\n");
        printf("2. Consulta de cliente\n");
        printf("0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

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

// ---------------- MENÚ ADMINISTRATIVO ----------------
static void menuAdministrativo(Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas, Config* info) {
    
    int opcion, salir = 0;
    while (!salir) {
         limpiarPantalla();
    mostrarencabezado(leer_config());
        printf("\n--- Menú Administrativo ---\n");
        printf("1. Registrar libros\n");
        printf("2. Manejo de inventario\n");
        printf("3. Registrar clientes\n");
        printf("4. Crear pedido\n");
        printf("5. Estadísticas\n");
        printf("0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:

            break;
       
            break;
        case 3: {
            Clientes c = SolicitarCliente();
                printf("Cliente %s registrado correctamente.\n", c.Nombre);
            } break;
        case 4:
            registrarPedido(inventario, pedidoActual, listaFacturas, &info);
            break;
        case 5:

            break;
        case 0:
            salir = 1;
            break;          
        default:
            printf("Opción no válida.\n");
        }
    }
}

static int login(Config* info) {
    char usuario[50], contrasena[50];
    printf("Usuario: ");
    scanf("%49s", usuario);
    printf("Contraseña: ");
    scanf("%49s", contrasena);

    if (strcmp(usuario, info->admin.usuario) == 0 && strcmp(contrasena, info->admin.contrasena) == 0) {
        printf("Login exitoso.\n");
        limpiarPantalla();
        return 1;
    }
    else {
        printf("Usuario o contraseña incorrectos.\n");
    }
    limpiarPantalla();
    return 0;
}

#endif  
