# Sistema de control de fructificación de hongos

&emsp;&#x1F4DD;&emsp;**TP Nº 3**

&emsp;&#x1F5D3;&emsp;**Sábado 31 de mayo de 2025**

&emsp;&#x1F464;&emsp;**Gabriel Sosa (87311)**

---

## Objetivo

&emsp;Diseñar e implementar un sistema embebido que permita controlar automáticamente las condiciones de temperatura, humedad y ventilación en una cámara de fructificación de hongos, registrando eventos relevantes y permitiendo interacción con el/la usuario/a a través de un menú en pantalla y teclado matricial.

&emsp;Particularmente en esta entrega se plantearon los siguientes objetivos:

* Incorporar el control de un servomotor usando PWM.
* Comandar un ventilador de 12V mediante un módulo relé.
* Implementar una **ISR** (_Interrupt Service Routine_) forzar el estado de ventilación.
* Incorporar almacenamiento externo mediante un sistema de archivos FAT32 sobre tarjeta SD.

---

## Descripción

### Funcionamiento

> :video_camera: **Video de funcionamiento**: [Sistema de control de fructificación de hongos](https://drive.google.com/file/d/1WG3PtOQO2l4rCSAxCABryTSitVziw2cl/view?usp=sharing)

&emsp;El sistema funciona de forma autónoma y ligada a una **máquina de estados finitos (FSM)** que evalúa periódicamente los sensores conectados. Además, el/la usuario/a puede configurar umbrales de temperatura y humedad, visualizar el estado actual, consultar registros y configurar el reloj interno (RTC) desde un menú interactivo.

---

## Variables controladas

| Variable     | Sensor                     | Actuador                     |
|--------------|----------------------------|------------------------------|
| Temperatura  | Sensor digital DHT22       | Módulo Peltier con cooler    |
| Humedad      | Sensor digital DHT22       | Humidificador ultrasónico    |
| Ventilación  | RTC + temporizador interno o bien ISR | Cooler 12 V + compuerta (servo) |


---

## Estructura general

### Directorios

| Directorio/Archivo                | Contenido principal                                                   |
| --------------------------------- | --------------------------------------------------------------------- |
| `TP3-SE-87311/`                   | Archivos fuente del proyecto (organizados por módulo)                 |
| `TP3-SE-87311/modules/fsm/`       | Lógica de la máquina de estados finita (FSM)                          |
| `TP3-SE-87311/modules/menu/`      | Interfaz de usuario basada en display TFT y teclado matricial         |
| `TP3-SE-87311/modules/display/`   | Funciones gráficas para el controlador ILI9341 (texto y reloj)        |
| `TP3-SE-87311/modules/events/`    | Registro y almacenamiento de eventos con timestamp                    |
| `TP3-SE-87311/modules/sensors/`   | Interfaces con sensores como el DHT22                                 |
| `TP3-SE-87311/modules/actuators/` | Control de relé, ventilador, humidificador y servomotor               |
| `TP3-SE-87311/modules/sdcard/`    | Montaje, desmontaje seguro y escritura de logs en la tarjeta SD       |
| `main.cpp`                        | Bucle principal, inicialización de periféricos y coordinación general |

---

## Implementación

### Máquina de Estados (FSM)

El sistema implementa una **FSM** que evalúa las condiciones cada 2 s y actúa según los siguientes criterios:

| Estado         | Condición de entrada                                   | Acción asociada             | Tiempo de acción |
|----------------|--------------------------------------------------------|-----------------------------|---|
| STAND_BY       | Estado base                                            | Todo apagado                | - |
| HUMIDIFYING    | Humedad menor al umbral configurado                    | Humidificador encendido     | $60\ s$ |
| COOLING        | Temperatura mayor al umbral configurado               | Peltier/cooler encendido    | $60\ s$ |
| VENTILATING    | Cada 5 min mediante el uso de tickers o forzado por medio de ISR | Ventilador encendido       | $30\ s$ |

&emsp;La duración de cada estado es fija, y se retorna automáticamente a `STAND_BY` tras cumplido el tiempo.

&emsp;El siguiente diagrama fue confeccionado con el software en la nube <a href="https://www.itemis.com/en/products/itemis-create/">**Itemis Create**</a>:

![alt text](media/FSM.png)
---

> **NOTA**: varias de las variables que pueden visualizarse en este no compareten el mismo nombre que en el código fuente.

### Interfaz

&emsp;La interacción con el sistema se realiza a través de:

* **Display gráfico TFT ILI9341**: ermite al usuario visualizar las opciones del sistema.
* **Teclado matricial 4x4**: permite al usuario interactuar con la interfaz gráfica.

#### Menú principal:

```
>>>------ Mushroomer Menu ------<<<
1: FSM State
2: Temp & Humidity
3: Set RTC Time
4: Show Current Time
5: Save State to Log
6: View Event Log
A: Set upper temperature threshold
B: Set lower humidity threshold
C: Force ventilation
D: Mount/Unmount SD card
#: Return to Menu

```

---

### Registro de eventos

&emsp;Cada vez que el/la usuario/a elige la opción 5 (o cada cinco minutos automáticamente), se guarda un evento tanto en memoria RAM como en la tarjeta SD (si está montada), en formato **.csv**. Cada evento contiene:

* Estado actual del sistema (FSM)

* Temperatura y humedad medidas

* Timestamp generado con el RTC

&emsp;Desde el menú (opción 6) pueden visualizarse los últimos eventos registrados. Se muestran hasta cinco entradas en pantalla. El archivo de eventos es persistente entre sesiones y puede analizarse externamente mediante herramientas como Excel o Python.

---

### Umbrales configurables

&emsp;Desde el menú principal, se permite configurar:

* Umbral superior de temperatura (opción A): el usuario ingresa dos dígitos (por teclado) representando los °C enteros.

* Umbral inferior de humedad (opción B): ingreso de dos dígitos para el porcentaje de HR mínimo deseado.

&emsp;Los valores ingresados se aplican de inmediato, modificando el comportamiento de la FSM en la siguiente evaluación.

&emsp;Además, para facilitar el diagnóstico visual durante el desarrollo, se usan los LEDs de la NUCLEO-F429ZI como indicadores del estado de los actuadores:

| LED  | Representa                   | Encendido indica que…                  |
| ---- | ---------------------------- | -------------------------------------- |
| LED1 | **Sistema de refrigeración** | El módulo Peltier (cooler) está activo |
| LED2 | **Humidificador**            | El humidificador está encendido        |
| LED3 | **Ventilación**              | El ventilador está funcionando         |


---

## Periféricos utilizados

| Componente        | Función                                        | Conexión                            |
| ----------------- | ---------------------------------------------- | ----------------------------------- |
| Display ILI9341   | Interfaz gráfica (menú, datos, reloj)          | SPI1 (CS, DC, RST, MOSI, SCK)       |
| Teclado 4x4       | Entrada de comandos del usuario                | 8 pines GPIO (4 filas, 4 columnas)  |
| Sensor DHT22      | Medición de temperatura y humedad              | GPIO digital                        |
| Módulo relé       | Control del ventilador 12 V                    | GPIO digital                        |
| Servo SG90        | Apertura de compuerta para ventilación         | PWM                                 |
| Módulo SD card    | Registro persistente de eventos (formato .csv) | SPI4 |


### Funcionalidades implementadas en TP3

| Característica                        | Tema relacionado                               | Capítulo del libro |
| ------------------------------------- | ---------------------------------------------- | ------------------ |
| Ventilación con ventilador 12V y relé | Relés y GPIO                                   | Cap. 7             |
| Forzado de ventilación por tecla      | ISR | Cap. 7             |
| Control de flap mediante servomotor   | PWM                                            | Cap. 8             |
| Invocación a FSM, refresco de display y guardado automático de LOGs  | Tickers                                            | Cap. 8             |
| Manejo de SD            | Filesystem, FAT                                | Cap. 9             |

### Implementación

* **PWM:** se utilizó la clase **PwmOut** de Mbed OS para controlar el ángulo del servomotor SG90. Se emplean ciclos de $50\ Hz$ con anchos de pulso en alto entre $500\ \mu s$ y $2,5\ ms$.
* **Relé:** se controla mediante una salida digital conectada a un GPIO, el cual activa o desactiva la bobina del relé para energizar el ventilador.
* **Ventilación forzada:** se introdujo una interrupción por software a través de polling sobre la tecla 'C' del teclado matricial. Esta acción permite al usuario activar la ventilación cuando lo considere conveniente.
* **SD y sistema de archivos:** se utilizó la clase `SDBlockDevice` junto con `FATFileSystem`. Se implementó una función que verifica la presencia de encabezados en el archivo CSV antes de escribir.
