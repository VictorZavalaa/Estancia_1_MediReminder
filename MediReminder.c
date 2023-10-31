#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct infoRecordatorios{
    char id[10];
    int numRecor;
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
    char id[20];
    char nombre[50];
    int edad;
};
struct infoAdministradores{
    struct infoPacientes informacionBasica;
    char usuario[50];
    char clave[10];
};


void inicioDeSeccion(struct infoAdministradores administrador, FILE*);
void registroDeAdministrador(struct infoAdministradores administrador, FILE*);
void menu(struct infoAdministradores admin);
void menuAdministrador();
void menuPacientes(FILE*, FILE*, struct infoAdministradores admin, FILE*);
void registrarPaciente(FILE*, struct infoAdministradores admin);
void editarPaciente(FILE*);
void elimiarPaciente(FILE*);
void consultarPaciente(FILE*);
void crearRecordatorio(FILE*, struct infoAdministradores admin, FILE*);


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
        printf("2.- Registrarme\n\n");
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

void inicioDeSeccion(struct infoAdministradores administrador, FILE *administradores){

    struct infoAdministradores temp; //considerar pasar este en vez del todo el archivo

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

        //verificar existencia de usuario en archivo administradores (evitar usuarios repetidos)

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
                menuAdministrador();
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

void menuAdministrador(){

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
        printf("6.- Resetear mis recordatorios vencidos\n");
        printf("0.- Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d%*c", &opc);
        
        switch(opc){
            
            case 1:
                
            break;
            
            case 2:
                
            break;
            
            case 3:
                
            break;

            case 4:
            break;

            case 5:
            break;

            case 6:
            break;

            case 0:
                salir = 1;
            break;       
        }
        
    }while(salir == 0);

}

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
                elimiarPaciente(pacientes);
            break;

            case 4:
                consultarPaciente(pacientes);
            break;

            case 5:
            break;

            case 6:
                crearRecordatorio(recordatorios, admin, medicamentos);
            break;

            case 7:
            break;

            case 8:
            break;

            case 9:
            break;

            case 10:
            break;

            case 11:
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
    int numero = 1;
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
                
                fseek(pacientes, -76, SEEK_CUR);                                    // Retrocede el tamanio de la estuctura leida
                fwrite(&paciente, sizeof(struct infoPacientes), 1, pacientes);      //Sobre escribe la nueva estructura en la otra
                printf("Nombre actualizado!!!\n");

            }else if(opc == 2){
                printf("Ingrese la nueva edad que desea asignar al paciente: ");
                scanf("%d", &paciente.edad);

                fseek(pacientes, -76, SEEK_CUR);                                    // Retrocede el tamanio de la estuctura leida
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

void elimiarPaciente(FILE *pacientes){

    struct infoPacientes paciente;

    char buscado[10];
    int encontrado = 0;
   

    //Archivo temporal
    FILE *temporal;
    temporal = fopen("temp.bin","rb+");

   
    if(temporal == NULL){
        temporal = fopen("temp.bin","wb");
        fclose(temporal);
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

void crearRecordatorio(FILE *recordatorios, struct infoAdministradores admin, FILE *medicamentos){

    struct infoRecordatorios recordRecuperado;
    struct infoRecordatorios recordatorio;
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

    printf("Ingrese el id del usuario al cual se le asignara el recordatorio: ");
    scanf("%[^\n]%*c", recordatorio.id);

    printf("Que necesitas recordar: ");
    scanf("%[^\n]%*c", recordatorio.recordatorio);

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
    printf("No. de recordatorio: %d\n", recordatorio.numRecor);
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