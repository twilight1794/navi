#include "init.h"

// Macro para crear directorios
#define CREATE_DIR(a)\
    strcpy(path_suf, a);\
    if (mkdir(path_created, 0777)){\
        errno = NAVI_INIT_ERR_SUBDIR;\
        return;\
    }

void Navi_Init(char* pname){
    // Crear identificador
    // TODO: Debemos pedir uno siempre
    char proj_uri[22];
    strcpy(proj_uri, "uri:navi:");
    proj_uri[21] = 0;
    srand(time(NULL));
    for (int i=9; i<22; ++i){
        //0x30-0x39,0x61-0x7A=36
        proj_uri[i] = rand()%36;
        if (proj_uri[i]<10) proj_uri[i] += 0x30;
        else proj_uri[i] += 0x57; //0x57=0x61-10d
    }

    // Crear directorio
    if (mkdir(pname, 0777)){
        errno = NAVI_INIT_ERR_DIRPROJ;
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
    LIBXML_TEST_VERSION;
    strcpy(path_suf, NAVI_PATH_PROJECT);
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "RDF");
    xmlNsPtr ns_navi = xmlNewNs(root_node, BAD_CAST "https://www.campanita.xyz/proyectos/navi#", NULL);
    xmlNsPtr ns_rdf = xmlNewNs(root_node, BAD_CAST "http://www.w3.org/1999/02/22-rdf-syntax-ns#", BAD_CAST "rdf");
    xmlSetNs(root_node, ns_rdf);
    xmlDocSetRootElement(doc, root_node);
    xmlNodePtr real_root = xmlNewChild(root_node, ns_rdf, BAD_CAST "Description", NULL);
    xmlNewNsProp(real_root, ns_rdf, BAD_CAST "about", BAD_CAST proj_uri);

    // Rellenar datos
    xmlNodePtr proj_title = xmlNewChild(real_root, ns_navi, BAD_CAST "title", NULL);
    xmlNewNsProp(proj_title, ns_rdf, BAD_CAST "resource", NULL);
    xmlNodePtr proj_icon = xmlNewChild(real_root, ns_navi, BAD_CAST "icon", NULL);
    xmlNewNsProp(proj_icon, ns_rdf, BAD_CAST "resource", NULL);
    xmlNodePtr proj_maintheme = xmlNewChild(real_root, ns_navi, BAD_CAST "main-theme", NULL);
    xmlNewNsProp(proj_maintheme, ns_rdf, BAD_CAST "value", BAD_CAST "system");

    // Guardar y cerrar todo
    xmlSaveFormatFileEnc(path_created, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    free(path_created);
    return;
}
