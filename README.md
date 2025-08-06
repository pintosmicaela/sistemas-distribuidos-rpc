# Proyecto N° 1
Proyectp de la materia Sistemas Distribuidos que se realizan experimentos y desarrollos prácticos para analizar distintos mecanismos de comunicación y cooperación entre procesos en sistemas distribuidos, evaluando su desempeño y comportamiento.

## Actividades principales:
Medición del tiempo de creación de procesos hijos:

1. Comparar creación local y remota.
   - Medir también con system() ejecutando la misma imagen.
   - Evaluar desempeño en microsegundos.
   - Comparación entre RPC y llamadas locales (LC):

2. Llamadas con entrada de 4096 bytes y respuesta de 2048 bytes.
   - Suma de dos arreglos de 200 elementos.
   - Casos: RPC local, RPC en otra máquina con misma o distinta arquitectura.
   - Medir el tiempo de atención a una solicitud de 4096 bytes con sockets TCP.

3. Implementar un servidor "Mini Servicios":
   - Funciones: nombre de la máquina, fecha del sistema, conversión decimal-binario, suma y multiplicación (hasta 4 operandos).
   - Comunicación:
     - Con IPC (memoria compartida, archivos, colas, etc.).
     - Con RPC.
   - Medir tiempos en micro/nanosegundos y comparar.
4. Comunicación entre agencias A y B:
   - A brinda: turnos para matrimonio, partida de nacimiento e inscripción de bebés.
   - B brinda: patentar vehículos, transferencia y dominio.
   - Comunicación mediante sockets con conexión.

5. Medición de la variación del reloj del cliente respecto al servidor.

6. Sincronización de relojes entre cliente y servidor.

7. Protocolo BitTorrent (teórico):
   - Ventajas y limitaciones tecnológicas.
