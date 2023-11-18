#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct infoRecordatorios{
    char id[50];
    int estado;
    int cont;
    int limite; 
    char recordatorio[100];
    int horas;
    int minutos;
    int dias;
    int recordarDia;
    int recordarMes;
    int recordarAnio;
    int recordarHora;
    int recordarMin;
};


struct infoPacientes{
    char id[50];
    char nombre[50];
    int edad;
};


struct infoAdministradores{
    struct infoPacientes informacionBasica;
    char usuario[50];
    char clave[10];
};


struct Medicamentos{
    char medicamento[50];
};


void inicioDeSeccion(struct infoAdministradores administrador, FILE*);
void registroDeAdministrador(struct infoAdministradores administrador, FILE*);

void menu(struct infoAdministradores admin);

void menuAdministrador(FILE *recordatorios, struct infoAdministradores admin, FILE *medicamentos);

void crearRecordatorioAdmin(FILE *recordatorios, struct infoAdministradores admin, FILE *medicamentos);
void editarRecordatorio(FILE *recordatorios, FILE *medicamentos, struct infoAdministradores admin);
void eliminarRecordatorio(FILE *recordatorios, struct infoAdministradores admin);
void mostrarRecordatoriosAdmin(FILE *recordatorios, struct infoAdministradores admin);
void verRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin);
void eliminarRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin);

void menuPacientes(FILE *recordatorios, FILE *pacientes, struct infoAdministradores admin, FILE *medicamentos);

void registrarPaciente(FILE *pacientes, struct infoAdministradores admin);
void editarPaciente(FILE *pacientes, struct infoAdministradores admin);
void eliminarPaciente(FILE *pacientes, struct infoAdministradores admin);
void consultarPaciente(FILE *pacientes, struct infoAdministradores admin);
void consultarRecordatoriosPaciente(FILE *recordatorios, FILE *pacientes, struct infoAdministradores admin);
void crearRecordatorioPaciente(FILE *recordatorios, FILE *pacientes, FILE *medicamentos);
void mostrarPacientesYRecordatorios(FILE *pacientes, FILE *recordatorios, struct infoAdministradores admin);
void mostrarRecordatoriosVencidosPacientes(FILE *pacientes, FILE *recordatorios, struct infoAdministradores admin);
void eliminarRecordatoriosVencidosPacientes(FILE *recordatorios, struct infoAdministradores admin);

void simularNotificacion(FILE *recordatorios);
void inicializarMedicamentos(FILE *medicamentos);


int main(){

    struct infoAdministradores administrador;

    FILE *administradores;

    administradores = fopen("info_administradores.bin","rb");

    if(administradores == NULL){

        administradores = fopen("info_administradores.bin","wb");
    
    }

    fclose(administradores);

    int salir = 0;

    do{

        int opc;

        salir = 0;

        printf("\tBienvenido\n\n");
        printf("1.- Iniciar seccion\n");
        printf("2.- Registrarme\n");
        printf("0.- Salir\n\n");
        scanf("Opcion: ");
        scanf("%d%*c", &opc);

        switch(opc){

            case 1:

                inicioDeSeccion(administrador, administradores);

            break;

            case 2:
                
                registroDeAdministrador(administrador, administradores);
            
            break;

            case 0:
                
                printf("\nHasta pronto!!!");
                
                salir = 1;
            
            break;

            default:
            
                printf("\nOpcion no valida!!!");
            
            break;
        
        }
    
    }while(salir == 0);

    return 0;

}


// Funcion para iniciar seccion y funcion para registrar administrador


void inicioDeSeccion(struct infoAdministradores administrador, FILE *administradores){

    struct infoAdministradores temp;

    printf("Usuario: ");

    scanf("%49[^\n]%*c", administrador.usuario);

    printf("Clave: ");

    scanf("%49[^\n]%*c", administrador.clave);

        int encontrado = 0;

        administradores = fopen("info_administradores.bin","rb+");

        do{

            fread(&temp, sizeof(temp), 1, administradores);

            if(strcmp(temp.usuario, administrador.usuario) == 0
            && strcmp(temp.clave, administrador.clave) == 0){

                encontrado = 1;
                
                break;
            
            }
        
        }while(!feof(administradores));

        fclose(administradores);

        if(encontrado == 1){

            menu(temp);

        }else{

            printf("Administrador no encontrado!!!\n\n");

        }

};


void registroDeAdministrador(struct infoAdministradores administrador, FILE *administradores){

    struct infoAdministradores temp;

    do{

        printf("\nNombre: ");

        scanf("%49[^\n]%*c", administrador.informacionBasica.nombre);

        printf("\nEdad: ");

        scanf("%d%*c", &administrador.informacionBasica.edad);

        do{

            printf("\nNombre de usuario (3 caracteres): ");

            scanf("%49[^\n]%*c", administrador.usuario);

            int tam = strlen(administrador.usuario);

            if(tam != 3){

                printf("El usuario debe ser de 3 caracteres\n");

            }else{

                break;

            }

        }while(1);

        printf("\nCree una clave de acceso: ");

        scanf("%49[^\n]%*c", administrador.clave);

        //verificar existencia de usuario en archivo administradores (para evitar usuarios repetidos)

        int duplicado = 0;

        administradores = fopen("info_administradores.bin","rb+");

        do{

            fread(&temp, sizeof(temp), 1, administradores);

            if(strcmp(temp.informacionBasica.nombre, administrador.informacionBasica.nombre) == 0 
            || strcmp(temp.usuario, administrador.usuario) == 0){

                duplicado = 1;

                break;

            }

        }while(!feof(administradores));

        fclose(administradores);

        if(duplicado == 1){

            printf("Usario duplicado, revise su nombre o nombre de usuario\n");

        }else{

            administradores = fopen("info_administradores.bin","ab+"); //apertura ("ab+") abre el archivo para lectura y escritura e inicia al final del archivo
            
            fwrite(&administrador, sizeof(struct infoAdministradores), 1, administradores);
            
            fclose(administradores);
            
            break;
        
        }
    
    }while(1);

}


// Funcion menu


void menu(struct infoAdministradores admin){

    //creacion de archivos

    FILE *recordatorios;

    recordatorios = fopen("info_recordatorios.bin","rb");

    if(recordatorios == NULL){

        recordatorios = fopen("info_recordatorios.bin","wb");

    }

    fclose(recordatorios);

    FILE *pacientes;

    pacientes = fopen("info_pacientes.bin","rb");

    if(pacientes == NULL){

        pacientes = fopen("info_pacientes.bin","wb");

    }

    fclose(pacientes);

    FILE *medicamentos;

    medicamentos = fopen("info_medicamentos.bin","rb");

    if(medicamentos == NULL){

        medicamentos = fopen("info_medicamentos.bin","wb");

        inicializarMedicamentos(medicamentos);

    }

    fclose(medicamentos);

    int salir;

    do{

        salir = 0;

        int opc;

        printf("\nDesde que modo desea ingresar: \n\n");

        printf("1.- Administracion Personal\n");

        printf("2.- Administracion de pacientes\n");

        printf("0.- Salir\n\n");

        printf("opcion: ");

        scanf("%d%*c", &opc);

        switch(opc){

            case 1:

                menuAdministrador(recordatorios, admin, medicamentos);

            break;

            case 2:

                menuPacientes(recordatorios, pacientes, admin, medicamentos);

            break;

            case 0:

                salir = 1;

            break;

            default:

                printf("Opcion no valida!!!");

        }

    }while(salir == 0);

}


