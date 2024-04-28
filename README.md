![Logo](https://gitlab.com/campanita/navi/-/raw/main/doc/isologo.svg)

# Navi

Navi es un framework web para crear clientes web con el menor esfuerzo posible. Navi se encarga de escribir el código necesario para encargarse de cosas como la responsividad, el manejo de actividades y los servicios en segundo plano, para que puedas enfocarte en diseñar la funcionalidad y la interfaz de tu aplicación.

## Características

- **Baterías incluidas**: Navi incluye ya muchos widgets listos para ser usados: desde botones y elementos de formulario comunes, hasta elementos complejos como un mapa o un emulador de terminal.
- **Material Design**: el diseño de los widgets de Navi se ajustan en la medida de lo posible a las directrices de Material Design 2: un diseño intuitivo y a la vez lo suficientemente simple.
- **Adaptabilidad**: Navi gestiona por sí mismo la responsividad de la página, y creará automáticamente el código para ello.
- **Código limpio**: a diferencia de otros sistemas, Navi generará el menor código posible, para mantener una aplicación ligera y fácil de modificar después.
- **Vistas previas**: Navi te permitirá servir tu aplicación mientras la desarrollas —y la actualizará cada que hagas un cambio—.
- **IDE incluido**: puedes desarrollar tus aplicaciones en un IDE específicamente diseñado para trabajar con Navi: prueba [Navi Studio](https://gitlab.com/campanita/navi/).

## Instalación

Para compilar el código fuente, necesitarás la biblioteca libxml2.

Clona el proyecto, o [descarga un tarball](https://gitlab.com/campanita/navi/-/archive/main/navi-main.tar).

```bash
  make
  sudo make install
```
Esto instalará navi en `/usr/local/`.

## Licencia

¡Navi es software libre! Está bajo la licencia [GPLv3+](https://choosealicense.com/licenses/gpl-3.0/).
