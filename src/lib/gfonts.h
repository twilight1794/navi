
/**
 * @file gfonts.h
 * Funciones para trabajar con la API de Google Fonts v2
 */

#ifndef NAVI_L_GFONTS
#define NAVI_L_GFONTS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../common/cadena.h"

// Valores de conveniencia para dev
char* NAVI_GFONTS_WEIGHT = "wght";
char* NAVI_GFONTS_WIDTH = "wdth";
char* NAVI_GFONTS_SLANT = "slnt";
char* NAVI_GFONTS_OPSIZE = "opsz";
char* NAVI_GFONTS_ITALIC = "ital";

/**
 * Errores
 */
enum Navi_GFonts_Error {
    NAVI_GFONTS_ERR_INVALID_OBJ, /**< Puntero a objeto inválido */
    NAVI_GFONTS_ERR_INVALID_DISP, /**< Valor para font-display inválido */
    NAVI_GFONTS_ERR_NOAXES, /**< No se incluyeron ejes a especificar, y sí valores */
    NAVI_GFONTS_ERR_INVALID_AXIS_SIZE, /**< La etiqueta de un eje sobrepasa el tamaño permitido */
    NAVI_GFONTS_ERR_INVALID_AXIS_NAME, /**< La etiqueta de un eje tiene caracteres inválidos */
};

/**
 * Valor para un eje de familia tipográfica
 */
struct Navi_GFonts_Value {
    bool isRange; /**< Especifica si el valor es puntual, o es un rango */
    union {
        float val; /**< Valor puntual */
        float valStart; /**< Valor mínimo en un rango */
    } valU;
    float valEnd; /**< Valor máximo en un rango */
};

/**
 * Objeto de configuración para una familia tipográfica
 */
struct Navi_GFonts_Family {
    char *name; /**< Nombre de la familia tipográfica */
    int n_axes; /**< Número de ejes especificados */
    int n_values; /**< Número de combinaciones de valores especificados */
    char **axes; /**< Array de cadenas con los ejes especificados */
    struct Navi_GFonts_Value **values; /**< Array de arrays de valores para los ejes */
};

/**
 * Valor para la propiedad font-display
 */
enum Navi_GFonts_Display {
    NAVI_GFONTS_AUTO,
    NAVI_GFONTS_BLOCK,
    NAVI_GFONTS_SWAP,
    NAVI_GFONTS_FALLBACK,
    NAVI_GFONTS_OPTIONAL
};

/**
 * Genera la URL para cargar un recurso de fuente desde Google Fonts
 * @param[in] n_familias Número de familias tipográficas a solicitar
 * @param[in] familias Lista de configuraciones para una tipografía
 * @param[in] display Valor para la propiedad font-display
 * @param[out] error Error ocurrido, si lo hubo
 * @return Cadena conteniendo la URL para cargar la fuente
 */
char* navi_get_url_gfonts(
  int n_familias,
  const struct Navi_GFonts_Family** familias,
  enum Navi_GFonts_Display display,
  int* error);

#endif