//Administracion administrador


void menuAdministrador(FILE *recordatorios, struct infoAdministradores admin, FILE *medicamentos){

    int opc, salir = 0;

    do{

        printf("\n\n* * * * * * * * * * * * * * * * * * * * *\n");
        printf("*                                       *\n");
        printf("*        Bienvenido Administrador       *\n");
        printf("*                                       *\n");
        printf("* * * * * * * * * * * * * * * * * * * * *\n\n");
       
        printf("1.- Crear recordatorio\n");
        printf("2.- Editar recordatorio\n");
        printf("3.- Eliminar recordatorio\n");
        printf("4.- Mostrar mis recordatorios\n");
        printf("5.- Ver mis recordatorios vencidos\n");
        printf("6.- Eliminar mis recordatorios vencidos\n");
        printf("7.- Simular notificacion\n");
        printf("0.- Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d%*c", &opc);

        switch(opc){

            case 1:
                crearRecordatorioAdmin(recordatorios, admin, medicamentos);
            break;

            case 2:
                editarRecordatorio(recordatorios, medicamentos, admin);
            break;

            case 3:
                eliminarRecordatorio(recordatorios, admin);
            break;


            case 4:
                mostrarRecordatoriosAdmin(recordatorios, admin);
            break;


            case 5:
                verRecordatoriosVencidosAdmin(recordatorios, admin);
            break;


            case 6:
                eliminarRecordatoriosVencidosAdmin(recordatorios, admin);
            break;


            case 7:
                simularNotificacion(recordatorios);
            break;


            case 0:
                salir = 1;
            break;      
        }

    }while(salir == 0);

}

//me quede aqui
void crearRecordatorioAdmin(FILE *recordatorios, struct infoAdministradores admin, FILE *medicamentos){


    struct infoRecordatorios recordRecuperado;
    struct infoRecordatorios recordatorio;
    struct Medicamentos medicamento;
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

    //crear un id para los recordatorios de los administradores, el cual estara conformado por: la cadena "adm"+ el id del administrador + un numero aleatorio de 3 digitos el cual podria quedar asi: "admvic123", verificar que el id no este duplicado en el archivo recordatorios (info_recordatorios.bin), si es asi hay que generar otro id

    srand(time(NULL)); // Inicializar la semilla del generador de numeros aleatorios

    do{
        int num1 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num2 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num3 = rand() % 10; // Generar un numero aleatorio entre 0 y 9


        sprintf(recordatorio.id, "adm%s%d%d%d", admin.usuario, num1, num2, num3); // Crear el ID del recordatorio


        printf("id: %s\n", recordatorio.id);


        recordatorios = fopen("info_recordatorios.bin", "rb"); // Abrir el archivo recordatorios (info_recordatorios.bin) en modo lectura


        if (recordatorios == NULL) {
            printf("Error al abrir el archivo.\n");
            return;
        }


        // Leer el archivo recordatorios (info_recordatorios.bin) y verificar que el ID no este duplicado
        while (fread(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios)) {
            if (strcmp(recordatorio.id, recordRecuperado.id) == 0) {
                printf("El id %s ya existe.\n", recordatorio.id);
                fclose(recordatorios);
                return;
            }
        }


        fclose(recordatorios);


    } while (0);

    printf("\n\nEl id del recordatorio es: %s\n", recordatorio.id);


    printf("Que necesitas recordar: \n\n");


    //generar un menu de acuerdo a los medicamentos que hay en el archivo medicamentos (info_medicamentos.bin)


    medicamentos = fopen("info_medicamentos.bin","rb");


    int i=1;
    int opc;


    do{
        fread(&medicamento, sizeof(medicamento), 1, medicamentos);
        printf("%d.- %s\n", i, medicamento.medicamento);
        i++;
    }while(!feof(medicamentos));


    fclose(medicamentos);


    printf("12.- Personalizado\n\n");

    do{

        printf("Opcion: ");

        scanf("%d%*c", &opc);

        if(opc < 1 || opc > 12){

            printf("\n\nOpcion no valida!!!\n\n");

        }

    }while(opc < 1 || opc > 12);

    i = 1;

    medicamentos = fopen("info_medicamentos.bin","rb");


    do{
        fread(&medicamento, sizeof(medicamento), 1, medicamentos);


        if(i == opc){
            strcpy(recordatorio.recordatorio, medicamento.medicamento);
            break;
        }
        i++;


    }while(i<12);

    fclose(medicamentos);

    if(opc == 12){

        printf("Ingrese su recordatorio personalizado: ");

        scanf("%[^\n]%*c", recordatorio.recordatorio);

    }

    //prguntar desde cuando se debe recordar el medicamento

    do{

        do{

            printf("Desde cuando desea recordar el medicamento: \n\n");

            printf("1.- Desde ahora\n");

            printf("2.- Desde una fecha y hora especifica\n");

            printf("Opcion: ");

            scanf("%d%*c", &opc);

            if(opc != 1 && opc != 2){

                printf("\n\nOpcion no valida!!!\n\n");
            
            }

        }while(opc != 1 && opc != 2);

        if(opc == 1){
                
                recordatorio.recordarHora = horario.hora;
                recordatorio.recordarMin = horario.minuto;
                recordatorio.recordarDia = horario.dia;
                recordatorio.recordarMes = horario.mes;
                recordatorio.recordarAnio = horario.anio;

        }else if(opc == 2){

                printf("Ingrese la fecha y hora desde la cual desea recordar el medicamento se trabaja en formato de 24h: \n\n");

            do{

                printf("Hora y minutos: (hh:mm)");

                scanf("%d:%d%*c", &recordatorio.recordarHora, &recordatorio.recordarMin);

                if(recordatorio.recordarHora < 0 || 
                recordatorio.recordarHora > 23 || 
                recordatorio.recordarMin < 0 || 
                recordatorio.recordarMin > 59){

                    printf("\n\nHora no valida!!!\n\n");

                }

            }while(recordatorio.recordarHora < 0 || 
            recordatorio.recordarHora > 23 || 
            recordatorio.recordarMin < 0 || 
            recordatorio.recordarMin > 59);

            do{

                printf("\n\nFecha: (dd-mm-aaaa)\n\n");

                scanf("%d-%d-%d%*c", &recordatorio.recordarDia, &recordatorio.recordarMes, &recordatorio.recordarAnio);

                if(recordatorio.recordarDia < 1 ||
                recordatorio.recordarDia > 31 ||
                recordatorio.recordarMes < 1 ||
                recordatorio.recordarMes > 12 ||
                recordatorio.recordarAnio < horario.anio ||
                (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes < horario.mes) ||
                (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes == horario.mes && recordatorio.recordarDia < horario.dia)){

                    printf("\n\nFecha no valida!!!\n\n");

                }

            }while(recordatorio.recordarDia < 1 ||
            recordatorio.recordarDia > 31 ||
            recordatorio.recordarMes < 1 ||
            recordatorio.recordarMes > 12 ||
            recordatorio.recordarAnio < horario.anio ||
            (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes < horario.mes) ||
            (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes == horario.mes && recordatorio.recordarDia < horario.dia));

        }

    }while(opc != 1 && opc != 2);

    //preguntar cada cuanto se debe recordar el medicamento

    do{

        printf("Cada cuantas horas: ");

        scanf("%d%*c", &recordatorio.horas);

        if(recordatorio.horas < 0 || recordatorio.horas > 24){

            printf("\n\nHora no valida!!!\n\n");

        }

    }while(recordatorio.horas < 0 || recordatorio.horas > 24);

    do{

        printf("Cada cuantos minutos: ");

        scanf("%d%*c", &recordatorio.minutos);

        if(recordatorio.minutos < 0 || recordatorio.minutos > 60){

            printf("\n\nMinutos no validos!!!\n\n");

        }

    }while(recordatorio.minutos < 0 || recordatorio.minutos > 60);

    do{

        printf("Por cuantos dias: ");

        scanf("%d%*c", &recordatorio.dias);

        if(recordatorio.dias < 0){

            printf("\n\nDias no validos!!!\n\n");

        }

    }while(recordatorio.dias < 0);

    //Definir Limite de repeticiones del recordatorio

    int horaAMin = recordatorio.horas*60;

    int diaAMin = recordatorio.dias*1440;

    lim = diaAMin/(recordatorio.minutos + horaAMin);

    recordatorio.limite = lim;

    //solucionar el problema de formato 24 horas cuando sumo horas

    struct fechaYHora suma;

    suma.hora = horario.hora + recordatorio.horas;

    suma.minuto = horario.minuto + recordatorio.minutos;

    if(suma.minuto >= 60) {

        suma.hora += suma.minuto / 60;
        
        suma.minuto %= 60;
    
    }

    if(suma.hora >= 24) {

        suma.hora %= 24;

    }

    recordatorio.estado = 1;

    recordatorio.recordarHora = suma.hora;

    recordatorio.recordarMin = suma.minuto;

    recordatorio.recordarDia = horario.dia;

    recordatorio.recordarMes = horario.mes;

    recordatorio.recordarAnio = horario.anio;

    //guardar en archivo recordatorios(info_recordatorios.bin)

    recordatorios = fopen("info_recordatorios.bin","ab+"); //apertura ("ab+") abre el archivo para lectura y escritura e inicia al final del archivo

    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

    rewind(recordatorios);

    fread (&recordRecuperado, sizeof(recordRecuperado), 1, recordatorios);    

    fclose(recordatorios);

    printf("Id: %s\n", recordatorio.id);

    printf("Cont: %d\n", recordatorio.cont);

    printf("Limite: %d\n", recordatorio.limite);

    printf("Medicamento: %s\n", recordatorio.recordatorio);

    printf("Horas: %d\n", recordatorio.horas);

    printf("Minutos: %d\n", recordatorio.minutos);

    printf("Dias: %d\n", recordatorio.dias);

    printf("Recordar Hora: %d\n", recordatorio.recordarHora);

    printf("Recordar Min: %d\n", recordatorio.recordarMin);

    printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);

    printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);

}


