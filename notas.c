*hacer que el usuario del administrador solo sea de 3 letras para evitar mamadas

196 es el tamaño de recordatorios

... es el tamaño de los pacientes



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
    recordRecuperado.estado = 1;
    recordRecuperado.recordarHora = suma.hora;          
    recordRecuperado.recordarMin = suma.minuto;
    recordRecuperado.recordarDia = horario.dia;
    recordRecuperado.recordarMes = horario.mes;
    recordRecuperado.recordarAnio = horario.anio;










hijjo de su puta madre desde la 2 hasta las 7:50 haciendo lo de simular notificacion alv
