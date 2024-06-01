
/**
 * @file error.h
 * Códigos de error de Navi
 */

#ifndef NAVI_C_ERROR
#define NAVI_C_ERROR

enum Navi_Error {
    // lib/gfonts.h
    NAVI_GFONTS_ERR_INVALID_OBJ, /**< Puntero a objeto inválido */
    NAVI_GFONTS_ERR_INVALID_DISPLAY, /**< Valor para font-display inválido */
    NAVI_GFONTS_ERR_NOAXES, /**< No se incluyeron ejes a especificar, y sí valores */
    NAVI_GFONTS_ERR_INVALID_AXIS_SIZE, /**< La etiqueta de un eje sobrepasa el tamaño permitido */
    NAVI_GFONTS_ERR_INVALID_AXIS_NAME, /**< La etiqueta de un eje tiene caracteres inválidos */
    // lib/init.h
    NAVI_INIT_ERR_DIRPROJ, /**< Hubo un error al crear el directorio raíz del proyecto */
    NAVI_INIT_ERR_SUBDIR, /**< Hubo un error al crear un subdirectorio del proyecto */
};

#endif
