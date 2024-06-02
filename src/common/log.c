#include "log.h"

void navi_log(struct Navi_Log_Config* conf, int type, const char* file, unsigned int line, const char* msg){

    // Fecha-hora
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);
    char dbuff[11];
    if (conf->show_date){
	strftime(dbuff, 11, "%F", timeinfo);
	printf("%s ", dbuff);
    }
    if (conf->show_time){
	strftime(dbuff, 11, "%T", timeinfo);
	printf("%s ", dbuff);
    }

    // Tipo
    switch (type){
    case NAVI_LOG_DEBUG:
	if (conf->use_color) printf("\x1b[36;1mDEBUG\x1b[0m ");
	else printf("DEBUG ");
	break;
    case NAVI_LOG_INFO:
	if (conf->use_color) printf("\x1b[32;1mINFO\x1b[0m  ");
	else printf("INFO  ");
	break;
    case NAVI_LOG_WARN:
	if (conf->use_color) printf("\x1b[33;1mWARN\x1b[0m  ");
	else printf("WARN  ");
	break;
    case NAVI_LOG_ERROR:
	if (conf->use_color) printf("\x1b[31;1mERROR\x1b[0m ");
	else printf("ERROR ");
	break;
    case NAVI_LOG_FATAL:
	if (conf->use_color) printf("\x1b[35;1mFATAL\x1b[0m ");
	else printf("FATAL ");
	break;
    }

    // Archivo-línea
    if (conf->show_file && conf->use_color) printf("\x1b[90m%s:%d \x1b[0m%s\x1b[0m\n", file, line, msg);
    else if (conf->show_file) printf("%s:%d: %s\n", file, line, msg);
    else printf("%s\n", msg);
}
