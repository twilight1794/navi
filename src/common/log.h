
/**
 * @file log.h
 * Funciones para generar mensajes de log
 */

#ifndef NAVI_C_LOG
#define NAVI_C_LOG

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/**
 * Objeto de configuración
 */
struct Navi_Log_Config {
    bool use_color; /**< Indica si se usarán caracteres de escape ANSI para colorear los mensajes */
    bool show_date; /**< Indica si se mostrará la fecha */
    bool show_time; /**< Indica si se mostrará la hora */
    bool show_file; /**< Indica si se mostrará el número de línea */
    int level; /**< Indica el nivel mínimo de los mensajes a mostrar */
};

enum { NAVI_LOG_DEBUG, NAVI_LOG_INFO, NAVI_LOG_WARN, NAVI_LOG_ERROR, NAVI_LOG_FATAL };

#define log_debug(CONF, MSG) navi_log(CONF, NAVI_LOG_DEBUG, __FILE__, __LINE__, MSG)
#define log_info(CONF, MSG) navi_log(CONF, NAVI_LOG_INFO, __FILE__, __LINE__, MSG)
#define log_warn(CONF, MSG) navi_log(CONF, NAVI_LOG_WARN, __FILE__, __LINE__, MSG)
#define log_error(CONF, MSG) navi_log(CONF, NAVI_LOG_ERROR, __FILE__, __LINE__, MSG)
#define log_fatal(CONF, MSG) navi_log(CONF, NAVI_LOG_FATAL, __FILE__, __LINE__, MSG)

void navi_log(struct Navi_Log_Config*, int, const char*, unsigned int, const char*);

#endif
