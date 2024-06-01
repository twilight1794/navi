#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libintl.h>

#include "../common/log.h"
#include "../lib/init.h"
#include "../lib/gfonts.h"

#define _(STRING) gettext(STRING)

#define NAVI_VERSION "1.0"

void f_help(char* name){
    printf("Usage: %s COMMAND [params]\n", name);
    printf("COMMAND=[init|package|serve|resource|lint|clean|config|help|version]\n\n");
    printf("For more info, see the man pages\n");
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

    // Comandos
    if (argc == 1){
      log_error(&log_cfg, _("A command was not specified."));
      exit(EXIT_FAILURE);
    }


    if (!strcmp(argv[1], "init")){
        if (argc < 3){
            log_fatal(&log_cfg, _("The project name was not specified."));
            exit(EXIT_FAILURE);
        } else {
            int error = 0;
            Navi_Init(argv[2], &error);
            if (error == 0xfe){ // FIX: Códigos de error
                log_fatal(&log_cfg, _("The project directory can not be created."));
                exit(EXIT_FAILURE);
            } else if (error == 0xff){
                log_fatal(&log_cfg, _("A directory inside the project directory can not be created."));
                exit(EXIT_FAILURE);
            }
        }
    } else if (!strcmp(argv[1], "package")){
    } else if (!strcmp(argv[1], "serve")){
    } else if (!strcmp(argv[1], "resource")){
    } else if (!strcmp(argv[1], "lint")){
    } else if (!strcmp(argv[1], "clean")){
    } else if (!strcmp(argv[1], "config")){
    } else if (!strcmp(argv[1], "help")){
        f_help(argv[0]);
    } else if (!strcmp(argv[1], "version")){
        f_version();
    } else {
        log_fatal(&log_cfg, _("The specified command was not recognized."));
        exit(EXIT_FAILURE);
    }

    return 0;
}
