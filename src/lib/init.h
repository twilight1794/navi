/**
 * @file init.h
 * Módulo para crear un proyecto
 */

#ifndef NAVI_L_INIT
#define NAVI_L_INIT

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "paths.h"

/**
 * Crea un directorio con un proyecto de Navi limpio
 * @param[in] pname Nombre del proyecto
 * @param[out] error Error ocurrido, si lo hubo
 */
void Navi_Init(char* pname, int* error);

#endif
