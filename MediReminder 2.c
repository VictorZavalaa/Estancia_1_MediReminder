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
    char usuario[50];//hacer que sea solo de 3 caracteres
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
void editarRecordatorioAdmin(FILE *recordatorios, FILE *medicamentos);
void eliminarRecordatorio(FILE *recordatorios);
void mostrarRecordatoriosAdmin(FILE *recordatorios, struct infoAdministradores admin);
void verRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin);
void eliminarRecordatoriosVencidosAdmin(FILE *recordatorios, struct infoAdministradores admin);


void menuPacientes(FILE *recordatorios, FILE *pacientes, struct infoAdministradores admin, FILE *medicamentos);
void registrarPaciente(FILE *pacientes, struct infoAdministradores admin);
void editarPaciente(FILE *pacientes);
void eliminarPaciente(FILE *pacientes);
void consultarPaciente(FILE *pacientes);
void consultarRecordatoriosPaciente(FILE *recordatorios, FILE *pacientes);
void crearRecordatorioPaciente(FILE *recordatorios, FILE *medicamentos);
void mostrarPacientesYRecordatorios(FILE *pacientes, FILE *recordatorios);
void mostrarRecordatoriosVencidosPacientes(FILE *pacientes, FILE *recordatorios);



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

        printf("\nNombre de usuario: ");
        scanf("%49[^\n]%*c", administrador.usuario);

        printf("\nCree una contraseña: ");
        scanf("%49[^\n]%*c", administrador.clave);

        //verificar existencia de usuario en archivo administradores (para evitar usuarios repetidos)
        int duplicado = 0;

        administradores = fopen("info_administradores.bin","rb+");

        do{
            fread(&temp, sizeof(temp), 1, administradores);

            if(strcmp(temp.informacionBasica.nombre, administrador.informacionBasica.nombre) == 0
            || strcmp(temp.informacionBasica.nombre, administrador.usuario) == 0){
                duplicado = 1;
                break;
            }
        }while(!feof(administradores));

        fclose(administradores);
        
        if(duplicado == 1){
            printf("Usario duplicado, revise su nombre de usuario o nombre");
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
                editarRecordatorioAdmin(recordatorios, medicamentos);
            break;
            
            case 3:
                eliminarRecordatorio(recordatorios);
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

    printf("Cada cuantas horas: ");
    scanf("%d%*c", &recordatorio.horas);

    printf("Cada cuantos minutos: ");
    scanf("%d%*c", &recordatorio.minutos);

    printf("Por cuantos dias: ");
    scanf("%d%*c", &recordatorio.dias);

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

//tiene error esta funcion no guarada bien los cambios y me altera un recordatorio mal y puede funcionar para ambos
void editarRecordatorioAdmin(FILE *recordatorios, FILE *medicamentos){

    struct infoRecordatorios recordRecuperado;
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

    //tamaño de la estructura recordRecuperado

    int tam = sizeof(recordRecuperado);

    printf("Tamanio de la estructura recordRecuperado: %d\n", tam);


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

    while(fread(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios)){
        if (strcmp(id, recordRecuperado.id) == 0){

            existe = 1;

            //preguntar que necesita editar del recordatorio Y guardar ese cambio en struct recordatorioRecuperado

            printf("Que desea editar: \n\n");

            printf("1.- Recordatorio\n");
            printf("2.- Cada cuantas horas\n");
            printf("3.- Cada cuantos minutos\n");
            printf("4.- Por cuantos dias\n");
            printf("Opcion: ");
            int opc;
            scanf("%d%*c", &opc);

            switch(opc){

                case 1:
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
                            strcpy(recordRecuperado.recordatorio, medicamento.medicamento);
                            break;
                        }
                        i++;

                    }while(i<12);

                    fclose(medicamentos);

                    if(opc == 12){

                        printf("Ingrese su recordatorio personalizado: ");
                        scanf("%[^\n]%*c", recordRecuperado.recordatorio);
                    }

                    fseek(recordatorios, -196, SEEK_CUR);                               
                    fwrite(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");
                break;

                case 2:
                    printf("Cada cuantas horas: ");
                    scanf("%d%*c", &recordRecuperado.horas);

                    //Definir Limite de repeticiones del recordatorio

                    int horaAMin = recordRecuperado.horas*60;
                    int diaAMin = recordRecuperado.dias*1440;
                    int lim;

                    lim = diaAMin/(recordRecuperado.minutos + horaAMin);

                    recordRecuperado.limite = lim;

                    //solucionar el problema de formato 24 horas cuando sumo horas

                    struct fechaYHora suma;

                    suma.hora = horario.hora + recordRecuperado.horas;
                    suma.minuto = horario.minuto + recordRecuperado.minutos;


                    if(suma.minuto >= 60) {
                        suma.hora += suma.minuto / 60;
                        suma.minuto %= 60;
                    }

                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }

                    recordRecuperado.recordarHora = suma.hora;          
                    recordRecuperado.recordarMin = suma.minuto;
                    recordRecuperado.recordarDia = horario.dia;
                    recordRecuperado.recordarMes = horario.mes;
                    recordRecuperado.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                               
                    fwrite(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");
                break;

                case 3:
                    printf("Cada cuantos minutos: ");
                    scanf("%d%*c", &recordRecuperado.minutos);

                    
                    //Definir Limite de repeticiones del recordatorio

                    horaAMin = recordRecuperado.horas*60;
                    diaAMin = recordRecuperado.dias*1440;

                    lim = diaAMin/(recordRecuperado.minutos + horaAMin);

                    recordRecuperado.limite = lim;

                    //solucionar el problema de formato 24 horas cuando sumo horas

                    suma.hora = horario.hora + recordRecuperado.horas;
                    suma.minuto = horario.minuto + recordRecuperado.minutos;


                    if(suma.minuto >= 60) {
                        suma.hora += suma.minuto / 60;
                        suma.minuto %= 60;
                    }

                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }
                    recordRecuperado.estado = 1;
                    recordRecuperado.recordarHora = suma.hora;          
                    recordRecuperado.recordarMin = suma.minuto;
                    recordRecuperado.recordarDia = horario.dia;
                    recordRecuperado.recordarMes = horario.mes;
                    recordRecuperado.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                               
                    fwrite(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");
                break;

                case 4:
                    printf("Por cuantos dias: ");
                    scanf("%d%*c", &recordRecuperado.dias);

                    
                    //Definir Limite de repeticiones del recordatorio

                    horaAMin = recordRecuperado.horas*60;
                    diaAMin = recordRecuperado.dias*1440;

                    lim = diaAMin/(recordRecuperado.minutos + horaAMin);

                    recordRecuperado.limite = lim;

                    //solucionar el problema de formato 24 horas cuando sumo horas

                    suma.hora = horario.hora + recordRecuperado.horas;
                    suma.minuto = horario.minuto + recordRecuperado.minutos;


                    if(suma.minuto >= 60) {
                        suma.hora += suma.minuto / 60;
                        suma.minuto %= 60;
                    }

                    if(suma.hora >= 24) {
                        suma.hora %= 24;
                    }
                    recordRecuperado.estado = 1;
                    recordRecuperado.recordarHora = suma.hora;          
                    recordRecuperado.recordarMin = suma.minuto;
                    recordRecuperado.recordarDia = horario.dia;
                    recordRecuperado.recordarMes = horario.mes;
                    recordRecuperado.recordarAnio = horario.anio;

                    fseek(recordatorios, -196, SEEK_CUR);                               
                    fwrite(&recordRecuperado, sizeof(struct infoRecordatorios), 1, recordatorios);
                    printf("Recordatorio Modificado con exito !!!\n");
                break;
            }
        }
    }

    fclose(recordatorios);

    if(existe == 0){
        printf("\nNo existe un recordatorio con ese id!!!\n");
    }
}