void editarRecordatorio(FILE *recordatorios, FILE *medicamentos, struct infoAdministradores admin){

    struct infoRecordatorios recordatorio;
    
    struct Medicamentos medicamento;

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

    char id[10];

    printf("Ingrese el id del recordatorio que desea editar: ");

    scanf("%s%*c", id);

    recordatorios = fopen("info_recordatorios.bin", "rb+"); // Abrir el archivo recordatorios (info_recordatorios.bin) en modo lectura

    if (recordatorios == NULL) {

        printf("Error al abrir el archivo.\n");

        return;

    }

    int existe = 0;

    // Leer el archivo recordatorios (info_recordatorios.bin) y buscar un recordatorio con el ID ingresado

    while(fread(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios)){

        printf("id: %s\n", recordatorio.id);
        printf("id recordatorio: %s\n", id);

        if((strcmp(id, recordatorio.id) == 0 && recordatorio.estado == 1 && 
        admin.usuario[0] == recordatorio.id[0] &&
        admin.usuario[1] == recordatorio.id[1] && 
        admin.usuario[2] == recordatorio.id[2])||(
            strcmp(id,recordatorio.id) == 0 &&
            recordatorio.estado == 1 &&
            recordatorio.id[0] == 'a' && 
            recordatorio.id[1] == 'd' && 
            recordatorio.id[2] == 'm' && 
            admin.usuario[0] == recordatorio.id[3] && 
            admin.usuario[1] == recordatorio.id[4] && 
            admin.usuario[2] == recordatorio.id[5])){

            existe = 1;

            printf("\nId: %s\n", recordatorio.id);
            printf("estado: %d\n", recordatorio.estado);

            struct fechaYHora suma;

            int horaAMin = 0, diaAMin = 0, lim = 0;

            //preguntar que necesita editar del recordatorio Y guardar ese cambio en struct recordatorioRecuperado

            printf("Que desea editar: \n\n");

            printf("1.- Recordatorio\n");
            printf("2.- Cada cuantas horas\n");
            printf("3.- Cada cuantos minutos\n");
            printf("4.- Por cuantos dias\n");
            printf("Opcion: ");

            int opc;

            scanf("%d%*c", &opc);

            if(opc == 1){

                    printf("Que necesitas recordar: \n\n");

                    //generar un menu de acuerdo a los medicamentos que hay en el archivo medicamentos (info_medicamentos.bin)

                    medicamentos = fopen("info_medicamentos.bin","rb");

                    int i=1;

                    int opc;

                    do{

                        fread(&medicamento, sizeof(medicamento), 1, medicamentos);

                        printf("%d.- %s\n", i, medicamento.medicamento);

                        i++;

                    }while(!feof(medicamentos));

                    fclose(medicamentos);

                    printf("11.- Personalizado\n\n");

                    printf("Opcion: ");

                    scanf("%d%*c", &opc);

                    i = 1;

                    medicamentos = fopen("info_medicamentos.bin","rb");

                    do{

                        fread(&medicamento, sizeof(medicamento), 1, medicamentos);

                        if(i == opc){

                            strcpy(recordatorio.recordatorio, medicamento.medicamento);

                            break;

                        }

                        i++;

                    }while(i<12);

                    fclose(medicamentos);

                    if(opc == 12){

                        printf("Ingrese su recordatorio personalizado: ");

                        scanf("%[^\n]%*c", recordatorio.recordatorio);

                    }

                    fseek(recordatorios, -196, SEEK_CUR);                      
                    
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);
                    
                    printf("Recordatorio Modificado con exito !!!\n");

                    break;

            }else if(opc == 2){

                    printf("Cada cuantas horas: ");

                    scanf("%d%*c", &recordatorio.horas);

                    //Definir Limite de repeticiones del recordatorio

                    horaAMin = recordatorio.horas*60;

                    diaAMin = recordatorio.dias*1440;

                    lim = diaAMin/(recordatorio.minutos + horaAMin);

                    recordatorio.limite = lim;

                    //solucionar el problema de formato 24 horas cuando sumo horas

                    suma.hora = horario.hora + recordatorio.horas;

                    suma.minuto = horario.minuto + recordatorio.minutos;




                    if(suma.minuto >= 60) {
                        suma.hora += suma.minuto / 60;
                        suma.minuto %= 60;
                    }


                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }


                    recordatorio.recordarHora = suma.hora;          
                    recordatorio.recordarMin = suma.minuto;
                    recordatorio.recordarDia = horario.dia;
                    recordatorio.recordarMes = horario.mes;
                    recordatorio.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                              
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");

                    break;
            
            }else if(opc == 3){

                    printf("Cada cuantos minutos: ");
                    scanf("%d%*c", &recordatorio.minutos);


                    //Definir Limite de repeticiones del recordatorio


                    horaAMin = recordatorio.horas*60;
                    diaAMin = recordatorio.dias*1440;


                    lim = diaAMin/(recordatorio.minutos + horaAMin);


                    recordatorio.limite = lim;


                    //solucionar el problema de formato 24 horas cuando sumo horas


                    suma.hora = horario.hora + recordatorio.horas;
                    suma.minuto = horario.minuto + recordatorio.minutos;




                    if(suma.minuto >= 60) {
                        suma.hora += suma.minuto / 60;
                        suma.minuto %= 60;
                    }


                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }
                    recordatorio.estado = 1;
                    recordatorio.recordarHora = suma.hora;          
                    recordatorio.recordarMin = suma.minuto;
                    recordatorio.recordarDia = horario.dia;
                    recordatorio.recordarMes = horario.mes;
                    recordatorio.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                              
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");

                    break;
            }else if(opc == 4){

                    printf("Por cuantos dias: ");
                    scanf("%d%*c", &recordatorio.dias);

                    //Definir Limite de repeticiones del recordatorio

                    horaAMin = recordatorio.horas*60;

                    diaAMin = recordatorio.dias*1440;

                    lim = diaAMin/(recordatorio.minutos + horaAMin);

                    recordatorio.limite = lim;

                    //solucionar el problema de formato 24 horas cuando sumo horas

                    suma.hora = horario.hora + recordatorio.horas;

                    suma.minuto = horario.minuto + recordatorio.minutos;

                    if(suma.minuto >= 60) {

                        suma.hora += suma.minuto / 60;

                        suma.minuto %= 60;

                    }

                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }
                    recordatorio.estado = 1;
                    recordatorio.recordarHora = suma.hora;          
                    recordatorio.recordarMin = suma.minuto;
                    recordatorio.recordarDia = horario.dia;
                    recordatorio.recordarMes = horario.mes;
                    recordatorio.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                              
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");

                    break;
            }else{

                printf("Opcion no valida!!!\n");

            }

        }
    }

    fclose(recordatorios);

    if(existe == 0){
        printf("\nNo existe un recordatorio con ese id o el recordatorio se encuentra inactivo!!!\n");
    }
}


