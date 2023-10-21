#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct datosRecor{
    char id[10];
    int numRecor;
    int cont;
    int limite;
    char medicamento[100];  //corregir, es recordatorio

    //Considerar cual es la importancia de guardarlos
    int Horas;
    int Minutos;
    int Dias;

    int recordarDia;
    int recordarMes;
    int recordarAnio;
    int recordarHora;
    int recordarMin;
};
struct infoPacientes{
    char id[20];
    char nombre[50];
    int edad;
    int cantRecord;
};


void menuUsuario();
void menuPacientes(struct infoPacientes paciente, struct datosRecor recordatorio, int*);

void registrarPaciente(struct infoPacientes paciente, FILE*, int**);
void elimiarPaciente(struct infoPacientes paciente, FILE*);
void consultarPaciente(struct infoPacientes paciente, FILE*);

void crearRecordatorio(struct datosRecor recordatorio, struct infoPacientes paciente, FILE *recordatorios, FILE *pacientes, int per);
void eliminarRecordatorio(); //mañana
//hacer que ya se muestren en pantalla
//pensar cada cuando saldran a la pantalla

void verRecordatoriosVencidos();
void resetearRecordatoriosVencidos();

void mostrarUsuarioConRecordatorios();
void mostrarMiInformacion();



int main(){
    
    struct datosRecor recordatorio;
    struct infoPacientes paciente;

    strcpy(recordatorio.id, "1");
    recordatorio.cont = 0;
    paciente.cantRecord = 0;


    int cantPacientes = 0;

    char clave1[20] = {"1234"};
    char clave2[20] = {"1234"};

    int opc, salir = 0;


    do{
        char clave[20];
        salir = 0;

        printf("\nDesde que modo desea ingresar: \n\n1.- Administracion Personal\n2.- Administracion de pacientes\n0.- Salir\n\nopcion: ");
        scanf("%d%*c", &opc);
    
        switch(opc){
            
            case 1:
                
                printf("Ingrese la clave de acceso: ");
                scanf("%s", clave);
            
                if(strcmp(clave, clave1) == 0){
                    printf("\n\nLogin exitoso a administracoin personal!!!\n");
                    menuUsuario();
                }else{
                    printf("\n\nClave incorrecta!!!\n");
                }
            break;

            case 2:
                printf("Ingrese la clave de acceso: ");
                scanf("%s", clave);

                if(strcmp(clave, clave2) == 0){
                    printf("\n\nLogin exitoso a administracoin personal!!!\n");
                    menuPacientes(paciente, recordatorio, &cantPacientes);
                }else{
                    printf("\n\nClave incorrecta!!!\n");
                }
            break;

            case 0:
                salir = 1;
            break;

            default:
                printf("Opcion no valida!!!");
        }
    }while(salir == 0);

    return 0;
}

void menuUsuario(){


    int opc, salir = 0;
    FILE *recordatoriosPers;

    recordatoriosPers = fopen("info_persRecordatorios.bin","rb");
   
    if(recordatoriosPers == NULL){
        recordatoriosPers = fopen("info_persRecordatorios.bin","wb");
        fclose(recordatoriosPers);
    }
   
    do{
        printf("\n\nAlerta de recordatorio");
        printf("\n\n\nBienvenido Usuario\n");
        printf("1.- Crear recordatorio\n2.- Eliminar recordatorio\n3.- Ver recordatorios vencidos\n4.- Resetear recordatorios vencidos\n5.- Mostrar mi informacion\n\nOpcion: ");
        scanf("%d%*c", &opc);
        
        switch(opc){
            
            case 1:
                
            break;
            
            case 2:
                eliminarRecordatorio();
            break;
            
            case 3:
                verRecordatoriosVencidos();
            break;
            
            case 4:
                resetearRecordatoriosVencidos();
            break;
            
            case 5:
                mostrarMiInformacion();
            break;
            
            case 0:
                salir = 1;
            break;       
        }
        
    }while(salir == 0);
}

