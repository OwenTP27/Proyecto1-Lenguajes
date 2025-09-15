#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include <string.h>
#include "Pedido.h"
#include "Inventario.h"
#include "Clientes.h"

// ---------------- MENÚ GENERAL ----------------
/*
  Nombre: menuGeneral
  Entradas: Pedido* pedidoActual, Pedido** listaFacturas
  Salida:  Ninguna
  Descripcion: Muestra el menú general de la aplicación.
*/
static void menuGeneral(Pedido* pedidoActual, Pedido** listaFacturas) {
    
    int opcion, salir = 0;
    while (!salir) {
        limpiarPantalla();
        mostrarencabezado(leer_config());
        printf("\n--- Opciones Administrativas ---\n");
        printf("1. Consulta de catálogo\n");
        printf("2. Consulta de cliente\n");
        printf("3. Consulta de facturas\n");
        printf("0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:

            break;
        case 2:
            consultarCLiente(*listaFacturas);
            break;
        case 3: imprimirFacturas(*listaFacturas); break;
        case 0:
            salir = 1;
            break;
        default:
			printf("Opción no válida.\n");
        }
    }
}

// ---------------- MENÚ ADMINISTRATIVO ----------------
/*
  Nombre: menuAdministrativo
  Entradas: Inventario* inventario, Pedido** pedidoActual, Pedido** listaFacturas, Config* info
  Salida:  Ninguna
  Descripcion: Muestra el menú administrativo de la aplicación.
*/
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
            menuEstadisticas(*listaFacturas);
            break;
        case 0:
            salir = 1;
            break;          
        default:
            printf("Opción no válida.\n");
        }
    }
}
/*
  Nombre: menuEstadisticas
  Entradas: Pedido* listaFacturas
  Salida:  Ninguna
  Descripcion: Muestra el menú de estadísticas de la aplicación.
*/
static void menuEstadisticas(Pedido* listaFacturas) {
    int opcion, salir = 0;
    while (!salir) {
        limpiarPantalla();
        mostrarencabezado(leer_config());
        printf("\n--- Estadísticas ---\n");
        printf("1. Total de ventas por mes y año\n");
        printf("2. Libro más vendido\n");
        printf("3. Cliente con más compras\n"); 
        printf("4. Autor con mas ventas\n");
        printf("0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:
            totalventasMesAno(listaFacturas);
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 0:
            salir = 1;
            break;
        default:
            printf("Opción no válida.\n");
        }
    }
}
/*
  Nombre: MenuExtras
  Entradas: Pedido* pedidoActual, Pedido** listaFacturas, Inventario* inventario
  Salida:  Ninguna
  Descripcion: Muestra el menú de opciones extras de la aplicación.
*/
static void MenuExtras(Pedido* pedidoActual, Pedido** listaFacturas, Inventario* inventario) {

    int opcion, salir = 0;
    while (!salir) {
        limpiarPantalla();
        mostrarencabezado(leer_config());
        printf("\n--- Opciones Administrativas ---\n");
        printf(" 1.Eliminar libro:\n");
        printf(" 2. Eliminar cliente\n");
        printf(" 3. Eliminar pedido\n");
        printf(" 4. Modificar pedido\n");
        printf(" 0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:

            break;
        case 2:
            
            break;
        case 3: 
                eliminarPedido(listaFacturas, inventario);
            break;
        case 4:  
        modificarPedido(*listaFacturas, inventario);   
            break;
            case 5:
            totalventasMesAno(*listaFacturas);
            break;
        case 0:
            salir = 1;
            break;
        default:
			printf("Opción no válida.\n");
        }
    }
}
/*
  Nombre: login
  Entradas: Config* info
  Salida:  int (1 si login exitoso, 0 si falla)
  Descripcion: Maneja el proceso de login para el acceso administrativo.
*/
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
    printf("Presione Enter para continuar...\n");
    getchar();
    getchar();
    limpiarPantalla();
    return 0;
}

#endif  