void eliminarRecordatorio(FILE *recordatorios){

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
       
        if(strcmp(buscado,recordatorio.id) == 0){
            encontrado = 1;
        }else{
           
            if(!feof(recordatorios)){
                fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, temporal);
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

        //hacer que encuentre unicamente los recordatorios del administrador buscado en el archivo recordatorios (info_recordatorios.bin)

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
        printf("11.- Resetear Recordatorios vencidos\n");
        printf("12.- Simular notificacion\n");
        printf("0.- Salir\n\n");

        printf("Seleccione una opcion: ");
        scanf("%d%*c", &opc);

        switch(opc){

            case 1:
                registrarPaciente(pacientes, admin);
            break;

            case 2:
                editarPaciente(pacientes);
            break;

            case 3:
                eliminarPaciente(pacientes);
            break;

            case 4:
                consultarPaciente(pacientes);
            break;

            case 5:
                consultarRecordatoriosPaciente(recordatorios, pacientes);
            break;

            case 6:
                crearRecordatorioPaciente(recordatorios, medicamentos);
            break;

            case 7:
                //pendiente hasta arreglar el de administradores
            break;

            case 8:
                eliminarRecordatorio(recordatorios);
            break;

            case 9:
                mostrarPacientesYRecordatorios(pacientes, recordatorios);
            break;

            case 10:
                mostrarRecordatoriosVencidosPacientes(pacientes, recordatorios);
            break;

            case 11:
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

void editarPaciente(FILE *pacientes){

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
        if(strcmp(id,paciente.id) == 0){ 

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
        printf("No se ha podido encontrar el usuario");
    }
    fclose(pacientes);
}

void eliminarPaciente(FILE *pacientes){

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
       
        if(strcmp(buscado,paciente.id) == 0){
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
        printf("No se ha podido encontrar a el usuario");
    } 
}

void consultarPaciente(FILE *pacientes){

    struct infoPacientes paciente;
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

        printf("\nNombre: %s\n", paciente.nombre);
        printf("Edad: %d\n", paciente.edad);
        printf("ID: %s\n", paciente.id);

    }else{
        printf("No se ha podido encontrar el usuario");
    }

    fclose(pacientes);

}

void consultarRecordatoriosPaciente(FILE *pacientes, FILE *recordatorios){

    struct infoPacientes paciente;
    struct infoRecordatorios recordatorio;
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
        printf("No se ha podido encontrar el usuario");
    }   

}

void crearRecordatorioPaciente(FILE *recordatorios, FILE *medicamentos){

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


    char id[10];
    
    printf("Ingrese el id del paciente al cual se le asignara el recordatorio: ");
    scanf("%[^\n]%*c", id);

//1.- generar id de recordatorio, 2.- el id sera conformado por el id del paciente + un numero aleatorio de 3 digitos, 3.- verificar que el id no este duplicado en el archivo recordatorios (info_recordatorios.bin), si es asi hay que generar otro id

    srand(time(NULL)); // Inicializar la semilla del generador de numeros aleatorios

    do{
        int num1 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num2 = rand() % 10; // Generar un numero aleatorio entre 0 y 9
        int num3 = rand() % 10; // Generar un numero aleatorio entre 0 y 9

        sprintf(recordatorio.id, "%s%d%d%d", id, num1, num2, num3); // Crear el ID del recordatorio

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

    printf("Cada cuantas horas: ");
    scanf("%d%*c", &recordatorio.horas);

    printf("Cada cuantos minutos: ");
    scanf("%d%*c", &recordatorio.minutos);

    printf("Por cuantos dias: ");
    scanf("%d%*c", &recordatorio.dias);

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

//editar recordatorio ya esta echa

void mostrarPacientesYRecordatorios(FILE *pacientes, FILE *recordatorios){

    struct infoPacientes pacienteR;
    struct infoRecordatorios recordatorio;
    int recorEnc;

    pacientes = fopen("info_pacientes.bin","rb");
    
    recordatorios = fopen("info_recordatorios.bin","rb");

    do{

        fread(&pacienteR, sizeof(pacienteR), 1, pacientes);

        if(!feof(pacientes)){


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

                    if(contador == tam && recordatorio.estado == 1){

                        printf("Id: %s\n", recordatorio.id);
                        
                        recorEnc = 1;

                    }

                }
                
            }while(!feof(recordatorios));

            if(recorEnc == 0){

                printf("No hay recordatorios registrados para este paciente\n\n");

            }
        
        }

    }while(!feof(pacientes));

    fclose(pacientes);

    fclose(recordatorios);

}   

void mostrarRecordatoriosVencidosPacientes(FILE *pacientes, FILE *recordatorios){

    struct infoPacientes pacienteR;
    struct infoRecordatorios recordatorio;
    int recorEnc;

    pacientes = fopen("info_pacientes.bin","rb");
    
    recordatorios = fopen("info_recordatorios.bin","rb");

    do{

        fread(&pacienteR, sizeof(pacienteR), 1, pacientes);

        if(!feof(pacientes)){


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

    }while(!feof(pacientes));

    fclose(pacientes);

    fclose(recordatorios);

}   

/*
void eliminarRecordatoriosVencidosPacientes(FILE *recordatorios, FILE *pacientes){

//eliminar todos los recordatorios que esten en estado 0 y lo que su id inicien con las el id del admin y lo que no se elimine guardarlo en un archivo temporal y luego renombrarlo

    struct infoRecordatorios recordatorio;
    struct infoPacientes paciente;

    recordatorios = fopen("info_recordatorios.bin","rb+");
    pacientes = fopen("info_pacientes.bin","rb+");

    //Archivo temporal
    FILE *temporal;
    temporal = fopen("temp.bin","rb+");

    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
    }

//combinar el codigo de mostrar recordatorios vencidos pacientes y eliminar recordatorios vencidos admin-----------------------------------------

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
0
    if(remove(orig) == 0){

        if(rename(temp, orig) == 0){
        }else{
            perror("Error al renombrar archivo");
        }
    }else{
        perror("Error al eliminar original");
    }
}



*/





//hacer que sirva para ambos tipos de usuarios (administrador y paciente)
void simularNotificacion(FILE *recordatorios) {

    //recorrer el archivo recordatorios (info_recordatorios.bin) y mostrar los recordatorios que tengan el estado en 1 y que la fecha y hora actual sea igual a la fecha y hora del recordatorio, cuando se encuente un recordatorio, hay que mostrarlo en pantalla, actualizar el estado del recordatorio a 0, actualizar la fecha y hora del recordatorio para que se muestre el proximo recordatorio y actualizar el contador del recordatorio

    struct infoRecordatorios recordatorio;

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

    recordatorios = fopen("info_recordatorios.bin", "rb+");
    
    if (recordatorios == NULL) {
        // Manejar el error de apertura del archivo
        perror("Error al abrir el archivo");
        return;
    }


    do{
        fread(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

        if (recordatorio.estado == 1 && recordatorio.recordarHora == horario.hora &&
            recordatorio.recordarMin == horario.minuto && recordatorio.recordarDia == horario.dia &&
            recordatorio.recordarMes == horario.mes && recordatorio.recordarAnio == horario.anio) {

            printf("------------------------------------------\n");
            printf("Hola %s, es hora de  %s\n", recordatorio.id, recordatorio.recordatorio);
            printf("------------------------------------------\n\n");

            printf("cont: %d\n", recordatorio.cont);
            printf("limite: %d\n", recordatorio.limite);
            printf("estado: %d\n", recordatorio.estado);

            if (recordatorio.cont == recordatorio.limite) {
                recordatorio.estado = 0;

                fseek(recordatorios, -196, SEEK_CUR);
                fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);

                printf("cont: %d\n", recordatorio.cont);
                printf("limite: %d\n", recordatorio.limite);
                printf("estado: %d\n", recordatorio.estado);
            } else {
                        
                recordatorio.recordarHora = recordatorio.recordarHora + recordatorio.horas;
                recordatorio.recordarMin = recordatorio.recordarMin + recordatorio.minutos;

                    if(recordatorio.recordarMin >= 60) {
                        recordatorio.recordarHora += recordatorio.recordarMin / 60;
                        recordatorio.recordarMin %= 60;
                    }

                    if(recordatorio.recordarHora >= 24){
                        recordatorio.recordarHora %= 24;
                    }

                    recordatorio.cont++;

                    printf("proximo recordatorio: %d:%d\n", recordatorio.recordarHora, recordatorio.recordarMin);                
                    fseek(recordatorios, -196, SEEK_CUR);
                    fwrite(&recordatorio, sizeof(struct infoRecordatorios), 1, recordatorios);
            }
            break;
        }

    }while(!feof(recordatorios));

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
