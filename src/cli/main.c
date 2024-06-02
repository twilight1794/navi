#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <libintl.h>

#include "../common/log.h"
#include "../lib/init.h"
#include "../lib/gfonts.h"

#define _(STRING) gettext(STRING)

#define NAVI_VERSION "1.0"

void f_help(char* name){
    printf(_("Usage: %s <command> [<params>]\n\n"), name);
    puts(_("This is a list of available commands:"));
    puts(_(" init      Create a new Navi project"));
    puts(_(" package   Export a bundle ready to be published"));
    puts(_(" serve     Run a simple live web server to serve the project"));
    puts(_(" resource  Manage the files of the project"));
    puts(_(" lint      Search for errors and things to improve in the project"));
    puts(_(" clean     Remove files that aren't related to the project"));
    puts(_(" config    Show or set options of the project"));
    puts(_(" help      Show this help"));
    puts(_(" version   Show the Navi version"));
    puts(_("\nFor more info, see the man pages."));
}

void f_version(){
    printf(_("Navi %s\n"), NAVI_VERSION);
    puts(_("Copyright (C) 2024 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));
}

int main(int argc, char **argv){

    // Configuracion de logs
    struct Navi_Log_Config log_cfg = {
        .use_color = true,
        .show_date = true,
        .show_time = true,
        .show_file = false,
        .level = NAVI_LOG_DEBUG
    };

    // Parámetros
    int c;
    unsigned char opts = 0;
    opterr = 0;
    while ((c = getopt(argc, argv, "delnu")) != -1){
        if (c == 'd') opts |= 1;
        else if (c == 'e') opts |= 2;
        else if (c == 'l') opts |= 4;
        else if (c == 'n') opts |= 8;
        else if (c == 'u') opts |= 16;
        else if (c == 'w') opts |= 32;
    }

    // Subcomandos
    char* subcom;
    if (optind == argc){
        log_error(&log_cfg, _("A command was not specified."));
        exit(EXIT_FAILURE);
    } else subcom = argv[optind++];

    if (!strcmp(subcom, "init")){
        if (optind == argc){
            log_fatal(&log_cfg, _("The project name was not specified."));
            exit(EXIT_FAILURE);
        } else {
            Navi_Init(argv[optind]);
            if (errno == NAVI_INIT_ERR_DIRPROJ){
                log_fatal(&log_cfg, _("The project directory can not be created."));
                exit(EXIT_FAILURE);
            } else if (errno == NAVI_INIT_ERR_SUBDIR){
                log_fatal(&log_cfg, _("A directory inside the project directory can not be created."));
                exit(EXIT_FAILURE);
            }
        }
    } else if (!strcmp(subcom, "package")){
    } else if (!strcmp(subcom, "serve")){
    } else if (!strcmp(subcom, "resource")){
    } else if (!strcmp(subcom, "lint")){
    } else if (!strcmp(subcom, "clean")){
    } else if (!strcmp(subcom, "config")){
    } else if (!strcmp(subcom, "help")){
        f_help(argv[0]);
    } else if (!strcmp(subcom, "version")){
        f_version();
    } else {
        char msg[100];
        snprintf(msg, 99, _("The command %s was not recognized."), subcom);
        log_fatal(&log_cfg, msg);
        exit(EXIT_FAILURE);
    }

    return 0;
}
