#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// --- Constantes de configuración ---
#define puerto_turno_matrimonio 100     // Puerto para turnos de matrimonio
#define puerto_informacion_nacimiento 101
#define puerto_turno_nacimiento 102
#define puerto_turno_patente_auto 103
#define puerto_turno_transferencia_auto 104
#define puerto_informacion_patentes 105

#define myport 666                      // Puerto general
#define backlog 66                      // Máximo de conexiones pendientes
#define MAX_BUFFER 1024                // Tamaño del buffer para mensajes
#define MAX_INFO 10                    // Cantidad máxima de estructuras
#define TAM_NOMBRE 50                 // Tamaño máximo para nombres
#define TAM_FECHA 11                  // Tamaño máximo para fechas (incluye '\0')

// --- Estructuras de datos ---

struct turnos_registro_nacimiento {
    char fecha[TAM_FECHA];
    int numero_turno;
};

struct turno_patentar_auto {
    char fecha[TAM_FECHA];
    int numero_turno;
};

struct turno_matrimonio_registrado { 
    char nombre1[TAM_NOMBRE];
    char nombre2[TAM_NOMBRE];
    char fecha[TAM_FECHA];
    int turno;
};
struct turno_transferencia_auto { 
    char nombre_duenio[TAM_NOMBRE];
    char patente[TAM_NOMBRE];
    char fecha[TAM_FECHA];
    int turno;
};

struct solicitud_turno_matrimonio {
    char nombre1[TAM_NOMBRE];
    char nombre2[TAM_NOMBRE];
};

struct solicitud_transferencia_vehiculo {
    char nombre_duenio[TAM_NOMBRE];
    char patente[TAM_NOMBRE];
};

struct informacion_nacimiento {
    char nombre[TAM_NOMBRE];
    char fecha[TAM_FECHA];
    char sexo;
    char documento[TAM_FECHA];  // O cambiar TAM_FECHA por TAM_DOC si es necesario
};

struct informacion_patentes{
	char nombre[TAM_NOMBRE];
	char patente[TAM_FECHA];
	char modelo[TAM_NOMBRE];
	char nro_chasis[TAM_NOMBRE];
};

#endif // ESTRUCTURAS_H