void eliminarRecordatorio(FILE *recordatorios, struct infoAdministradores admin){


    struct infoRecordatorios recordatorio;


    char buscado[10];
    int encontrado = 0;


    //Archivo temporal
    FILE *temporal;
    temporal = fopen("temp.bin","rb+");


    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
    }


    //ubicar recordatorio a eliminar y pasar lo que no eliminare a un archivo temporal




    printf("\nIngrese el id del recordatorio el cual desea elminiar: ");
    scanf("%[^\n]%*c", buscado);


    recordatorios = fopen("info_recordatorios.bin","rb+");


    do{
        
        fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);

        if(!feof(recordatorios)){
        
            if((strcmp(buscado,recordatorio.id) == 0 && recordatorio.estado == 1 && 
            admin.usuario[0] == recordatorio.id[0] &&
            admin.usuario[1] == recordatorio.id[1] && 
            admin.usuario[2] == recordatorio.id[2])||(
                strcmp(buscado,recordatorio.id) == 0 && recordatorio.estado == 1 &&
                recordatorio.id[0] == 'a' && 
                recordatorio.id[1] == 'd' && 
                recordatorio.id[2] == 'm' && 
                admin.usuario[0] == recordatorio.id[3] && 
                admin.usuario[1] == recordatorio.id[4] && 
                admin.usuario[2] == recordatorio.id[5])){


                encontrado = 1;



            }else{
            
                if(!feof(recordatorios)){
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, temporal);
                }
            }
        }
   
    }while(!feof(recordatorios));


    fclose(temporal); // Cierra el archivo temporal antes de intentar renombrarlo
    fclose(recordatorios);


   
    if(encontrado == 1){
        printf("Se ha eliminado correctamente el recordatorio");
       
        char temp[30] = "temp.bin";
        char orig[30] = "info_recordatorios.bin";
   
        if(remove(orig) == 0){


            if(rename(temp, orig) == 0){
            }else{
                perror("Error al renombrar archivo");
            }
        }else{
            perror("Error al eliminar original");
        }
    }else{
        printf("No se ha podido encontrar el recordatorio");
    }
}


void mostrarRecordatoriosAdmin(FILE *recordatorios, struct infoAdministradores admin){


    struct infoRecordatorios recordatorio;


    recordatorios = fopen("info_recordatorios.bin","rb+");


    do{
        fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);


        if(!feof(recordatorios)){
            int tam = strlen(admin.usuario);
            int contador = 0;


            for(int i = 0; i < tam; i++){
                if(admin.usuario[i] == recordatorio.id[i+3]){
                    contador++;
                }
            }


            if(contador == tam && recordatorio.estado == 1){
                printf("\nId: %s\n", recordatorio.id);
                printf("Cont: %d\n", recordatorio.cont);
                printf("Limite: %d\n", recordatorio.limite);
                printf("Medicamento: %s\n", recordatorio.recordatorio);
                printf("Horas: %d\n", recordatorio.horas);
                printf("Minutos: %d\n", recordatorio.minutos);
                printf("Dias: %d\n", recordatorio.dias);
                printf("Recordar Hora: %d\n", recordatorio.recordarHora);
                printf("Recordar Min: %d\n", recordatorio.recordarMin);
                printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);
                printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);
            }
        }
    }while(!feof(recordatorios));


    fclose(recordatorios);
}


void verRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin){




    struct infoRecordatorios recordatorio;


    recordatorios = fopen("info_recordatorios.bin","rb+");


    do{
        fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);


        //hacer que encuentre unicamente los recordatorios del administrador buscado en el archivo recordatorios (info_recordatorios.bin)


        if(!feof(recordatorios)){
            int tam = strlen(admin.usuario);
            int contador = 0;


            for(int i = 0; i < tam; i++){
                if(admin.usuario[i] == recordatorio.id[i+3]){
                    contador++;
                }
            }


            if(contador == tam && recordatorio.estado == 0){
                printf("\nId: %s\n", recordatorio.id);
                printf("Cont: %d\n", recordatorio.cont);
                printf("Limite: %d\n", recordatorio.limite);
                printf("Medicamento: %s\n", recordatorio.recordatorio);
                printf("Horas: %d\n", recordatorio.horas);
                printf("Minutos: %d\n", recordatorio.minutos);
                printf("Dias: %d\n", recordatorio.dias);
                printf("Recordar Hora: %d\n", recordatorio.recordarHora);
                printf("Recordar Min: %d\n", recordatorio.recordarMin);
                printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);
                printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);
            }
        }
    }while(!feof(recordatorios));


    fclose(recordatorios);
}


void eliminarRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin){


//eliminar todos los recordatorios que esten en estado 0 y lo que su id inicien con las el id del admin y lo que no se elimine guardarlo en un archivo temporal y luego renombrarlo


    struct infoRecordatorios recordatorio;


    recordatorios = fopen("info_recordatorios.bin","rb+");


    //Archivo temporal
    FILE *temporal;
    temporal = fopen("temp.bin","rb+");


    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
    }


    do{
        fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);


        //hacer que encuentre unicamente los recordatorios del administrador buscado en el archivo recordatorios (info_recordatorios.bin)


        if(!feof(recordatorios)){
            int tam = strlen(admin.usuario);
            int contador = 0;


            for(int i = 0; i < tam; i++){
                if(admin.usuario[i] == recordatorio.id[i+3]){
                    contador++;
                }
            }


            if(contador == tam && recordatorio.estado == 0){
                printf("\nId: %s\n", recordatorio.id);
                printf("Estado: %d\n", recordatorio.estado);
            }else{
                fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, temporal);
            }
        }
    }while(!feof(recordatorios));


    printf("Recordatorios eliminados con exito!!!\n");


    fclose(temporal); // Cierra el archivo temporal antes de intentar renombrarlo
    fclose(recordatorios);


    char temp[30] = "temp.bin";
    char orig[30] = "info_recordatorios.bin";


    if(remove(orig) == 0){


        if(rename(temp, orig) == 0){
        }else{
            perror("Error al renombrar archivo");
        }
    }else{
        perror("Error al eliminar original");
    }
}


