#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CLIENTES 100 
#define MAX_NOMBRE 50
#define MAX_CEDULA 10
#define MAX_TELEFONO 15
#define MIN_TELEFONO 8
#define Archivotxt "Data/Clientes.txt"


typedef struct {
    char Cedula [MAX_CEDULA]; 
    char Nombre [MAX_NOMBRE];
    char Telefono[MAX_TELEFONO];
} Clientes;

/*
  Nombre: existe
  Entradas: char *vcedula puntero a un numero de cedula
  Salida: 0 si no existe y 1 si existe 
  Descripcion: verifica en el txt si existe algun usuario con la misma cedula
*/

int existe(char *vcedula){
    FILE *file = fopen(Archivotxt, "r");
    char linea[200];
    char cedula[MAX_CEDULA];
    while (fgets(linea, sizeof(linea), file)) {
        char *token = strtok(linea, ",");
        if (token) {
            strncpy(cedula, token, MAX_CEDULA);
            cedula[MAX_CEDULA-1] = '\0'; 
            if (strcmp(cedula, vcedula) == 0) {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}


/*
  Nombre:validarnumero
  Entradas:char *numero, puntero a un numero 
  Salida: o si el numero cumple los riquisitos 1 si no
  Descripcion: valiada que el numero telefonico no tenga numero y tenga mas de 8 dijitos
*/
int validarnumero(char *numero) {
    int validar = 0;
    int largo = 0;
    for (int i = 0; numero[i] != '\0'; i++) {
        largo ++;
        if (((int)numero[i]) < 48 || ((int)numero[i]) > 57 ){
            printf("El contenido del numero telefonico deben ser numerico \n");
            return 1;
        }
    }
    if (largo < MIN_TELEFONO ){
        printf("El numero telefonico debe de ser minimo de 8 dijitos \n");
        return 1;
    }
    else{
        return 0;

    }
}


/*
  Nombre:AgregarCliente
  Entradas:Clientes *Cliente, puntero al struct de clientes 
  Salida: ninguna
  Descripcion: agrega el cliente al txt
*/

void AgregarCliente(Clientes *Cliente) {
    FILE *filePointer = fopen(Archivotxt, "a"); 

    fprintf(filePointer, "%s,%s,%s\n",
        Cliente->Cedula,
        Cliente->Nombre,
        Cliente->Telefono);
    fclose(filePointer);
   
}


/*
  Nombre:SolicitarCliente
  Entradas:ninguna
  Salida: un struct 
  Descripcion: solicita los datos necesarios para la creacion de nuevo cliente 
*/


Clientes SolicitarCliente (){
    Clientes cliente;
    int valida = 0; 
    char pregunta[10];

    while (valida == 0) {
        printf("\nIngrese el número de cédula: ");
        scanf("%9s", cliente.Cedula);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        

        if (!existe(cliente.Cedula)) {
            printf("Ingrese el nombre: ");
            fgets(cliente.Nombre, MAX_NOMBRE, stdin);
            cliente.Nombre[strcspn(cliente.Nombre, "\n")] = '\0'; 
            valida=1;
            while (valida != 0){
                printf("Ingrese el teléfono: ");
                scanf("%14s", cliente.Telefono);
                printf("\n");
                valida = validarnumero(cliente.Telefono);
            }

            printf("\nCliente registrado con éxito.\n");
            valida = 1;
            AgregarCliente(&cliente);
        }
        else {
            printf("Cédula existente\n");
            printf("intente de nuevo\n");
        }
    }

    return cliente;
}
/*
  Nombre: LeerClientes
  Entradas: int *total (salida con la cantidad de clientes)
  Salida: puntero a un arreglo dinámico de Clientes
  Descripción: lee el txt y devuelve todos los clientes en memoria dinámica
*/
Clientes* LeerClientes(int *total) {
    FILE *file = fopen(Archivotxt, "r");
    char linea[200];
    int count = 0;

    while (fgets(linea, sizeof(linea), file)) {
        count++;
    }
    rewind(file); 
    Clientes *clientes = malloc(count * sizeof(Clientes));
    int largo = 0;
    while (fgets(linea, sizeof(linea), file) && largo < count) {
        Clientes cliente;
        int i = 0, j = 0;
        j = 0;
        while (linea[i] != ',') {
            cliente.Cedula[j++] = linea[i++];
        }
        cliente.Cedula[j] = '\0';
        if (linea[i] == ',') i++;
        j = 0;
        while (linea[i] != ',') {
            cliente.Nombre[j++] = linea[i++];
        }
        cliente.Nombre[j] = '\0';
        if (linea[i] == ',') i++;
        j = 0;
        while (linea[i] != '\0' && linea[i] != '\n') {
            cliente.Telefono[j++] = linea[i++];
        }
        cliente.Telefono[j] = '\0';
        clientes[largo++] = cliente;
    }

    fclose(file);
    *total = largo; 
    return clientes;
}

int compararcedula (char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}


void BorrarCliente(char *cedula) {
    FILE *file = fopen(Archivotxt, "r");
    char lineas[100][200]; 
    int totalLineas = 0;
    int encontrado = 0;
    char linea[200];
    while (fgets(linea, sizeof(linea), file) && totalLineas < 100) {
        char cedulaArchivo[MAX_CEDULA];
        int i = 0, j = 0;
        while (linea[i] != ',') {
            cedulaArchivo[j++] = linea[i++];
        }
        cedulaArchivo[j] = '\0';
        if (compararcedula(cedulaArchivo, cedula)) {
            continue;
        }
        strcpy(lineas[totalLineas++], linea); 
    }
    fclose(file);
    file = fopen(Archivotxt, "w");
    for (int i = 0; i < totalLineas; i++) {
        fputs(lineas[i], file);
    }
    fclose(file);

}


#endif