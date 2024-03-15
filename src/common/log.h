#ifndef NAVI_L_LOG
#define NAVI_L_LOG

#include <stdio.h>
#include <stdbool.h>

// Objeto de configuración
typedef struct {
    bool use_color;
    bool show_date;
    bool show_time;
    bool show_file;
    int level;
} Navi_log_config;

enum { NAVI_LOG_DEBUG, NAVI_LOG_INFO, NAVI_LOG_WARN, NAVI_LOG_ERROR, NAVI_LOG_FATAL };

#define log_debug(CONF, MSG) navi_log(CONF, NAVI_LOG_DEBUG, __FILE__, __LINE__, MSG)
#define log_info(CONF, MSG) navi_log(CONF, NAVI_LOG_INFO, __FILE__, __LINE__, MSG)
#define log_warn(CONF, MSG) navi_log(CONF, NAVI_LOG_WARN, __FILE__, __LINE__, MSG)
#define log_error(CONF, MSG) navi_log(CONF, NAVI_LOG_ERROR, __FILE__, __LINE__, MSG)
#define log_fatal(CONF, MSG) navi_log(CONF, NAVI_LOG_FATAL, __FILE__, __LINE__, MSG)

void navi_log(Navi_log_config*, int, const char*, unsigned int, const char*);
  
#endif