//Administracion pacientes


void menuPacientes(FILE *recordatorios, FILE *pacientes, struct infoAdministradores admin, FILE *medicamentos){


    int opc, salir = 0;
   
    do{
        printf("\n\n* * * * * * * * * * * * * * * * * * * * *\n");
        printf("*                                       *\n");
        printf("*               Bienvenido              *\n");
        printf("*                                       *\n");
        printf("* * * * * * * * * * * * * * * * * * * * *\n\n");
       
        printf("1.- Registrar paciente\n");
        printf("2.- Editar paciente registrado\n");
        printf("3.- Eliminar paciente\n");
        printf("4.- Consultar paciente\n");
        printf("5.- Consular recordatorios de un paciente\n");
        printf("6.- Crear recordatorio\n");
        printf("7.- Editar recordatorio\n");
        printf("8.- Eliminar recordatorio\n");
        printf("9.- Mostrar pacientes y recordatorios\n");
        printf("10.- Ver recordatorios vecidos\n");
        printf("11.- Eliminar Recordatorios vencidos\n");
        printf("12.- Simular notificacion\n");
        printf("0.- Salir\n\n");


        printf("Seleccione una opcion: ");
        scanf("%d%*c", &opc);


        switch(opc){


            case 1:
                registrarPaciente(pacientes, admin);
            break;


            case 2:
                editarPaciente(pacientes, admin);
            break;


            case 3:
                eliminarPaciente(pacientes, admin);
            break;


            case 4:
                consultarPaciente(pacientes, admin);
            break;


            case 5:
                consultarRecordatoriosPaciente(recordatorios, pacientes, admin);
            break;


            case 6:
                crearRecordatorioPaciente(recordatorios, pacientes, medicamentos);
            break;


            case 7:
                editarRecordatorio(recordatorios, medicamentos, admin);
            break;


            case 8:
                eliminarRecordatorio(recordatorios, admin);
            break;


            case 9:
                mostrarPacientesYRecordatorios(pacientes, recordatorios, admin);
            break;


            case 10:
                mostrarRecordatoriosVencidosPacientes(pacientes, recordatorios, admin);
            break;


            case 11:
                eliminarRecordatoriosVencidosPacientes(recordatorios, admin);
            break;


            case 12:
                simularNotificacion(recordatorios);
            break;


            case 0:
                salir = 1;
            break;

            default:
                printf("\nOpcion no encontrada\n");  
        }
    }while(salir == 0);
}


void registrarPaciente(FILE *pacientes, struct infoAdministradores admin){


    struct infoPacientes paciente;
    struct infoPacientes temp;


    char id[50];


    printf("\nNombre completo del paciente: ");
    scanf("%[^\n]%*c", temp.nombre);


    printf("\nEdad del paciente: ");
    scanf("%d", &temp.edad);


    //unir el usuario del administrador con 3 numeros aleatorios los cuales cambiaran de acuerdo a la hora para crear el ID del paciente, tambien verificar que este ID no este duplicado en el archivo pacientes (info_pacientes.bin), si es asi hay que generar otro id


    srand(time(NULL)); // Inicializar la semilla del generador de numeros aleatorios


    do{
        int num1 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num2 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num3 = rand() % 10; // Generar un numero aleatorio entre 0 y 9


        sprintf(id, "%s%d%d%d", admin.usuario, num1, num2, num3); // Crear el ID del paciente


        pacientes = fopen("info_pacientes.bin", "rb"); // Abrir el archivo pacientes (info_pacientes.bin) en modo lectura


        if (pacientes == NULL) {
            printf("Error al abrir el archivo.\n");
            return;
        }


        // Leer el archivo pacientes (info_pacientes.bin) y verificar que el ID no este duplicado
        while (fread(&paciente, sizeof(struct infoPacientes), 1, pacientes)) {
            if (strcmp(id, paciente.id) == 0) {
                printf("El id %s ya existe.\n", id);
                fclose(pacientes);
                return;
            }
        }


        fclose(pacientes);


    } while (0);


    strcpy(temp.id, id); // Copiar el ID al paciente


    printf("\nEl id del paciente es: %s\n", temp.id);


    // Guardar en el archivo pacientes (info_pacientes.bin)
    pacientes = fopen("info_pacientes.bin", "ab+");
    if (pacientes == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    fwrite(&temp, sizeof(struct infoPacientes), 1, pacientes);
    fclose(pacientes);
}


void editarPaciente(FILE *pacientes, struct infoAdministradores admin){

    struct infoPacientes paciente;

    int tam = sizeof(paciente);

    printf("tam: %d", tam);

    char id[10];            //Id buscado en el archivo pacientes          

    int encontrado = 0;     //Saber si encontre al paciente

    pacientes = fopen("info_pacientes.bin","rb+");      //Se abre el archivo en modo lectura y escritura (se ubica al inicio del archivo)

    printf("\nIngrese el id del paciente a editar: ");

    scanf("%49[^\n]%*c", id);

    do{

        fread(&paciente, sizeof(paciente), 1, pacientes);   //leer paciente del archivo

        //si el id buscado es igual al id del paciente que se ha leido del archivo

        if(strcmp(id,paciente.id) == 0 && 
        admin.usuario[0] == paciente.id[0] &&
        admin.usuario[1] == paciente.id[1] &&
        admin.usuario[2] == paciente.id[2]){

            encontrado = 1;

            int opc;

            printf("\n\nQue dato desea modificar del paciente?\n\n1.- Nombre\n2.- Edad\n\nopcion: ");

            scanf("%d%*c", &opc); //NOTA: verificar dato introducido

            if(opc == 1){

                printf("\n\nIngrese el nuevo nombre que desea asignar al paciente: ");

                scanf("%49[^\n]%*c", paciente.nombre);
               
                fseek(pacientes, -104, SEEK_CUR);                                    // Retrocede el tamanio de la estuctura leida

                fwrite(&paciente, sizeof(struct infoPacientes), 1, pacientes);      //Sobre escribe la nueva estructura en la otra

                printf("Nombre actualizado!!!\n");

            }else if(opc == 2){

                printf("Ingrese la nueva edad que desea asignar al paciente: ");

                scanf("%d", &paciente.edad);

                fseek(pacientes, -104, SEEK_CUR);                                    // Retrocede el tamanio de la estuctura leida

                fwrite(&paciente, sizeof(struct infoPacientes), 1, pacientes);      //Sobre escribe la nueva estructura en la otra

                printf("Edad actualizada con éxito.\n");

            }

            break;

        }

    }while(!feof(pacientes));

    if(encontrado == 0){

        printf("No se ha podido encontrar el usuario, o no te pertenece ese paciente\n");

    }

    fclose(pacientes);

}


void eliminarPaciente(FILE *pacientes, struct infoAdministradores admin){


    struct infoPacientes paciente;


    char buscado[10];
    int encontrado = 0;
   


    //Archivo temporal
    FILE *temporal;
    temporal = fopen("temp.bin","rb+");


   
    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
    }
   
    //ubicar usuario a eliminar y pasar lo que no eliminare a un archivo temporal


   
    printf("\nIngrese la matricula del usuario el cual desea elminiar: ");
    scanf("%[^\n]%*c", buscado);


    pacientes = fopen("info_pacientes.bin","rb+");


    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);
       
        if(strcmp(buscado,paciente.id) == 0 &&
        admin.usuario[0] == paciente.id[0] &&
        admin.usuario[1] == paciente.id[1] &&
        admin.usuario[2] == paciente.id[2]){
            encontrado = 1;
        }else{
           
            if(!feof(pacientes)){
                fwrite(&paciente, sizeof(struct infoPacientes), 1, temporal);
            }
        }
   
    }while(!feof(pacientes));


    fclose(temporal); // Cierra el archivo temporal antes de intentar renombrarlo
    fclose(pacientes);


   
    if(encontrado == 1){
        printf("Se ha eliminado correctamente el usuario");


       
        char temp[20] = "temp.bin";
        char orig[20] = "info_pacientes.bin";
   
        if(remove(orig) == 0){


            if(rename(temp, orig) == 0){
            }else{
                perror("Error al renombrar archivo");
            }
        }else{
            perror("Error al eliminar original");
        }
    }else{
        printf("No se ha podido encontrar a el usuario, o no te pertenece ese paciente");
    }
}


