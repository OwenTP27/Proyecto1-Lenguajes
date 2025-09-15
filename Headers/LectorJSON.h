#ifndef LECTORJSON_H
#define LECTORJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Archivo "Data/DatosIniciales.json"

typedef struct Libreria {
    char *nombre;
    char *telefono;
    char *cedula_juridica;
    char *horario;
    int siguiente_pedido;
} Libreria;

typedef struct Admin {
    char *usuario;
    char *contrasena;
} Admin;

typedef struct Config {
    Libreria libreria;
    Admin admin;
} Config;

static Config leer_config() {
    Config cfg;

    
    cfg.libreria.nombre = malloc(100);
    cfg.libreria.telefono = malloc(20);
    cfg.libreria.cedula_juridica = malloc(20);
    cfg.libreria.horario = malloc(100);
    cfg.admin.usuario = malloc(50);
    cfg.admin.contrasena = malloc(50);

    if (!cfg.libreria.nombre || !cfg.libreria.telefono || !cfg.libreria.cedula_juridica ||
        !cfg.libreria.horario || !cfg.admin.usuario || !cfg.admin.contrasena) {
        fprintf(stderr, "Error al reservar memoria\n");
        exit(1);
    }

    FILE *f = fopen(Archivo, "r");
    if (!f) {
        perror("No se pudo abrir el archivo");
        exit(1);
    }

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
    if (!f) {
        perror("No se pudo abrir el archivo para leer");
        exit(1);
    }
    //printf("Siguiente pedido actualizado a: %d\n", *nuevo);
    char line[200];
    char buffer[5000] = "";
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"siguiente_pedido\"")) {
            char nueva_linea[100];
            sprintf(nueva_linea, "    \"siguiente_pedido\": %d\n", *nuevo);
            strcat(buffer, nueva_linea);
        } else {
            strcat(buffer, line);
        }
    }
    fclose(f);

    f = fopen(Archivo, "w");
    if (!f) {
        perror("No se pudo abrir el archivo para escribir");
        exit(1);
    }
    fputs(buffer, f);
    fclose(f);
}

// Liberar memoria
static void liberar_config(Config *cfg) {
    free(cfg->libreria.nombre);
    free(cfg->libreria.telefono);
    free(cfg->libreria.cedula_juridica);
    free(cfg->libreria.horario);
    free(cfg->admin.usuario);
    free(cfg->admin.contrasena);
}

#endif
