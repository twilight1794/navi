#include "init.h"

// Macro para crear directorios
#define CREATE_DIR(a)\
    strcpy(path_suf, a);\
    if (mkdir(path_created, 0777)){\
        *error = 0xff;\
        return;\
    }

void Navi_Init(char* pname, int* error){
    // Crear directorio
    if (mkdir(pname, 0777)){
        *error = 0xfe;
        return;
    }

    // Inicializar nombre de directorio de proyecto
    size_t tam_pname = strlen(pname);
    char* path_created = calloc(tam_pname+14, 1); // Tamaño máximo de las rutas a crear + 2
    memcpy(path_created, pname, tam_pname);
    path_created[tam_pname] = '/';
    char* path_suf = path_created + tam_pname + 1;

    // Preparar directorios
    CREATE_DIR(NAVI_PATH_BUILD)
    CREATE_DIR(NAVI_PATH_LIB)
    CREATE_DIR(NAVI_PATH_SRC)
    CREATE_DIR(NAVI_PATH_AUDIO)
    CREATE_DIR(NAVI_PATH_CSS)
    CREATE_DIR(NAVI_PATH_DRAWABLE)
    CREATE_DIR(NAVI_PATH_FONT)
    CREATE_DIR(NAVI_PATH_LAYOUT)
    CREATE_DIR(NAVI_PATH_JS)
    CREATE_DIR(NAVI_PATH_MENU)
    CREATE_DIR(NAVI_PATH_RAW)
    CREATE_DIR(NAVI_PATH_THEME)
    CREATE_DIR(NAVI_PATH_VALUES)
    CREATE_DIR(NAVI_PATH_VIDEO)

    // Preparar project.xml
}