void consultarPaciente(FILE *pacientes, struct infoAdministradores admin){


    struct infoPacientes paciente;
    char buscado[10];
    int encontrado = 0;


    printf("\nIngrese la matricula del usuario a buscar: ");
    scanf("%[^\n]%*c", buscado);


    pacientes = fopen("info_pacientes.bin","rb+");


    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);


        if(strcmp(buscado,paciente.id) == 0 && 
        admin.usuario[0] == paciente.id[0] &&
        admin.usuario[1] == paciente.id[1] &&
        admin.usuario[2] == paciente.id[2]){
            encontrado = 1;
            break;
        }
    }while(!feof(pacientes));


    if(encontrado == 1){


        printf("\nNombre: %s\n", paciente.nombre);
        printf("Edad: %d\n", paciente.edad);
        printf("ID: %s\n", paciente.id);


    }else{
        printf("No se ha podido encontrar el usuario o no te pertenece ese paciente");
    }


    fclose(pacientes);


}


void consultarRecordatoriosPaciente(FILE *pacientes, FILE *recordatorios, struct infoAdministradores admin){

    struct infoPacientes paciente;
    struct infoRecordatorios recordatorio;
    char buscado[10];
    int encontrado = 0;


    printf("\nIngrese la matricula del usuario a buscar: ");
    scanf("%[^\n]%*c", buscado);


    pacientes = fopen("info_pacientes.bin","rb+");


    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);


        if(strcmp(buscado,paciente.id) == 0 &&
        admin.usuario[0] == paciente.id[0] &&
        admin.usuario[1] == paciente.id[1] &&
        admin.usuario[2] == paciente.id[2]){
            encontrado = 1;
            break;
        }
    }while(!feof(pacientes));


    if(encontrado == 1){


        printf("\nNombre: %s\n", paciente.nombre);
        printf("Edad: %d\n", paciente.edad);
        printf("ID: %s\n", paciente.id);


        recordatorios = fopen("info_recordatorios.bin","rb+");

        do{
            fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);

            //hacer que encuentre unicamente los recordatorios del paciente buscado en el archivo recordatorios (info_recordatorios.bin)

            if(!feof(recordatorios)){

                int tam = strlen(buscado);

                int contador = 0;


                for(int i = 0; i < tam; i++){
                    if(buscado[i] == recordatorio.id[i]){
                        contador++;
                    }
                }


                if(contador == tam){
                    printf("Id: %s\n", recordatorio.id);
                    printf("Cont: %d\n", recordatorio.cont);
                    printf("Limite: %d\n", recordatorio.limite);
                    printf("Medicamento: %s\n", recordatorio.recordatorio);
                    printf("Horas: %d\n", recordatorio.horas);
                    printf("Minutos: %d\n", recordatorio.minutos);
                    printf("Dias: %d\n", recordatorio.dias);
                    printf("Recordar Hora: %d\n", recordatorio.recordarHora);
                    printf("Recordar Min: %d\n", recordatorio.recordarMin);
                    printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);
                    printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);
                }
            }
        }while(!feof(recordatorios));


        fclose(recordatorios);
    }else{
        printf("No se ha podido encontrar el usuario o no te pertenece ese paciente");
    }  


}

