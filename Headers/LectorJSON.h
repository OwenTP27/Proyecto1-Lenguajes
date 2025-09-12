#ifndef LECTORJSON_H
#define LECTORJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Archivo "Data/DatosIniciales.json"

typedef struct {
    char *nombre;
    char *telefono;
    char *cedula_juridica;
    char *horario;
    int siguiente_pedido;
} Libreria;

typedef struct {
    char *usuario;
    char *contrasena;
} Admin;

typedef struct {
    Libreria libreria;
    Admin admin;
} Config;

static Config leer_config() {
    Config cfg;
    FILE *f = fopen(Archivo, "r");
    char line[200];

    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"nombre\""))
            sscanf(line, " \"nombre\": \"%[^\"]\"", cfg.libreria.nombre);
        else if (strstr(line, "\"telefono\""))
            sscanf(line, " \"telefono\": \"%[^\"]\"", cfg.libreria.telefono);
        else if (strstr(line, "\"cedula_juridica\""))
            sscanf(line, " \"cedula_juridica\": \"%[^\"]\"", cfg.libreria.cedula_juridica);
        else if (strstr(line, "\"horario\""))
            sscanf(line, " \"horario\": \"%[^\"]\"", cfg.libreria.horario);
        else if (strstr(line, "\"siguiente_pedido\""))
            sscanf(line, " \"siguiente_pedido\": %d", &cfg.libreria.siguiente_pedido);
        else if (strstr(line, "\"usuario\""))
            sscanf(line, " \"usuario\": \"%[^\"]\"", cfg.admin.usuario);
        else if (strstr(line, "\"contrasena\""))
            sscanf(line, " \"contrasena\": \"%[^\"]\"", cfg.admin.contrasena);
    }

    fclose(f);
    return cfg;
}


static void cambiar(int *nuevo) {
    FILE *f = fopen(Archivo, "r");
    char line[200];
    char buffer[5000] = "";  
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"siguiente_pedido\"")) {
            char nueva_linea[100];
            sprintf(nueva_linea, "    \"siguiente_pedido\": %d,\n", *nuevo);
            strcat(buffer, nueva_linea);
        } else {
            strcat(buffer, line);
        }
    }
    fclose(f);
    f = fopen(Archivo, "w");
    fputs(buffer, f);
    fclose(f);
}

#endif
