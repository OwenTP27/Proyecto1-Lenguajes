#ifndef LECTORJSON_H
#define LECTORJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#define CONFIG_FILE "../Data/DatosIniciales.json"

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
    FILE *f = fopen(CONFIG_FILE, "r");
    fseek(f, 0, SEEK_END);
    long largo = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = malloc(largo + 1);
    fread(data, 1, largo, f);
    data[largo] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    free(data);


    Config cfg;
    cJSON *libreria = cJSON_GetObjectItem(json, "libreria");
    cJSON *admin = cJSON_GetObjectItem(json, "admin");
    cfg.libreria.nombre = strdup(cJSON_GetObjectItem(libreria, "nombre")->valuestring);
    cfg.libreria.telefono = strdup(cJSON_GetObjectItem(libreria, "telefono")->valuestring);
    cfg.libreria.cedula_juridica = strdup(cJSON_GetObjectItem(libreria, "cedula_juridica")->valuestring);
    cfg.libreria.horario = strdup(cJSON_GetObjectItem(libreria, "horario")->valuestring);
    cfg.libreria.siguiente_pedido = cJSON_GetObjectItem(libreria, "siguiente_pedido")->valueint;
    cfg.admin.usuario = strdup(cJSON_GetObjectItem(admin, "usuario")->valuestring);
    cfg.admin.contrasena = strdup(cJSON_GetObjectItem(admin, "contrasena")->valuestring);
    cJSON_Delete(json);
    return cfg;
}


static void cambiar(int *nuevo) {
    FILE *f = fopen(CONFIG_FILE, "r");

    fseek(f, 0, SEEK_END);
    long largo = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(largo + 1);
    fread(data, 1, largo, f);
    data[largo] = '\0';
    fclose(f);
    cJSON *json = cJSON_Parse(data);
    free(data);
    cJSON *libreria = cJSON_GetObjectItem(json, "libreria");
    cJSON_ReplaceItemInObject(libreria, "siguiente_pedido", cJSON_CreateNumber(*nuevo));
    char *nuevo_json = cJSON_Print(json);
    cJSON_Delete(json);

    f = fopen(CONFIG_FILE, "w");
    fprintf(f, "%s", nuevo_json);
    fclose(f);
    free(nuevo_json);
}

#endif