//arreglar este
void crearRecordatorioPaciente(FILE *recordatorios, FILE *pacientes, FILE *medicamentos){


    struct infoRecordatorios recordRecuperado;
    struct infoRecordatorios recordatorio;
    struct infoPacientes paciente;
    struct Medicamentos medicamento;
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

    //crear un id para los recordatorios de los administradores, el cual estara conformado por: la cadena "adm"+ el id del administrador + un numero aleatorio de 3 digitos el cual podria quedar asi: "admvic123", verificar que el id no este duplicado en el archivo recordatorios (info_recordatorios.bin), si es asi hay que generar otro id

    char idPac[10];

    printf("Ingrese el id del paciente al que desea crearle un recordatorio: ");

    scanf("%[^\n]%*c", idPac);

    //veriricar si el id del paciente existe

    pacientes = fopen("info_pacientes.bin","rb+");
    int existe = 0;

    do{
        fread(&paciente, sizeof(paciente), 1, pacientes);

        if(strcmp(idPac,paciente.id) == 0){

            existe = 1;

            break;

        }

    }while(!feof(pacientes));

    fclose(pacientes);

    if(existe == 0){

        printf("No se ha podido encontrar el usuario o no te pertenece ese paciente");

        return;

    }else{

        srand(time(NULL)); // Inicializar la semilla del generador de numeros aleatorios

        do{
            int num1 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
            int num2 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
            int num3 = rand() % 10; // Generar un numero aleatorio entre 0 y 9

            sprintf(recordatorio.id, "%s%d%d%d", idPac, num1, num2, num3); // Crear el ID del recordatorio

            printf("id: %s\n", recordatorio.id);

            recordatorios = fopen("info_recordatorios.bin", "rb"); // Abrir el archivo recordatorios (info_recordatorios.bin) en modo lectura

            if(recordatorios == NULL){

                printf("Error al abrir el archivo.\n");

                return;

            }

            // Leer el archivo recordatorios (info_recordatorios.bin) y verificar que el ID no este duplicado

            while(fread(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios)){

                if (strcmp(recordatorio.id, recordRecuperado.id) == 0) {

                    printf("El id %s ya existe.\n", recordatorio.id);

                    fclose(recordatorios);

                    return;

                }

            }

            fclose(recordatorios);

        } while (0);

        printf("\n\nEl id del recordatorio es: %s\n", recordatorio.id);

        printf("Que necesitas recordar: \n\n");

        //generar un menu de acuerdo a los medicamentos que hay en el archivo medicamentos (info_medicamentos.bin)

        medicamentos = fopen("info_medicamentos.bin","rb");

        int i=1;

        int opc;

        do{
            fread(&medicamento, sizeof(medicamento), 1, medicamentos);
            printf("%d.- %s\n", i, medicamento.medicamento);
            i++;
        }while(!feof(medicamentos));


        fclose(medicamentos);


        printf("12.- Personalizado\n\n");

        do{

            printf("Opcion: ");

            scanf("%d%*c", &opc);

            if(opc < 1 || opc > 12){

                printf("\n\nOpcion no valida!!!\n\n");

            }

        }while(opc < 1 || opc > 12);

        i = 1;

        medicamentos = fopen("info_medicamentos.bin","rb");


        do{
            fread(&medicamento, sizeof(medicamento), 1, medicamentos);


            if(i == opc){
                strcpy(recordatorio.recordatorio, medicamento.medicamento);
                break;
            }
            i++;


        }while(i<12);

        fclose(medicamentos);

        if(opc == 12){

            printf("Ingrese su recordatorio personalizado: ");

            scanf("%[^\n]%*c", recordatorio.recordatorio);

        }

        //prguntar desde cuando se debe recordar el medicamento

        do{

            do{

                printf("Desde cuando desea recordar el medicamento: \n\n");

                printf("1.- Desde ahora\n");

                printf("2.- Desde una fecha y hora especifica\n");

                printf("Opcion: ");

                scanf("%d%*c", &opc);

                if(opc != 1 && opc != 2){

                    printf("\n\nOpcion no valida!!!\n\n");
                
                }

            }while(opc != 1 && opc != 2);

            if(opc == 1){
                    
                    recordatorio.recordarHora = horario.hora;
                    recordatorio.recordarMin = horario.minuto;
                    recordatorio.recordarDia = horario.dia;
                    recordatorio.recordarMes = horario.mes;
                    recordatorio.recordarAnio = horario.anio;

            }else if(opc == 2){

                    printf("Ingrese la fecha y hora desde la cual desea recordar el medicamento se trabaja en formato de 24h: \n\n");

                do{

                    printf("Hora y minutos: (hh:mm)");

                    scanf("%d:%d%*c", &recordatorio.recordarHora, &recordatorio.recordarMin);

                    if(recordatorio.recordarHora < 0 || 
                    recordatorio.recordarHora > 23 || 
                    recordatorio.recordarMin < 0 || 
                    recordatorio.recordarMin > 59){

                        printf("\n\nHora no valida!!!\n\n");

                    }

                }while(recordatorio.recordarHora < 0 || 
                recordatorio.recordarHora > 23 || 
                recordatorio.recordarMin < 0 || 
                recordatorio.recordarMin > 59);

                do{

                    printf("\n\nFecha: (dd-mm-aaaa)\n\n");

                    scanf("%d-%d-%d%*c", &recordatorio.recordarDia, &recordatorio.recordarMes, &recordatorio.recordarAnio);

                    if(recordatorio.recordarDia < 1 ||
                    recordatorio.recordarDia > 31 ||
                    recordatorio.recordarMes < 1 ||
                    recordatorio.recordarMes > 12 ||
                    recordatorio.recordarAnio < horario.anio ||
                    (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes < horario.mes) ||
                    (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes == horario.mes && recordatorio.recordarDia < horario.dia)){

                        printf("\n\nFecha no valida!!!\n\n");

                    }

                }while(recordatorio.recordarDia < 1 ||
                recordatorio.recordarDia > 31 ||
                recordatorio.recordarMes < 1 ||
                recordatorio.recordarMes > 12 ||
                recordatorio.recordarAnio < horario.anio ||
                (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes < horario.mes) ||
                (recordatorio.recordarAnio == horario.anio && recordatorio.recordarMes == horario.mes && recordatorio.recordarDia < horario.dia));

            }

        }while(opc != 1 && opc != 2);

        //preguntar cada cuanto se debe recordar el medicamento

        do{

            printf("Cada cuantas horas: ");

            scanf("%d%*c", &recordatorio.horas);

            if(recordatorio.horas < 0 || recordatorio.horas > 24){

                printf("\n\nHora no valida!!!\n\n");

            }

        }while(recordatorio.horas < 0 || recordatorio.horas > 24);

        do{

            printf("Cada cuantos minutos: ");

            scanf("%d%*c", &recordatorio.minutos);

            if(recordatorio.minutos < 0 || recordatorio.minutos > 60){

                printf("\n\nMinutos no validos!!!\n\n");

            }

        }while(recordatorio.minutos < 0 || recordatorio.minutos > 60);

        do{

            printf("Por cuantos dias: ");

            scanf("%d%*c", &recordatorio.dias);

            if(recordatorio.dias < 0){

                printf("\n\nDias no validos!!!\n\n");

            }

        }while(recordatorio.dias < 0);

        //Definir Limite de repeticiones del recordatorio

        int horaAMin = recordatorio.horas*60;

        int diaAMin = recordatorio.dias*1440;

        lim = diaAMin/(recordatorio.minutos + horaAMin);

        recordatorio.limite = lim;

        //solucionar el problema de formato 24 horas cuando sumo horas

        struct fechaYHora suma;

        suma.hora = horario.hora + recordatorio.horas;

        suma.minuto = horario.minuto + recordatorio.minutos;

        if(suma.minuto >= 60) {

            suma.hora += suma.minuto / 60;
            
            suma.minuto %= 60;
        
        }

        if(suma.hora >= 24) {

            suma.hora %= 24;

        }

        recordatorio.estado = 1;

        recordatorio.recordarHora = suma.hora;

        recordatorio.recordarMin = suma.minuto;

        recordatorio.recordarDia = horario.dia;

        recordatorio.recordarMes = horario.mes;

        recordatorio.recordarAnio = horario.anio;

        //guardar en archivo recordatorios(info_recordatorios.bin)

        recordatorios = fopen("info_recordatorios.bin","ab+"); //apertura ("ab+") abre el archivo para lectura y escritura e inicia al final del archivo

        fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

        rewind(recordatorios);

        fread (&recordRecuperado, sizeof(recordRecuperado), 1, recordatorios);    

        fclose(recordatorios);

        printf("Id: %s\n", recordatorio.id);

        printf("Cont: %d\n", recordatorio.cont);

        printf("Limite: %d\n", recordatorio.limite);

        printf("Medicamento: %s\n", recordatorio.recordatorio);

        printf("Horas: %d\n", recordatorio.horas);

        printf("Minutos: %d\n", recordatorio.minutos);

        printf("Dias: %d\n", recordatorio.dias);

        printf("Recordar Hora: %d\n", recordatorio.recordarHora);

        printf("Recordar Min: %d\n", recordatorio.recordarMin);

        printf("Fecha: %d-%d-%d\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio);

        printf("Proximo recordatorio: %d:%d\n\n",recordatorio.recordarHora, recordatorio.recordarMin);
    }
}

//editar recordatorio ya esta echa

void mostrarPacientesYRecordatorios(FILE *pacientes, FILE *recordatorios, struct infoAdministradores admin){

    struct infoPacientes pacienteR;

    struct infoRecordatorios recordatorio;

    int recorEnc;

    pacientes = fopen("info_pacientes.bin","rb");

    recordatorios = fopen("info_recordatorios.bin","rb");

    do{

        fread(&pacienteR, sizeof(pacienteR), 1, pacientes);

        if(!feof(pacientes)){

            if(admin.usuario[0] == pacienteR.id[0] && 
            admin.usuario[1] == pacienteR.id[1] &&
            admin.usuario[2] == pacienteR.id[2]
            ){

                printf("ID: %s", pacienteR.id);

                printf("\nNombre: %s\n", pacienteR.nombre);

                printf("Edad: %d\n", pacienteR.edad);

                recorEnc = 0;

                rewind(recordatorios);

                do{

                    fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);

                    if(!feof(recordatorios)){

                        int tam = strlen(pacienteR.id);

                        int contador = 0;

                        for(int i = 0; i < tam; i++){

                            if(pacienteR.id[i] == recordatorio.id[i]){

                                contador++;

                            }

                        }

                        if(contador == tam && recordatorio.estado == 1){

                            printf("Id del recordatorio: %s\n", recordatorio.id);

                            printf("Recordatorio: %s\n", recordatorio.recordatorio);

                            printf("Fecha y hora del proximo reocrdatorio: %d-%d-%d %d:%d\n\n", recordatorio.recordarDia, recordatorio.recordarMes, recordatorio.recordarAnio, recordatorio.recordarHora, recordatorio.recordarMin);

                            recorEnc = 1;

                        }

                    }

                }while(!feof(recordatorios));

                if(recorEnc == 0){

                    printf("No hay recordatorios registrados para este paciente\n\n");

                }

            }

        }

    }while(!feof(pacientes));

    fclose(pacientes);

    fclose(recordatorios);

}  


