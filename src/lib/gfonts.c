#include "gfonts.h"

// Especificación de la URL de la API de Google Fonts v2
// Tomado de https://developers.google.com/fonts/docs/css2
//
// /css2?family=<spec>[&family=<spec>...][&text=<text>][&display=<display>]
// spec: <family_name>[:<axis_tag_list>@<axis_tuple_list>]
// family_name: Nombre de la familia de fuentes
// axis_tag_list: <axis>[,<axis>...] // Sorted alphabetically (en-US locale)
// axis: Una etiqueta de eje, p.ej., ital, wdth, wght
// axis_tuple_list: <axis_tuple>[;<axis_tuple>...]
// axis_tuple: <value>[,<value>...] // Same length as axis_tag_list
// value: <range> | <float>
// range: <float>..<float>
// float: un valor dentro del rango del eje correspondiente
// text: Es el texto que se mostrará en el tipo de letra solicitado.
// display: auto | block | swap | fallback | optional

char* navi_get_url_gfonts(
  int n_familias,
  const Navi_GFonts_Family** familias,
  Navi_GFonts_Display display,
  int* error){

    if (familias == NULL || *familias == NULL){
        *error = NAVI_GFONTS_ERR_INVALID_OBJ;
        return NULL;
    }
    if (display<0 || display>4){
        *error = NAVI_GFONTS_ERR_INVALID_DISP;
        return NULL;
    }

    char* url = Cadena_Create();
    url = Cadena_Concat(url, "https://fonts.googleapis.com/css2?");
    char bufc[5]; // Para usar en Procesar nombre

    for (int i=0; i<n_familias; i++){
        // Procesar nombre
        url = Cadena_Concat(url, "family=");
        char* c = (familias[i])->name;
        while (*c){
            memset(bufc, 0, 5);
            bufc[0] = ((*c)==' ')?'+':(*c);
            url = Cadena_Add(url, bufc); // Substituir ' ' por '+'
            c++;
        }
        // Procesar valores
        if ((familias[i])->n_values){
            url = Cadena_Concat(url, ":");

            // Validar ejes
            if (!(familias[i])->n_axes){
                free(url);
                *error = NAVI_GFONTS_ERR_NOAXES;
                return NULL;
            }
            for (int j=0; j<(familias[i])->n_axes; j++){
                char* axis = (familias[i])->axes[j];
                if (strlen(axis) > 4){
                    free(url);
                    *error = NAVI_GFONTS_ERR_INVALID_AXIS_SIZE;
                    return NULL;
                }
                c = axis;
                while (*c){
                    if (isupper(*c) || islower(*c)){
                        free(url);
                        *error = NAVI_GFONTS_ERR_INVALID_AXIS_NAME;
                        return NULL;
                    }
                }
            }

            // Procesar ejes
            for (int j=0; j<(familias[i])->n_axes; j++){
                url = Cadena_Concat(url, (familias[i])->axes[j]);
                url = Cadena_Concat(url, ",");
            }
            url = Cadena_Remove(url);
            url = Cadena_Concat(url, "@");

            // Procesar conjuntos de valores
            Navi_GFonts_Value** vals = (familias[i])->values;
            for (int j=0; j<(familias[i])->n_values; j++){
                for (int k=0; k<(familias[i])->n_axes; k++){
                    if ((vals[j])->isRange){
                    } else {
                    }
                    url = Cadena_Concat(url, ",");
                }
                Cadena_Concat(url, ";");
            }
        }
        url = Cadena_Concat(url, "&");
    }
    // Quitar & si es último caracter
    if (url[strlen(url)-1] == '&') url = Cadena_Remove(url);
    return url;
}
