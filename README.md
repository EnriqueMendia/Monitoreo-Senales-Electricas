# Proyecto de Monitoreo de Energía con ESP32

Este proyecto está diseñado para monitorear parámetros de energía eléctrica, como corriente, voltaje y temperatura, utilizando un **ESP32** junto con diversos sensores. La información se procesa y se envía a la plataforma **ThingSpeak** para su análisis en tiempo real, y se genera una alerta a través de **Telegram** si se detectan valores fuera de los límites predefinidos. Además, el sistema controla un **relé** para activar o desactivar dispositivos según los parámetros de corriente y voltaje.

## Características

- **Monitoreo de corriente**: Utiliza un sensor **ACS712** para medir la corriente eléctrica en un rango específico.
- **Monitoreo de voltaje**: Se usa un sensor **ZMPT101B** para medir el voltaje de la red eléctrica.
- **Monitoreo de temperatura**: Se implementa un sensor **DS18B20** para medir la temperatura ambiente.
- **Conexión a Internet**: Conexión WiFi para transmitir datos a la plataforma **ThingSpeak**.
- **Alertas por Telegram**: Se envían alertas en tiempo real si la corriente es demasiado alta o el voltaje es demasiado bajo.
- **Control de relé**: Un relé controlado por el ESP32 se activa o desactiva dependiendo de los valores de corriente y voltaje.

## Requisitos

- **Hardware**:
  - **ESP32**: Placa de desarrollo con WiFi integrado.
  - **ACS712**: Sensor de corriente.
  - **ZMPT101B**: Sensor de voltaje.
  - **DS18B20**: Sensor de temperatura.
  - **RTC DS3231**: Módulo de reloj en tiempo real.
  - **Relé**: Para controlar dispositivos.
  
- **Software**:
  - **Arduino IDE** con soporte para ESP32.
  - **Bibliotecas**: `WiFi.h`, `Wire.h`, `RTClib.h`, `OneWire.h`, `DallasTemperature.h`, `WiFiClientSecure.h`.
  
## Instalación

1. **Configura tu entorno de desarrollo**:
   - Instala la última versión de [Arduino IDE](https://www.arduino.cc/en/software).
   - Asegúrate de tener el soporte para **ESP32** instalado en el IDE.

2. **Configura las credenciales de WiFi**:
   - Modifica las líneas `const char* ssid` y `const char* password` con los datos de tu red WiFi.

3. **Configura ThingSpeak**:
   - Crea una cuenta en [ThingSpeak](https://thingspeak.com/) y genera una clave API para tu canal de datos.
   - Modifica la variable `apiKey` con tu clave API.

4. **Configura Telegram**:
   - Crea un bot en [BotFather](https://core.telegram.org/bots#botfather) y obtén el token.
   - Modifica la variable `token` con el token de tu bot.
   - Obtén tu `chat_id` a través de un bot de Telegram.

5. **Conecta los sensores**:
   - Conecta los sensores ACS712, ZMPT101B, DS18B20 y RTC a los pines especificados en el código.

6. **Carga el código en el ESP32**:
   - Conecta tu ESP32 al ordenador y carga el código desde el IDE de Arduino.

## Funcionamiento

### Conexión a WiFi
Al iniciar el sistema, el ESP32 intentará conectarse a la red WiFi utilizando las credenciales proporcionadas. Si la conexión es exitosa, se mostrará la IP del dispositivo.

### Lectura de Sensores
El sistema lee periódicamente los valores de los sensores conectados:

- **ACS712** para medir la corriente (en amperios).
- **ZMPT101B** para medir el voltaje (en voltios).
- **DS18B20** para medir la temperatura (en grados Celsius).

### Envío de Datos
Cada vez que se leen los valores de los sensores, los datos se envían a **ThingSpeak** a través de una solicitud HTTP POST.

### Alertas de Telegram
Si el sistema detecta que la corriente es superior a un umbral de 5 A o que el voltaje es inferior a 190 V, enviará una alerta a través de **Telegram** para notificar al usuario sobre el problema.

### Control del Relé
En función de los valores de corriente y voltaje, el sistema controla un **relé**. Si el voltaje es bajo o la corriente es alta, el relé se activa para desconectar el dispositivo conectado.

### RTC (Reloj en Tiempo Real)
El **RTC DS3231** proporciona la hora actual para poder realizar un monitoreo más detallado si es necesario.

## Configuración de ThingSpeak

1. Crea un canal en ThingSpeak para almacenar los datos.
2. Obtén la clave API de tu canal y configúralo en el código en la variable `apiKey`.

## Seguridad

Este sistema utiliza una conexión segura HTTPS para enviar mensajes a Telegram, protegiendo la privacidad de la información.

## Conclusión

Este proyecto proporciona una solución completa para monitorear y controlar parámetros de energía eléctrica en tiempo real, con la capacidad de alertar al usuario sobre situaciones anómalas a través de Telegram y actualizar los datos en ThingSpeak para su análisis remoto.

---

¡No dudes en experimentar con el código y adaptarlo a tus necesidades! Si tienes alguna duda o sugerencia, no dudes en abrir un **issue** o hacer un **pull request**.

