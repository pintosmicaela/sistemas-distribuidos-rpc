# Comunicación entre procesos en sistemas distribuidos (RPC, sockets e IPC)

Primer proyecto de la materia **Sistemas Distribuidos** (UNS, 2025). Reúne experimentos y desarrollos prácticos en **C** para analizar distintos mecanismos de comunicación y cooperación entre procesos, y evaluar su desempeño: llamadas locales, **RPC (ONC/Sun RPC con `rpcgen`)**, **sockets TCP** e **IPC**.

**Stack:** C · ONC RPC (`rpcgen`) · Sockets TCP · IPC · Make · Linux

## Contenido

| Carpeta | Ejercicio | Qué hace |
|---|---|---|
| `ejer1/` | Creación de procesos | Mide, en microsegundos, el tiempo de crear un proceso hijo con `fork` + `execl` y con `system()`. La parte **a** es local y la parte **b** es remota con RPC (`ejercicio1.x`). |
| `ejer2/` | Llamadas RPC | Llamada RPC con una entrada de 4096 bytes y una respuesta de 2048 bytes (`ejercicio2.x`). Las variantes **a**, **b** y **c** corresponden a los distintos escenarios de prueba. |
| `ejer3/` | Sockets TCP | Cliente y servidor con sockets con conexión, para medir el tiempo de atención de una solicitud de 4096 bytes. |
| `ejer4/` | Mini servicios | Servidor con funciones de nombre de la máquina, fecha del sistema, conversión decimal a binario, suma y multiplicación (hasta 4 operandos). La parte **a** usa IPC (memoria compartida) y la **b** usa RPC (`mini_servicios.x`), para comparar tiempos. |
| `ejer5/` | Agencias A y B | Servicios de turnos y trámites (matrimonio, nacimiento, patentamiento y transferencia de vehículos) sobre sockets con conexión. El servidor crea un proceso por servicio, cada uno escuchando en su puerto. |
| `ejer6/` | Reloj cliente-servidor | Medición del desvío del reloj del cliente respecto del servidor, probado con una máquina (`desvio_una_maquina`) y con tres (`desvio_tres_maquinas`). |
| `docs/` | Documentación | Enunciado (`PrimerProyecto-2025.pdf`) e informe (`Informe_.docx`). |

El enunciado también incluye una parte teórica sobre el protocolo BitTorrent (ventajas y limitaciones tecnológicas), que se desarrolla en el informe.

## Conceptos aplicados

- Interfaces RPC definidas en archivos `.x` y generación de stubs con `rpcgen`.
- Programación de sockets TCP (`socket`, `bind`, `listen`, `accept`, `connect`).
- Procesos con `fork`, `execl` y `waitpid`.
- Medición de tiempos con `gettimeofday` y comparación entre llamadas locales, RPC y sockets.
- Ejecución en una sola máquina y en varias.

## Cómo compilar y ejecutar

Requisitos: Linux, `gcc`, `make` y `rpcgen` (en Debian/Ubuntu: paquetes `build-essential`, `rpcsvc-proto` y `libtirpc-dev`).

Cada ejercicio tiene su propio `makefile`:

```bash
cd ejer3
make            # compila cliente y servidor
./servidor      # en una terminal (escucha en el puerto 666)
./cliente <host>  # en otra terminal
make clean
```

Los ejercicios con RPC (`ejer1/b`, `ejer2` y `ejer4/b`) generan el código con `rpcgen` desde el `makefile`. Antes de ejecutar un cliente RPC, el servidor debe estar corriendo y `rpcbind` activo en la máquina.

## Autora

**Micaela Luján Pintos** · [LinkedIn](https://linkedin.com/in/mlpintos) · [GitHub](https://github.com/pintosmicaela)
