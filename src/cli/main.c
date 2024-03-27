#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <libintl.h>

#include "navidefs.h"
#include "../common/log.h"

#define _(STRING) gettext(STRING)

int opterr = 0;

void help(char* name){
    printf("%s [-v|-h|COMMAND]\n", name);
}

void version(){
    printf(_("Navi %s\n"), NAVI_VERSION);
    puts(_("Copyright (C) 2024 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));
}

int main(int argc, char **argv){
  Navi_Log_Config log_cfg = {
    .use_color = true,
    .show_date = true,
    .show_time = true,
    .show_file = true,
    .level = NAVI_LOG_DEBUG
  };

    int c;
    // Parametros estándar
    while ((c = getopt(argc, argv, "hv")) != -1){
        switch (c){
            case 'h':
                help(argv[0]);
                break;
            case 'v':
                version();
                return 0;
            case 'l':
                break;
            case '?':
               printf("[navi] Unknown param -%c\n", optopt);
               break;
        }
    }

    // Comandos
    if (argc == optind){
      log_error(&log_cfg, _("No se especificó un comando."));
      return 1;
    }

    char* cmd = argv[optind];
    if (strcmp(cmd, "start")){
    } else if (strcmp(cmd, "package")){
    } else if (strcmp(cmd, "serve")){
    } else if (strcmp(cmd, "resource")){
    }
    return 0;
}
