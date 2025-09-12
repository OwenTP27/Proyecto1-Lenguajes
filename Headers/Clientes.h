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
    FILE *file = fopen("../Data/Clientes.txt", "r");

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
    FILE *filePointer = fopen("../Data/Clientes.txt", "a"); 

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
        getchar(); 

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


#endif