void mostrarRecordatoriosVencidosPacientes(FILE *pacientes, FILE *recordatorios, struct infoAdministradores admin){

    struct infoPacientes pacienteR;

    struct infoRecordatorios recordatorio;
    
    int recorEnc;

    pacientes = fopen("info_pacientes.bin","rb");
   
    recordatorios = fopen("info_recordatorios.bin","rb");

    do{

        fread(&pacienteR, sizeof(pacienteR), 1, pacientes);

        if(!feof(pacientes)){

            if(admin.usuario[0] == pacienteR.id[0] &&
            admin.usuario[1] == pacienteR.id[1] &&
            admin.usuario[2] == pacienteR.id[2]){

                printf("\nNombre: %s\n", pacienteR.nombre);

                printf("Edad: %d\n", pacienteR.edad);

                printf("ID: %s\n\n", pacienteR.id);

                recorEnc = 0;

                rewind(recordatorios);

                do{

                    fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);

                    if(!feof(recordatorios)){

                        int tam = strlen(pacienteR.id);

                        int contador = 0;

                        for(int i = 0; i < tam; i++){

                            if(pacienteR.id[i] == recordatorio.id[i]){

                                contador++;

                            }

                        }

                        if(contador == tam && recordatorio.estado == 0){

                            printf("Id: %s\n", recordatorio.id);

                            recorEnc = 1;

                        }

                    }

                }while(!feof(recordatorios));

                if(recorEnc == 0){

                    printf("Este paciente no tiene recordatorios vencidos\n\n");

                }

            }
        
        }

    }while(!feof(pacientes));

    fclose(pacientes);

    fclose(recordatorios);

}  


void eliminarRecordatoriosVencidosPacientes(FILE *recordatorios, struct infoAdministradores admin){

    struct infoRecordatorios recordatorio;

    recordatorios = fopen("info_recordatorios.bin","rb+");

    //Archivo temporal

    FILE *temporal;

    temporal = fopen("temp.bin","rb+");

    if(temporal == NULL){

        temporal = fopen("temp.bin","wb");

    }

    //eliminar todos los recordatorios que no inicien con las letras "adm".

    int vencidos = 0;

    do{

        fread(&recordatorio, sizeof(recordatorio), 1, recordatorios);

        if(!feof(recordatorios)){

            int tam = strlen(recordatorio.id);

            int contador = 0;

            for(int i = 0; i < tam; i++){

                if(recordatorio.id[i] == 'a' && recordatorio.id[i+1] == 'd' && recordatorio.id[i+2] == 'm'){

                    contador++;

                }

            }

            if(contador == 0 && recordatorio.estado == 0 &&
            admin.usuario[0] == recordatorio.id[0] &&
            admin.usuario[1] == recordatorio.id[1] &&
            admin.usuario[2] == recordatorio.id[2]){

                vencidos++;

                printf("Sera eliminado el recordatorio: \n");

                printf("\nId: %s\n", recordatorio.id);

                printf("Estado: %d\n", recordatorio.estado);

            }else{

                fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, temporal);

            }

        }

    }while(!feof(recordatorios));

    if(vencidos == 0){

        printf("No hay recordatorios vencidos\n");

    }else{


        printf("Recordatorios eliminados con exito!!!\n");

        fclose(temporal); // Cierra el archivo temporal antes de intentar renombrarlo

        fclose(recordatorios);

        char temp[30] = "temp.bin";

        char orig[30] = "info_recordatorios.bin";

        if(remove(orig) == 0){

            if(rename(temp, orig) == 0){

            }else{

                perror("Error al renombrar archivo");

            }

        }else{

            perror("Error al eliminar original");

        }
    
    }

}



void simularNotificacion(FILE *recordatorios){

    struct infoRecordatorios recordatorio;

    time_t tiempo_actual;

    time(&tiempo_actual);

    struct tm *tiempo_descompuesto;

    tiempo_descompuesto = localtime(&tiempo_actual);

    struct fechaYHora {
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

    recordatorios = fopen("info_recordatorios.bin", "rb+");

    if (recordatorios == NULL) {

        perror("Error al abrir el archivo");

        return;

    }

    while(fread(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios)){

        if(!feof(recordatorios)){

            if(recordatorio.estado == 1 && recordatorio.recordarHora == horario.hora &&
                recordatorio.recordarMin == horario.minuto && recordatorio.recordarDia == horario.dia &&
                recordatorio.recordarMes == horario.mes && recordatorio.recordarAnio == horario.anio){

                printf("------------------------------------------\n");

                printf("Hola %s, es hora de %s\n", recordatorio.id, recordatorio.recordatorio);

                printf("------------------------------------------\n\n");

                if(recordatorio.cont == recordatorio.limite){

                    recordatorio.estado = 0;

                    // Mover el puntero de archivo al inicio del registro actual
                    fseek(recordatorios, -196, SEEK_CUR);

                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

                    fflush(recordatorios);  // Asegurar escritura inmediata en el archivo

                    printf("Este recordatorio ahora está vencido!!!\n");
                }else{

                    //actualizar el recordatorio para la siguiente vez que se muestre

                    recordatorio.cont++;

                    recordatorio.recordarHora += recordatorio.horas;

                    recordatorio.recordarMin += recordatorio.minutos;

                    if(recordatorio.recordarMin >= 60) {

                        recordatorio.recordarHora += recordatorio.recordarMin / 60;

                        recordatorio.recordarMin %= 60;

                    }

                    if(recordatorio.recordarHora >= 24) {

                        recordatorio.recordarHora %= 24;

                    }
                    
                    // Mover el puntero de archivo al inicio del registro actual
                    fseek(recordatorios, -196, SEEK_CUR);

                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

                    fflush(recordatorios);  // Asegurar escritura inmediata en el archivo

                    printf("Este recordatorio se volvera a mostrar en %d:%d\n", recordatorio.recordarHora, recordatorio.recordarMin);

                }

            }

        }

    }

    fclose(recordatorios);

}

//Medicamentos

void inicializarMedicamentos(FILE *medicamentos){

    struct Medicamentos medicamento;


    strcpy(medicamento.medicamento, "tomar paracetamol");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar ibuprofeno");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar aspirina");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar amoxicilina");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar omeprazol");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar loratadina");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar diazepam");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar cetirizina");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar clonazepam");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);


    strcpy(medicamento.medicamento, "tomar metformina");
    fwrite(&medicamento, sizeof(struct Medicamentos), 1, medicamentos);

}


//terminado