void menuPacientes(struct infoPacientes paciente, struct datosRecor recordatorio, int *cantPacientes){

     //Archivo de pacientes
    FILE *pacientes;
    pacientes = fopen("info_pacientes.bin","rb");

    if(pacientes == NULL){
        pacientes = fopen("info_pacientes.bin","wb");
        fclose(pacientes);
    }
    //saber cuantos pacientes hay y asignar un nuevo valor a cantPacientes

    int cantPac = 0;

    pacientes = fopen("info_pacientes.bin","rb+");
    
    while(!feof(pacientes)){
        fread(&paciente, sizeof(paciente), 1, pacientes);
        cantPac++;
    }

    *cantPacientes = cantPac;

    printf("id del primer paciente: %d\n", *cantPacientes);

    fclose(pacientes);
    
    //Archivo de recordatorios
    FILE *recordatorios;
    recordatorios = fopen("info_recordatorios.bin","rb");
    
    if(recordatorios == NULL){
        recordatorios = fopen("info_recordatorios.bin","wb");
        fclose(recordatorios);
    }
    
    int opc, salir = 0, per = 0;
    
    do{
        printf("\n\n* * * * * * * * * * * * * * * * * * * * *\n");
        printf("*                                       *\n");
        printf("*               Bienvenido              *\n");
        printf("*                                       *\n");
        printf("* * * * * * * * * * * * * * * * * * * * *\n\n");
        
        printf("1.- Registrar usuario\n2.- Eliminar usuario\n3.- Consultar usuario\n4.- Crear recordatorio\n5.- Eliminar recordatorio\n8.- Mostrar todos los usuarios con sus recordatorios\n0.- Salir\n\n");
        
        printf("Seleccione una opcion: ");
        scanf("%d%*c", &opc);

        switch(opc){
            
            case 1:
                registrarPaciente(paciente, pacientes, &cantPacientes);
            break;
            
            case 2:
                elimiarPaciente(paciente, pacientes);
            break;
            
            case 3:
                consultarPaciente(paciente, pacientes);
            break;
            
            case 4:
                crearRecordatorio(recordatorio, paciente, recordatorios, pacientes, per);
            break;
            
            case 5:
                eliminarRecordatorio();
            break;
            
            case 6:
                verRecordatoriosVencidos();
            break;
            
            case 7:
                resetearRecordatoriosVencidos();
            break;
            
            case 8:
                mostrarUsuarioConRecordatorios();
            break;
            
            case 0:
                salir = 1;
            break;
            
            default:
                printf("\nOpcion no encontrada\n");   
        }
    }while(salir == 0);
}
//Verificar si ese paciente no esta repetido
void registrarPaciente(struct infoPacientes paciente, FILE *pacientes, int **cantPacientes) {

    char anioActual[3];
    char id[20]; // Declarar un arreglo para el ID

    printf("\nNombre completo del paciente: ");
    scanf("%49[^\n]%*c", paciente.nombre);

    printf("\nEdad del paciente: ");
    scanf("%d", &paciente.edad);

    // Sacar las primeras tres letras del primer nombre para formar el ID del paciente
    char primTresLetrNom[10];
    strncpy(primTresLetrNom, paciente.nombre, 3);
    primTresLetrNom[3] = '\0';

    // Conseguir los últimos dos dígitos del año actual para formar el ID del paciente
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(anioActual, "%02d", tm.tm_year % 100);

    // Conseguir el número de pacientes para formar el ID del paciente
    int numero = **cantPacientes;
    char numeroComoCadena[10];
    snprintf(numeroComoCadena, sizeof(numeroComoCadena), "%d", numero);

    // Juntar el ID del paciente
    strcpy(id, primTresLetrNom); // Inicializa id con las primeras tres letras
    strcat(id, anioActual); // Concatena los últimos dos dígitos del año
    strcat(id, numeroComoCadena); // Concatena el número de pacientes
    strcpy(paciente.id, id);

    printf("\nid: %s", paciente.id);

    // Guardar en el archivo pacientes (info_pacientes.bin)
    pacientes = fopen("info_pacientes.bin", "ab+"); 
    if (pacientes == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    
    fwrite(&paciente, sizeof(struct infoPacientes), 1, pacientes);
    fclose(pacientes);

    **cantPacientes = **cantPacientes + 1;
}

void elimiarPaciente(struct infoPacientes paciente, FILE *pacientes){

    char buscado[10];
    int encontrado = 0;
   

    //Archivo temporal
   
    FILE *temporal;
   
    temporal = fopen("temp.bin","rb");
   
    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
        fclose(temporal);
    }
   
    //ubicar usuario a eliminar y pasar lo que no eliminare a un archivo temporal

   
    printf("\nIngrese la matricula del usuario el cual desea elminiar: ");
    scanf("%[^\n]%*c", buscado);

    pacientes = fopen("info_pacientes.bin","rb+");
   
    temporal = fopen("temp.bin","rb+");
       
    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);
       
        if(strcmp(buscado,paciente.id) == 0){
            printf("Encontrado");
            encontrado = 1;
        }else{
           
            if(!feof(pacientes)){
                fwrite(&paciente, sizeof(struct infoPacientes), 1, temporal);
            }
        }
   
    }while(!feof(pacientes));
   
    if(encontrado == 1){
        printf("Se ha eliminado correctamente el usuario");
    }else{
        printf("No se ha podido eliminar el usuario");
    }

    //renombrar el archivo temporal como se llamaba el original y el original renombrarlo como se llamaba el temporal
   
    char temp[20] = "temp.bin";
    char orig[20] = "info_pacientes.bin";
   
    if(rename(temp, orig) == 0){
        printf("\nSe han renombrado con exito!!!");
    }else{
        perror("Error");
    }
   
    fclose(temporal);
    fclose(pacientes);
   
}

