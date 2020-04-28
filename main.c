#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

// VARIABLES GLOBALES *******************************************************************************************************/
sqlite3 *db;
int res;
char *error = 0;
char *nombre;
int contador = 0;
int numero_comando = 0;
int contador2;
int contadorfinal = 0;

/* Consideramos que se piden ciertos datos porque son de importancia para la empresa, de forma que en vez de simplemente
 * imprimirlos, los almacenaremos en una estructura, de forma que podamos operar con ellos, por ejemplo en el caso de
 * las horas*/

/* Para poder concatenar cadenas de caracteres, éstas tienen que tener una longitud. No podemos concaternar
 * cadenas definidas por punteros*/

/* En todo momento, el número máximo de personas trabajando en un mismo proyecto es de 2, luego creamos un array de
 * 2 empleados, pero podríamos aumentar el número si se necesitaran más */

struct Empleados{
    char nombre[100];
    char apellido[100];
    char nombre_departamento[100];
    char ubicacion_departamento[100];
    char numero_proyecto[100];
    char horas[100];
    char numero_departamento[100];
    char id_empleado[100];
}empleado[2];


// PROTOTIPOS DE FUNCIONES **************************************************************************************************/

void abrirBase(char (*nombre_base)[]);
void consultaProyecto(char (*nombre_proyecto)[]);
static int obtenerDatos(void *nada, int longitud, char **Dato, char **NombreColumna);


// MAIN  ********************************************************************************************************************/

int main() {

    // Le pedimos al usuario que introduzca el nombre de la base de datos que quiere consultar y lo almacenamos
    // en una variable, que enviamos como puntero a la función abrir base para que la abra
    char nombre_base[20];

    printf("\t.:MENU:.\n\n");
    printf("Introduzca el nombre de la base de datos a consultar: ");
    scanf("%s",&*nombre_base);

    // Abrimos la base de datos
    abrirBase(&nombre_base);

    // Le pedimos al usuario el nombre del proyecto que quiere buscar. Modificamos scanf para que lea hasta el salto de línea
    char nombre_proyecto[20];

    printf("Introduzca el nombre del proyecto a consultar: ");
    scanf(" %[^\t\n]s",&*nombre_proyecto);

    // Y llamamos a la funcón que imprimirá los datos
    consultaProyecto(&nombre_proyecto);

    /* Ahora ya tenemos una estructura con todos los datos más importantes de todos los empleados trabajando en un proyecto
     * determinado. Creamos dos vectores donde almacenaremos el id de empleado, el numero de proyecto y las horas de cada empleado
     * como enteros */
    int l, num_proy, num_dpto, horas[2], ides[2];

    num_proy = atoi(empleado[0].numero_proyecto);
    num_dpto = atoi(empleado[0].numero_departamento);

    for (l = 0; l < 2; l++){
        ides[l] = atoi(empleado[l].id_empleado);
        horas[l] = atoi(empleado[l].horas);
    }

    // Y finalmente, imprimirmos los datos en forma de tabla
    printf("\n\nEMPLEADO             TRABAJA EN               ASOACIADO AL PROYECTO            DEPARTAMENTO           HORAS DE TRABAJO\n");
    int i;
    for (i = 0; i < contadorfinal; i++){
        printf("%s %s           %s                      %s                          %s                  %s\n", empleado[i].nombre, empleado[i].apellido, empleado[0].ubicacion_departamento, empleado[0].numero_proyecto, empleado[0].nombre_departamento,empleado[i].horas);
    }
    printf("\n");

    // Cerramos la base de datos
    sqlite3_close(db);
    
    system("pause");
}


