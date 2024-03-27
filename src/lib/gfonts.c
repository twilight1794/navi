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


//isupper
//strlen
int navi_get_url_gfonts(const Navi_GFonts_Family** familias, Navi_GFonts_Display display){
    return 0;
}
