# PeXis ZeroUno

Firmware de competición para un robot siguelíneas basado en el **PX-01 de OPRobots**, con 16 sensores infrarrojos, dos motores de tracción y ventilador de succión. Usa una **Seeed Studio XIAO ESP32-C3**, el framework Arduino y PlatformIO.

## Historia y autoría

El punto de partida es el proyecto abierto [OPRobots/PX-01](https://github.com/OPRobots/PX-01), concretamente su firmware [PX-01 Pro & OPR Style](https://github.com/OPRobots/PX-01/tree/main/software/PX-01%20Pro%20%26%20OPR%20Style). Los diseños de electrónica, modelos 3D y documentación del robot base están en ese repositorio.

Aitor (software) y Alberto (hardware) desarrollaron y ajustaron juntos la versión utilizada en **OSHWDEM 2025, donde el robot quedó segundo**. El repositorio original es [AiToR51/px01_aitor, en GitLab](https://gitlab.com/AiToR51/px01_aitor).

PeXis ZeroUno es una adaptación para la **OSHWDEM 2026**.

Se han modificado las ganancias de control por perfil, la lectura con un segundo umbral apoyado en sensores vecinos y los ajustes de velocidad, succión y tiempos. Los parámetros actuales pueden seguir evolucionando; la referencia para compilar es siempre el contenido de este repositorio.

## Cargar el código: guía rápida para Windows

Este es el procedimiento utilizado para cargar correctamente el firmware en, por ejemplo, **COM4**. Ejecuta los comandos en **PowerShell**.

### 1. Abrir el proyecto y localizar PlatformIO

```powershell
Set-Location '\GitHub\PeXis ZeroUno'
$pio = Join-Path $env:USERPROFILE '.platformio\penv\Scripts\platformio.exe'
& $pio --version
```

La variable `$pio` dura hasta cerrar esa sesión de PowerShell. La ruta corresponde a la instalación habitual de PlatformIO Core en Windows y permite usarlo aunque `pio` no esté en el `PATH`. Si el proyecto está en otro directorio, adapta `Set-Location`.

Si el ejecutable no existe, instala PlatformIO como se explica en [Preparar el entorno](#preparar-el-entorno).

### 2. Conectar la placa y comprobar el puerto

Conecta la XIAO con un cable USB de datos y cierra cualquier monitor serie abierto en IntelliJ, Arduino IDE, VS Code u otra terminal.

```powershell
& $pio device list
```

En el equipo utilizado, la placa aparece como `Dispositivo serie USB (COM4)`, con identificador USB `303A:1001`. El número de COM puede cambiar al reconectar o usar otro ordenador: usa el que corresponda a la XIAO, no el de otro dispositivo USB.

### 3. Compilar y cargar

Para compilar sin escribir en la placa:

```powershell
& $pio run -e PX-01
```

Para compilar y cargar en COM4:

```powershell
& $pio run -e PX-01 -t upload --upload-port COM4
```

PlatformIO descarga las herramientas y librerías que falten, compila y escribe el firmware. La primera compilación necesita acceso a Internet y puede tardar más. Una carga correcta termina con `Hash of data verified` y `[SUCCESS]`; después intenta reiniciar la placa. Si permanece en modo de carga, suelta BOOT y pulsa RESET una vez.

El binario de aplicación generado queda en `.pio/build/PX-01/firmware.bin`. Para cargar, usa el comando de PlatformIO: también escribe el bootloader y la tabla de particiones necesarios.

### 4. Si COM4 falla: BOOT y RESET

Si aparece `Could not open COM4`, `Cannot configure port` o el error de Windows «Uno de los dispositivos conectados al sistema no funciona», sigue esta secuencia:

1. Cierra los monitores serie que puedan tener abierto el puerto.
2. Con la placa conectada, mantén pulsado **BOOT**.
3. Pulsa y suelta **RESET** sin soltar BOOT.
4. Suelta **BOOT**: la placa queda preparada para recibir firmware.
5. Ejecuta `& $pio device list` y repite la carga con el puerto que aparezca.

Otra forma de entrar en modo de carga es desconectar el USB, mantener BOOT pulsado mientras vuelves a conectarlo y soltarlo después. Si el puerto sigue sin funcionar, prueba otro cable de datos o puerto USB. Esta secuencia permitió completar la carga de este proyecto después de los fallos iniciales de COM4. Véase también la [guía de recuperación de Seeed](https://wiki.seeedstudio.com/noport_upload_fails/).

**BOOT y RESET son botones de la XIAO. El botón de uso del robot es BTN_1; tienen funciones distintas.**

### 5. Monitor serie opcional

```powershell
& $pio device monitor --port COM4 --baud 115200
```

Sal con **Ctrl+C** antes de volver a cargar código. El firmware imprime `Calibrations:` y una fila por sensor con mínimo, máximo y umbral al terminar la calibración. No emite telemetría continua durante la marcha en su configuración normal.

Referencia de opciones: [monitor serie de PlatformIO](https://docs.platformio.org/en/latest/core/userguide/device/cmd_monitor.html).

## Preparar el entorno

Necesitas el robot con una XIAO ESP32-C3, su alimentación de funcionamiento, un cable USB de datos y PlatformIO Core. Un editor sirve para modificar el código; la compilación y carga pueden hacerse íntegramente por CLI.

Para instalar Core en otro equipo, sigue el [instalador oficial de PlatformIO](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html): instala Python, descarga `get-platformio.py` desde esa página y ejecútalo con Python desde el directorio donde lo hayas guardado:

```powershell
python get-platformio.py
```

Después vuelve a localizar el ejecutable con `$pio` como en la guía rápida. Si ya tienes los comandos de PlatformIO en el `PATH`, sus equivalentes son:

```powershell
pio device list
pio run -e PX-01
pio run -e PX-01 -t upload --upload-port COM4
pio device monitor --port COM4 --baud 115200
```

### Editores

- **IntelliJ IDEA:** puedes editar el proyecto y ejecutar los comandos desde su terminal. [Flexible for Arduino](https://github.com/ilscipio/flexible-for-arduino-jetbrains-plugin) anuncia un backend PlatformIO; esa integración depende de la versión y configuración del plugin. La carga comprobada aquí se hizo por CLI.
- **VS Code:** puedes abrir la raíz del proyecto con la extensión PlatformIO IDE y seleccionar el entorno `PX-01` para compilar o cargar.
- **Arduino IDE / Arduino CLI:** el código usa el framework Arduino, pero el repositorio está organizado como proyecto PlatformIO (`src/`, `include/`, `platformio.ini`). Usar otro sistema de compilación requiere adaptar su estructura y configuración.

### Configuración y versiones comprobadas

`platformio.ini` define el entorno `PX-01`, la placa `seeed_xiao_esp32c3`, la plataforma `espressif32`, el framework `arduino`, el flag `PIO_FRAMEWORK_ARDUINO_ENABLE_CDC` y Adafruit NeoPixel `^1.12.0`. El monitor funciona a 115200 baudios.

La compilación y carga del **3 de octubre de 2026** se completaron con:

| Componente | Versión usada |
| --- | --- |
| PlatformIO Core | 6.1.18 |
| Plataforma Espressif 32 | 6.12.0 |
| Arduino-ESP32 | 2.0.17; paquete `3.20017.241212+sha.dcc1105b` |
| Adafruit NeoPixel | 1.15.5 |
| esptool.py | 4.9.0 |

El firmware utilizó 14380 bytes de RAM (4,4 %) y 258290 bytes del espacio de aplicación Flash (19,7 %). La carga verificó los datos escritos; esa comprobación no sustituye la prueba del robot en pista.

La plataforma y las dependencias no están fijadas a todas esas versiones en `platformio.ini`. Una instalación futura podría resolver versiones diferentes. `src/motors.cpp` utiliza `ledcSetup` y `ledcAttachPin`, de Arduino-ESP32 2.x: cambiar al core 3.x exige adaptar esas llamadas, como explica la [guía de migración de Espressif](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html).

## Cómo usar el robot

La secuencia de uso es **encender → calibrar → elegir perfil → iniciar → detenerse**. Se hace con **BTN_1**, el botón de control del robot. El código reconoce una pulsación corta de menos de 250 ms y una larga de al menos 250 ms; las acciones se ejecutan al soltar el botón.

La configuración actual sigue una **línea negra** y usa **modo prueba**, con parada automática a los **8 segundos de marcha**.

### 1. Encender y calibrar los 16 sensores

1. Alimenta el robot y sitúa los sensores a su altura normal de trabajo sobre la pista.
2. Los LED muestran naranja y el superior parpadea: está esperando iniciar la calibración.
3. Haz una **pulsación corta** de BTN_1.
4. Durante los siguientes **4 segundos**, desplaza manualmente el robot sobre la línea y el fondo, de lado a lado, para que **cada uno de los 16 sensores** vea ambos. El LED superior muestra un efecto arcoíris.
5. Al terminar, el LED superior se ilumina **verde durante 1 segundo** si todos los sensores han registrado suficiente contraste. Después pasa al menú de perfiles.

El criterio de contraste es un rango de al menos 1000 unidades entre mínimo y máximo para cada sensor, sobre lecturas de 0 a 4095.

Si el LED superior **parpadea en rojo**, algún sensor no ha alcanzado ese contraste. Revisa la altura, la limpieza y que todos hayan cruzado línea y fondo; pulsa RESET y repite la calibración. El firmware permite continuar con una pulsación larga y suelta, pero eso **acepta la calibración insuficiente**, no la repite.

La calibración se guarda en RAM: hay que repetirla después de apagar o reiniciar. Para calibrar durante el uso normal no necesitas tener el ordenador conectado.

### 2. Elegir uno de los tres perfiles

Después de calibrar queda seleccionado el **perfil 1**. Cada pulsación corta avanza **1 → 2 → 3 → 1**. Un LED azul indica la selección.

Valores actuales de `include/config.h`:

| Perfil | LED azul | Velocidad base (%) | Aceleración (puntos porcentuales/s) | Succión (%) | Kp | Kd |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | Izquierdo | 55 | 60 | 65 | 0.20 | 4.5 |
| 2 | Superior | 70 | 60 | 85 | 0.22 | 6.0 |
| 3 | Derecho | 80 | 60 | 100 | 0.24 | 6.5 |

La velocidad es una consigna de PWM; no representa una velocidad medida en m/s. El control suma o resta la corrección a cada motor y limita su consigna al rango de −100 a 100 %. La succión es una consigna para el ESC del ventilador.

### 3. Iniciar la marcha

1. Coloca el robot sobre la línea, orientado en el sentido de la pista, y comprueba el perfil elegido.
2. Mantén BTN_1 pulsado al menos 250 ms; una pulsación de aproximadamente medio segundo permite superar el umbral. Los LED azules parpadean mientras lo mantienes pulsado.
3. **Suelta el botón** para iniciar la cuenta atrás.
4. En modo prueba la cuenta atrás dura **3 segundos**; en modo carrera, **5 segundos**. Los últimos tres segundos se indican con tres, dos y un LED azul.
5. Durante el **último segundo** se activa el ventilador al 85 % de la consigna del perfil y los motores ya pueden corregir la orientación. Al acabar la cuenta atrás comienza el avance y la rampa de velocidad.

Deja el robot colocado y libre para moverse antes del último segundo de la cuenta atrás. El pin `MOD_START` está configurado como entrada, pero el firmware actual no lo lee para arrancar: la salida se hace con BTN_1.

### 4. Parada y nueva salida

- **Modo prueba actual:** corta motores y succión al alcanzar 8 segundos desde el inicio del avance. La cuenta atrás no se incluye en esos 8 segundos.
- **Pérdida de línea:** durante la marcha detiene motores y succión si pasan más de 250 ms sin una detección válida. Cero sensores activos o los 16 activos no renuevan esa detección.
- **Modo carrera:** no tiene el límite de 8 segundos; sigue hasta perder la línea o cortar la alimentación.

BTN_1 no se consulta como botón de parada mientras el robot corre. Para interrumpir una marcha de forma inmediata, usa el corte de alimentación del robot. Tras una parada automática vuelve al menú y conserva el perfil y la calibración; puedes recolocarlo y lanzar otra salida con una pulsación larga.

### Resumen de indicadores

| Indicador | Significado / acción |
| --- | --- |
| Naranja, superior parpadeando | Espera calibración; pulsación corta |
| Arcoíris en LED superior | Calibrando durante 4 segundos |
| Verde en LED superior | Contraste suficiente en todos los sensores |
| Rojo intermitente en LED superior | Calibración insuficiente; reiniciar y repetir |
| Un LED azul | Perfil seleccionado: izquierdo 1, superior 2, derecho 3 |
| Tres LED azules parpadeando al mantener BTN_1 | Pulsación larga; soltar para iniciar |
| Tres, dos y un LED azul | Últimos tres segundos de la cuenta atrás |

## Ajustar el firmware

Los ajustes de uso están en `include/config.h`. Después de modificarlos, vuelve a compilar y cargar: cambiar el archivo en el PC no cambia el firmware que ya está en la placa.

### Prueba y carrera

Para pruebas con parada temporizada:

```cpp
#define MODE_TEST 1
// #define MODE_RACE 2
```

Para carrera sin límite de tiempo:

```cpp
// #define MODE_TEST 1
#define MODE_RACE 2
```

Activa **solo uno**. Son macros que el código comprueba con `#ifdef`: poner `MODE_TEST` a `0` no lo desactiva. Si defines ambos, tendrás cuenta atrás de carrera y parada temporizada de prueba.

### Color de la línea

Configuración actual, línea negra:

```cpp
// #define LINE_WHITE 0
#define LINE_BLACK 1
```

Para línea blanca:

```cpp
#define LINE_WHITE 0
// #define LINE_BLACK 1
```

La inversión de lecturas depende de que `LINE_BLACK` esté definido; el `0` de `LINE_WHITE` no significa desactivado. Vuelve a calibrar en la pista correspondiente después de cargar el cambio.

### Tiempos y perfiles

| Parámetro | Valor actual | Función |
| --- | --- | --- |
| `START_TEST_MS` | 3000 ms | Cuenta atrás de prueba |
| `START_RACE_MS` | 5000 ms | Cuenta atrás de carrera |
| `TEST_DURATION_MS` | 8000 ms | Duración máxima de marcha en prueba |
| `SENSORS_CALIBRATION_MS` | 4000 ms | Tiempo para barrer línea y fondo |
| `CONTROL_LOOP_US` | 750 µs | Intervalo mínimo entre cálculos de control |
| `MENU_n_BASE_SPEED` | Según perfil | Consigna base de tracción |
| `MENU_n_ACCEL_SPEED` | Según perfil | Pendiente de la rampa inicial |
| `MENU_n_BASE_FAN_SPEED` | Según perfil | Consigna de succión, de 0 a 100 |
| `MENU_n_PID_KP` / `MENU_n_PID_KD` | Según perfil | Ganancias proporcional y derivativa |

Aunque los nombres y comentarios hablan de PID, la corrección implementada es **PD**, sin término integral:

```text
corrección = Kp × error + Kd × (error − error_anterior)
motor izquierdo = velocidad + corrección
motor derecho   = velocidad − corrección
```

La posición se estima con la media ponderada de los 16 sensores y se expresa entre −255 y 255, con el centro en 0. El umbral principal de cada sensor se sitúa a dos tercios de su rango calibrado. Una lectura que supera el umbral secundario, a mitad del rango, también contribuye si un sensor vecino supera el principal. La lectura de sensores se refresca aproximadamente cada 1 ms.

La diferencia derivativa no se divide por el tiempo entre muestras, de modo que cambiar `CONTROL_LOOP_US` también puede cambiar el comportamiento del control. Ajusta los perfiles en pista, empezando por el de menor velocidad y modificando un parámetro cada vez. Usa literales válidos de C++, como `6.0f`; `6f` no compila.

## Estructura del código y hardware

| Archivo / directorio | Contenido |
| --- | --- |
| `platformio.ini` | Placa, framework, dependencias y opciones de compilación |
| `include/config.h` | Modos, tiempos y parámetros de los tres perfiles |
| `src/config.cpp` | Inicialización de pines y selección del tiempo de salida |
| `src/main.cpp` | Calibración inicial, cuenta atrás y ciclo principal |
| `src/menu.cpp` | Selección de perfil y LED correspondiente |
| `src/control.cpp` | Corrección PD, rampa y parada por pérdida de línea |
| `src/sensors.cpp` / `include/sensors.h` | Lectura, calibración, umbrales y posición |
| `src/motors.cpp` / `include/motors.h` | PWM de motores y señal del ESC de succión |
| `src/utils.cpp` / `include/utils.h` | NeoPixel y detección de pulsaciones |
| `include/pinout.h` | Asignación de GPIO |
| `.pio/` | Dependencias y resultados de compilación generados por PlatformIO |
| `LICENSE` | Licencia del proyecto |

### Pines usados

Estos números son **GPIO del ESP32-C3**, no números de etiqueta `D` de la placa. La referencia del cableado del robot base está en [el hardware de OPRobots](https://github.com/OPRobots/PX-01/tree/main/hardware).

| Señal | GPIO |
| --- | --- |
| Cadena de tres NeoPixel | 20 |
| BTN_1, control del robot | 6 |
| MOD_START, entrada configurada sin uso de arranque | 21 |
| SENSOR_1 / SENSOR_2 | 1 / 0 |
| MUX_A / MUX_B / MUX_C | 9 / 4 / 3 |
| MOTOR_RIGHT_A / MOTOR_RIGHT_B | 2 / 8 |
| MOTOR_LEFT_A / MOTOR_LEFT_B | 7 / 10 |
| MOTOR_SUCTION | 5 |

La tracción usa PWM a 1 kHz y resolución de 10 bits. La succión usa una señal de 50 Hz y resolución de 11 bits, aproximadamente de 1000 a 2000 µs, para el ESC. El código inicializa el ESC con la consigna mínima; su espera adicional de 5 segundos está comentada. La fase de calibración deja tiempo antes de la salida.

Este repositorio contiene el firmware. Para planos de fabricación, modelos 3D y especificaciones de alimentación del robot base, consulta [OPRobots/PX-01](https://github.com/OPRobots/PX-01); comprueba las características de los componentes montados en tu unidad.

## Diagnóstico rápido

| Problema | Qué comprobar |
| --- | --- |
| `pio` no se reconoce | Usa la ruta del ejecutable mediante `$pio` o configura el `PATH` |
| Puerto ocupado o error 31 al abrir COM4 | Cierra monitores, entra con BOOT/RESET, vuelve a listar puertos y prueba otro cable/USB |
| La carga queda en `Connecting...` | Pon la XIAO en modo de carga con BOOT/RESET y confirma el puerto |
| Error de compilación con `operator""f` | Revisa los literales: `6.0f`, no `6f` |
| Faltan `ledcSetup` / `ledcAttachPin` | Comprueba la versión de Arduino-ESP32; este código usa la API 2.x |
| Naranja y no pasa al menú | Falta una pulsación corta para comenzar la calibración |
| Rojo después de calibrar | Todos los sensores deben ver línea y fondo; revisa altura y repite con RESET |
| Arranca cuando querías cambiar perfil | Has superado 250 ms; el cambio de perfil exige una pulsación corta |
| Se detiene a los 8 segundos | Está activo `MODE_TEST`; cambia a `MODE_RACE` y vuelve a cargar para carrera |
| Se detiene antes | Revisa calibración, color de línea y pérdida de detección durante más de 250 ms |
| No aparecen lecturas continuas por serie | Solo imprime calibración por defecto; `debug_sensors()` está comentado en `setup()` |

Para inspeccionar sensores puedes activar temporalmente `debug_sensors()` en `src/main.cpp` y volver a cargar. Esa función contiene un bucle infinito: imprime posición y lecturas cada 500 ms y **no llega al menú ni a la marcha**. Coméntala de nuevo para recuperar el funcionamiento normal.

## Créditos y licencia

- **OPRobots:** proyecto PX-01, hardware y firmware de partida.
- **Aitor (AiToR51) y Alberto Ferreiro Campello (Kampy):** desarrollo y puesta a punto de la versión que obtuvo el segundo puesto en OSHWDEM 2025.
- **Alberto Ferreiro Campello (Kampy):** continuación PeXis ZeroUno y presentación para OSHWDEM 2026.

Se conserva la licencia **GNU GPL versión 3** incluida en `LICENSE`, heredada del proyecto de partida. Las librerías externas mantienen sus propias licencias.
