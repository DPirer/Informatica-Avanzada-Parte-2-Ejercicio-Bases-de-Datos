#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

// VARIABLES GLOBALES
sqlite3 *db;
int res;
char *error = 0;
char *nombre;
int contador = 0;
int numero_comando = 0;


struct Empleados{
    char nombre[100];
    char apellido[100];
    char nombre_departamento[100];
    char ubicacion_departamento[100];
    char numero_proyecto[100];
    char horas[100];
    char numero_departamento[100];
    char id_empleado[100];
}empleado[10];


// PROTOTIPOS DE FUNCIONES
void abrirBase(char (*nombre_base)[]);
void consultaProyecto(char (*nombre_proyecto)[]);
static int imprimirDatos(void *nada, int longitud, char **Dato, char **NombreColumna);


// MAIN
int main() {

    // Le pedimos al usuario que introduzca el nombre de la base de datos que quiere consultar y lo almacenamos
    // en una variable, que enviamos como puntero a la función abrir base para que la abra
    char nombre_base[20];

    printf("\t\t.:MENU:.\n\n");
    printf("Introduzca el nombre de la base de datos a consultar: ");
    scanf("%s",&*nombre_base);

    // Abrimos la base de datos
    abrirBase(&nombre_base);

    // Le pedimos al usuario el nombre del proyecto que quiere buscar
    char nombre_proyecto[20];

    printf("Introduzca el nombre del proyecto a consultar: ");
    scanf("%s",&*nombre_proyecto);

    // Y llamamos a la funcón que imprimirá los datos
    consultaProyecto(&nombre_proyecto);

    // Cerramos la base de datos
    sqlite3_close(db);
}


// FUNCIONES
void abrirBase(char (*nombre_base)[]){

    res = sqlite3_open(*nombre_base, &db);

    // No merece la pena comprobar si hemos abierto la base de datos, porque aunque la base no exista, dev c++ la crea
    // luego res = 0 siempre. Dejo el código por si sirve en el futuro
    if (res == 1){
        fprintf(stderr, "No puedo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else{
        fprintf(stderr, "Base de datos abierta correctamente\n\n");
    }
}


void consultaProyecto(char (*nombre_proyecto)[]){

    // Tengo que definir char de esta forma en vez de como puntero sin gamaño definido, porque si no, es una
    // variable de sólo lectura, de forma que no podré concatenarla
    numero_comando = 1;
    char comando1[100] = "SELECT Numero_Dpto FROM Proyecto WHERE Proy_Nombre = '";
    strcat(comando1,*nombre_proyecto);
    strcat(comando1,"';");

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando1, imprimirDatos, 0, &error);

    numero_comando = 2;
    char comando2[100] = "SELECT Proy_Numero FROM Proyecto WHERE Proy_Nombre = '";
    strcat(comando2,*nombre_proyecto);
    strcat(comando2,"';");

    printf("\n%s\n",comando2);


    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando2, imprimirDatos, 0, &error);

    numero_comando = 3;
    char comando3[100] = "SELECT Nombre_Dpto FROM Departamento WHERE Num_Dpto = ";
    strcat(comando3,empleado[0].numero_departamento);
    strcat(comando3,";");

    printf("\n%s\n",comando3);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando3, imprimirDatos, 0, &error);

    numero_comando = 4;
    char comando4[100] = "SELECT Ubicacion_Dpto FROM Ubicaciones_Departamento WHERE Numero_Dpto = ";
    strcat(comando4,empleado[0].numero_departamento);
    strcat(comando4,";");

    printf("\n%s\n",comando4);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando4, imprimirDatos, 0, &error);

    numero_comando = 5;
    char comando5[100] = "SELECT ID_Empleado FROM Trabaja_En WHERE Proy_Numero = ";
    strcat(comando5,empleado[0].numero_proyecto);
    strcat(comando5,";");

    printf("\n%s\n",comando5);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando5, imprimirDatos, 0, &error);

    numero_comando = 6;
    char comando6[100] = "SELECT Nombre_Pila FROM Empleado WHERE ID_Empleado =  ";
    strcat(comando6,empleado[0].id_empleado);
    strcat(comando6,";");

    printf("\n%s\n",comando6);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando6, imprimirDatos, 0, &error);

    numero_comando = 7;
    char comando7[100] = "SELECT Apellido_1 FROM Empleado WHERE ID_Empleado =  ";
    strcat(comando7,empleado[0].id_empleado);
    strcat(comando7,";");

    printf("\n%s\n",comando7);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando7, imprimirDatos, 0, &error);

    numero_comando = 8;
    char comando8[100] = "SELECT Horas FROM Trabaja_En WHERE Proy_Numero =  ";
    strcat(comando8,empleado[0].numero_proyecto);
    strcat(comando8,";");

    printf("\n%s\n",comando8);

    // Ejecución de las instrucciones y llamamos a la función imprimir datos
    res = sqlite3_exec(db, comando8, imprimirDatos, 0, &error);

	int i;
    // Una vez tenemos las estructuras rellenas, podemos imprimir los datos
    printf("\n\nEMPLEADO             TRABAJA EN               ASOACIADO AL PROYECTO            DEPARTAMENTO           HORAS DE TRABAJO\n");
    for ( i = 0; i < 2; i++){
        printf("%s %s         %s                      %s                          %s                  %s\n", empleado[i].nombre, empleado[i].apellido, empleado[i].ubicacion_departamento, empleado[i].numero_proyecto, empleado[i].nombre_departamento,empleado[i].horas);
    }
    printf("\n");

    // Finalmente, cerramos la base de datos
    sqlite3_close(db);
}

static int imprimirDatos(void *nada, int longitud, char **Dato, char **NombreColumna){
    int i,j;

    for (i = 0; i<longitud; i++){

        switch (numero_comando) {
        case 1:
            for ( j = 0; j<100; j++){
                empleado[i].numero_departamento[j] = Dato[i][j];
            }
            printf("%s", empleado[i].numero_departamento);
            break;
        case 2:
            for ( j = 0; j<100; j++){
                empleado[i].numero_proyecto[j] = Dato[i][j];
            }
            printf("%s", empleado[i].numero_proyecto);
            break;
        case 3:
            for ( j = 0; j<100; j++){
                empleado[i].nombre_departamento[j] = Dato[i][j];
            }
            printf("%s", empleado[i].nombre_departamento);
            break;
        case 4:
            for ( j = 0; j<100; j++){
                empleado[i].ubicacion_departamento[j] = Dato[i][j];
            }
            printf("%s", empleado[i].ubicacion_departamento);
            break;
        case 5:
            for ( j = 0; j<100; j++){
                empleado[i].id_empleado[j] = Dato[i][j];
            }
            printf("%s", empleado[i].id_empleado);
            break;
        case 6:
            for ( j = 0; j<100; j++){
                empleado[i].nombre[j] = Dato[i][j];
            }
            printf("%s", empleado[i].nombre);
            break;
        case 7:
            for ( j = 0; j<100; j++){
                empleado[i].apellido[j] = Dato[i][j];
            }
            printf("%s", empleado[i].apellido);
            break;
        case 8:
            for ( j = 0; j<100; j++){
                empleado[i].horas[j] = Dato[i][j];
            }
            printf("%s", empleado[i].horas);
            break;
        }
    }

    printf("\n");
    fflush(stdin);

    return 0;
}