void consultarPaciente(struct infoPacientes paciente, FILE *pacientes){
   
    char buscado[10];
    int encontrado = 0;
   
    printf("\nIngrese la matricula del usuario a buscar: ");
    scanf("%[^\n]%*c", buscado);
   
    pacientes = fopen("info_pacientes.bin","rb+");
   
    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);
       
        if(strcmp(buscado,paciente.id) == 0){
            encontrado = 1;
            break;
        }
    }while(!feof(pacientes));
   
   
    if(encontrado == 1){
   
        printf("\n%s\n", paciente.nombre);
        printf("%d\n", paciente.edad);
        printf("%s\n", paciente.id);
       
    }else{
        printf("No se ha podido encontrar el usuario");
    }
   
    fclose(pacientes);
   
}

void crearRecordatorio(struct datosRecor recordatorio, struct infoPacientes paciente, FILE *recordatorios, FILE *pacientes, int per){
   
    struct datosRecor recordRecuperado;
    int lim;

    time_t tiempo_actual;
    time(&tiempo_actual);

    struct tm *tiempo_descompuesto;
    tiempo_descompuesto = localtime(&tiempo_actual);

    struct fechaYHora{
        int hora;
        int minuto;
        int dia;
        int mes;
        int anio;
    };

    struct fechaYHora horario;

    horario.hora = tiempo_descompuesto->tm_hour;
    horario.minuto = tiempo_descompuesto->tm_min;
    horario.dia = tiempo_descompuesto->tm_mday;
    horario.mes = tiempo_descompuesto->tm_mon + 1;
    horario.anio = tiempo_descompuesto->tm_year + 1900;

    char buscado[10];

    printf("Ingrese el id del usuario al cual se le asignara el recordatorio: ");
    scanf("%[^\n]%*c", recordatorio.id);

    strcpy(buscado, recordatorio.id);



    int encontrado = 0;
    int pos = 0;    //ubicacion del paciente buscado en el archivo

    //ir a pacientes, buscar ese paciente y en su archivo modificar el valor de cantRecor + 1


    pacientes = fopen("info_pacientes.bin","rb+");
   
    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);
       
        if(strcmp(buscado,paciente.id) == 0){
            encontrado = 1;

            break;
        }
        pos++;
    }while(!feof(pacientes));

   
    fclose(pacientes);
    recordatorio.numRecor = paciente.cantRecord;
   
    printf("Que necesitas recordar: ");
    scanf("%[^\n]%*c", recordatorio.medicamento);

    printf("Cada cuantas horas: ");
    scanf("%d%*c", &recordatorio.Horas);

    printf("Cada cuantos minutos: ");
    scanf("%d%*c", &recordatorio.Minutos);
   
    printf("Por cuantos dias: ");
    scanf("%d%*c", &recordatorio.Dias);

    //Definir Limite de repeticiones del recordatorio

    int horaAMin = recordatorio.Horas*60;
    int diaAMin = recordatorio.Dias*1440;

    lim = diaAMin/(recordatorio.Minutos + horaAMin);

    recordatorio.limite = lim;

    //solucionar el problema de formato 24 horas cuando sumo horas

    struct fechaYHora suma;

    suma.hora = horario.hora + recordatorio.Horas;
    suma.minuto = horario.minuto + recordatorio.Minutos;


    if(suma.minuto >= 60) {
        suma.hora += suma.minuto / 60;
        suma.minuto %= 60;
    }

    if(suma.hora >= 24) {
        suma.hora %= 24;
    }

    //----

    recordatorio.recordarHora = suma.hora;          
    recordatorio.recordarMin = suma.minuto;
    recordatorio.recordarDia = horario.dia;
    recordatorio.recordarMes = horario.mes;
    recordatorio.recordarAnio = horario.anio;


    //
   
    if(per == 1){
       
        //guardar en archivo recordatorios(info_persRecordatorios.bin)
       
        recordatorios = fopen("info_persRecordatorios.bin","ab+"); //apertura ("ab+") abre el archivo para lectura y escritura e inicia al final del archivo
       
        fwrite(&recordatorio, sizeof(struct datosRecor), 1, recordatorios);
       
        rewind(recordatorios);
   
        fread (&recordRecuperado, sizeof(recordRecuperado), 1, recordatorios);
             
        fclose(recordatorios);
       
    }else{
       
        //guardar en archivo recordatorios(info_recordatorios.bin)

        recordatorios = fopen("info_recordatorios.bin","ab+"); //apertura ("ab+") abre el archivo para lectura y escritura e inicia al final del archivo
   
        fwrite(&recordatorio, sizeof(struct datosRecor), 1, recordatorios);
   
        rewind(recordatorios);

        fread (&recordRecuperado, sizeof(recordRecuperado), 1, recordatorios);
     
        fclose(recordatorios);
   
    }
        printf("Id: %s\n", recordatorio.id);
        printf("No. de recordatorio: %d\n", recordatorio.numRecor);
        printf("Cont: %d\n", recordatorio.cont);
        printf("Limite: %d\n", recordatorio.limite);
        printf("Medicamento: %s\n", recordatorio.medicamento);
        printf("Horas: %d\n", recordatorio.Horas);
        printf("Minutos: %d\n", recordatorio.Minutos);
        printf("Dias: %d\n", recordatorio.Dias);
        printf("Recordar Hora: %d\n", recordatorio.recordarHora);
        printf("Recordar Min: %d\n", recordatorio.recordarMin);
        printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);

        printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);
}

void eliminarRecordatorio(){


}

void verRecordatoriosVencidos(){}

void resetearRecordatoriosVencidos(){}

void mostrarUsuarioConRecordatorios(){}

void mostrarMiInformacion(){}