// FUNCIONES *****************************************************************************************************************/

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

    // Se crea una variable que será una bandera, para indicarle a la función obtener dato dónde debe guardarse el mismo
    // Después se crea el comando
    numero_comando = 1;
    char comando1[100] = "SELECT Numero_Dpto FROM Proyecto WHERE Proy_Nombre = '";
    strcat(comando1,*nombre_proyecto);
    strcat(comando1,"';");

    // Y se llama a la función que obtendrá el dato diciéndole en qué empleado se guardará el dato,
    // ya que contador es una variable global
    contador = 0;
    res = sqlite3_exec(db, comando1, obtenerDatos, 0, &error);

    numero_comando = 2;
    char comando2[100] = "SELECT Proy_Numero FROM Proyecto WHERE Proy_Nombre = '";
    strcat(comando2,*nombre_proyecto);
    strcat(comando2,"';");

    contador = 0;
    res = sqlite3_exec(db, comando2, obtenerDatos, 0, &error);

    // Conforme se obtienen datos, se usan para concanetar cadenas de caracteres que constituirán el siguiente comando
    // para obtener la información necesaria
    numero_comando = 3;
    char comando3[100] = "SELECT Nombre_Dpto FROM Departamento WHERE Num_Dpto = ";
    strcat(comando3,empleado[0].numero_departamento);
    strcat(comando3,";");

    contador = 0;
    res = sqlite3_exec(db, comando3, obtenerDatos, 0, &error);

    numero_comando = 4;
    char comando4[100] = "SELECT Ubicacion_Dpto FROM Ubicaciones_Departamento WHERE Numero_Dpto = ";
    strcat(comando4,empleado[0].numero_departamento);
    strcat(comando4,";");

    contador = 0;
    res = sqlite3_exec(db, comando4, obtenerDatos, 0, &error);

    /* El caso de obtener el ID de empleado es especial porque es el primer comando en el que tenemos dos salidas
     * La función imprime línea a línea, y se vuelve a ejecutar tantas veces como filas haya. Aquí es cuando entra el contador
     * que aumentará en uno su valor al terminar la ejecución de la función para que en la próxima ejecución, los datos
     * se almacenen en el siguiente empleado */
    numero_comando = 5;
    char comando5[100] = "SELECT ID_Empleado FROM Trabaja_En WHERE Proy_Numero = ";
    strcat(comando5,empleado[0].numero_proyecto);
    strcat(comando5,";");

    contador = 0;
    res = sqlite3_exec(db, comando5, obtenerDatos, 0, &error);


    /* A partir de este momento, necesitamos el nombre, apellidos y horas de trabajo de empleados que tienen
    dos ids distintos, luego tenemos que repetir la siguiente parte tantas veces como empleados haya trabajando
    en un mismo proyecto, que en este caso es como máximo 2 */
    int k;
    for (k= 0; k<2; k++){

        // La variable global contador 2 nos asegurará que estamos guardando los datos en la función obtener datos
        // en la posición correcta
        contador2 = k;

        // Y repetimos el mismo proceso que antes
        numero_comando = 6;
        char comando6[100] = "SELECT Nombre_Pila FROM Empleado WHERE ID_Empleado = ";
        strcat(comando6,empleado[k].id_empleado);
        strcat(comando6,";");

        contador = 0;
        res = sqlite3_exec(db, comando6, obtenerDatos, 0, &error);

        numero_comando = 7;
        char comando7[100] = "SELECT Apellido_1 FROM Empleado WHERE ID_Empleado = ";
        strcat(comando7,empleado[k].id_empleado);
        strcat(comando7,";");

        contador = 0;
        res = sqlite3_exec(db, comando7, obtenerDatos, 0, &error);

        numero_comando = 8;
        char comando8[100] = "SELECT Horas FROM Trabaja_En WHERE ID_Empleado = '";
        strcat(comando8,empleado[k].id_empleado);
        strcat(comando8,"' AND Proy_Numero = ");
        strcat(comando8,empleado[0].numero_proyecto);
        strcat(comando8,";");

        contador = 0;
        res = sqlite3_exec(db, comando8, obtenerDatos, 0, &error);

    }

}

static int obtenerDatos(void *nada, int longitud, char **Dato, char **NombreColumna){

		int j;
        // A la hora de almacenar los datos, existirá solamente una columna y solamente un dato luego el dato será siempre el 0

        /* Podríamos almacenar la cadena como un puntero, pero entonces la memoria es fija y no podemos realizar la concatenación.
         * Necesitamos recorrer el vector de caracteres almacenándolos en la estructura de caracteres para luego poder concatenarlas. No se
         * puede obtener tampoco el tamaño de un vector del que sólo conocemos el puntero, luego tenemos que poner un valor que sepamos
         * que nunca vamos a alcanzar, en este caso escogemos 100*/

        // Este primer switch contempla los primeros casos en los que el resultado es solamente un dato
        switch (numero_comando) {
        case 1:
            for (j = 0; j<100; j++){
                empleado[0].numero_departamento[j] = Dato[0][j];
            }
            break;
        case 2:
            for (j = 0; j<100; j++){
                empleado[0].numero_proyecto[j] = Dato[0][j];
            }
            break;
        case 3:
            for (j = 0; j<100; j++){
                empleado[0].nombre_departamento[j] = Dato[0][j];
            }
            break;
        case 4:
            for (j = 0; j<100; j++){
                empleado[0].ubicacion_departamento[j] = Dato[0][j];
            }
            break;
        }


        /* Para el caso del ID, este switch almacenará los datos en la posición 0 en la primera iteración, luego aumentamos el
         * contador a 1 para almacenarlo en la siguiente posición del array de estructuras */

        switch (numero_comando) {
        case 5:
            for (j = 0; j<100; j++){
                empleado[contador].id_empleado[j] = Dato[0][j];
            }
            contadorfinal++; // Este contador nos servirá para saber cuántos empleados hay a la hora de imprimirlos
            break;
        }


        /* Y este switch es para el resto de valores posteriores al ID. En este caso, el contador 2 viene definiido por k,
         *  que es la variable que nos indica con qué empleado estamos trabajando en este momento */
        switch (numero_comando) {
        case 6:
            for (j = 0; j<100; j++){
                empleado[contador2].nombre[j] = Dato[0][j];
            }
            break;
        case 7:
            for (j = 0; j<100; j++){
                empleado[contador2].apellido[j] = Dato[0][j];
            }
            break;
        case 8:
            if (Dato[0] == NULL){ // En el caso de Reorganizacion, hay un campo en null que daría error
                empleado[contador2].horas[0] = '0';
            } else {
                for (j = 0; j<100; j++){
                    empleado[contador2].horas[j] = Dato[0][j];
                }
            }
            break;
        }

        // Finalmente, aumentamos el contador para el caso del ID
        contador++;
        fflush(stdin);

    return 0;
}

