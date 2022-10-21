# Tutorial sobre ESP-IDF

## Introducción

Este es un tutorial paso a paso para configurar el framework ESP-IDF que te permitirá escribir aplicaciones en lenguaje C para la familia de microcontroladores ESP32 de Espressif. Está basado en la documentación de la versión estable v4.4.2 que puedes encontrar [aquí](https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/index.html).

## Qué necesitas

- Una PC con un SO Ubuntu o similar.
- Un buen editor de texto. Recomiendo [Visual Studio Code](https://code.visualstudio.com/).
- Una placa de desarrollo con el microcontrolador ESP32 ([modelos disponibles](https://www.espressif.com/en/products/devkits)).
- Un cable USB que te permita conectar la placa de desarrollo a la PC.
- Si quieres probar el ejemplo SSL contra un servidor en la nube necesitarás sus credenciales de acceso. También puedes probarlo con tu PC actuando como servidor.

## Instalación de dependencias

```shell
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

Probablemente necesites dar permiso a tu usuario para acceder a dispositivos de tipo USB-Serial. Basta con agregar tu usuario al grupo `dialout` y luego reiniciar tu sesión o bien reiniciar tu PC:

```shell
sudo adduser $USER dialout
```

## Instalación del framework ESP-IDF

El repositorio de ESP-IDF es bastante grande y posee muchos submódulos. El siguiente paso puede demorar varios minutos.

```shell
git clone -b v4.4.2 --recursive https://github.com/espressif/esp-idf.git
```

Luego entramos en la carpeta y ejecutamos el script de instalación:
```shell
cd esp-idf
./install.sh esp32
```

Si todo sale bien, deberíamos observar un mensaje como este al final:
```
All done! You can now run:

  . ./export.sh
```

## Configuración, compilación y ejecución de una aplicación básica

Para poder utilizar las herramientas de ESP-IDF es necesario configurar variables de entorno adicionales. Debes hacerlo siempre que abras una terminal nueva. Dirígete a la carpeta `esp-idf` que creamos en el paso anterior y ejecuta:

```shell
. ./export.sh
```

Si todo sale bien, deberíamos observar un mensaje como este al final:
```
Done! You can now compile ESP-IDF projects.
Go to the project directory and run:

  idf.py build
```

Ahora compilemos y ejecutemos nuestra primer aplicación! Partiendo de la carpeta `esp-idf` nos posicionamos en el ejemplo `hello_world`, lo compilamos y lo descargamos a la placa de desarrollo:
```shell
cd examples/get-started/hello_world/
idf.py build # si este paso falla es porque no configuraste las variables de entorno, recuerda el paso . ./export.sh.

idf.py flash monitor # grabamos el binario en la memoria flash y abrimos el monitor del puerto serial para observar la salida del programa.
```
## Creando nuestra propia aplicación básica

## Algo un poco más divertido: Ejemplo de conexión SSL
