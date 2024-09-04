"use strict"

/**
 * Clase para manejar una aplicación Navi
 */
export default class Navi {
  #activities_cache;
  #views_cache;
  #uri_regexes;
  #states;
  #current_state_index;
  #aid_seq;

  constructor(){
    // Caché de objetos
    this.#activities_cache = {};
    this.#views_cache = {};
    this.#uri_regexes = {};

    this.#states = []; // Pila de estados
    this.#current_state_index = 0; // Puntero al estado actual
    this.#aid_seq = 0; // Secuencia de AIDs
  }

  // Funciones de bajo nivel
  //
  // Estas funciones no deben comprobar errores del usuario: en este punto, ya
  // se ha aceptado la petición de iniciar la nueva actividad

  /**
   * Crea una nueva instancia de una actividad
   * @param activity_class Clase de la cual se creará la actividad
   * @param params Parámetros pasados por la URI
   * @param objs Objetos pasados por código
   */
  #activity_create(activity_class, params, objs){
    console.debug(`Iniciando crear actividad ${activity_class.name}`);
    let aid = ++this.#aid_seq;

    // Crear instancia de actividad
    let activity_obj = new activity_class(aid, params, objs);

    // Agregar a la lista de actividades
    //   Si anadimos actividades en medio de la pila, los estados siguientes ya
    // ↓ no serán válidos
    this.#states.splice(this.#current_state_index, Infinity, activity_obj);
    this.#current_state_index++;

    activity_obj.on_create();
    return aid;
  }

  /**
   * Muestra una actividad al usuario
   * @param aid AID de la actividad a mostrar
   */
  #activity_show(aid){
    console.debug(`Iniciando mostrar actividad ${aid}`);
    let activity_obj = this.#states.find(e => aid == e.aid);
    let activity_elem = document.getElementById(`aid-${aid}`);
    if (!activity_elem){
      // Hacer un clon de la vista
      activity_elem = this.#views_cache[activity_obj.constructor.name.slice(0, -8)].documentElement.cloneNode(true);
      activity_elem.id = `aid-${aid}`;
      document.body.appendChild(activity_elem);
    }
    let previous_view = document.querySelector("[aria-current='page']");
    if (previous_view) previous_view.ariaCurrent = undefined;
    activity_elem.ariaCurrent = "page";
    activity_obj.on_show();
  }

  /**
   * Oculta una actividad al usuario
   * @param aid AID de la actividad a mostrar
   */
  #activity_hide(aid){
    console.debug(`Iniciando ocultar actividad ${aid}`);
    let activity_elem = document.querySelector("[aria-current='page']");
    if (activity_elem){
      activity_elem.ariaCurrent = undefined;
    }
    let activity_obj = this.#states.find(e => aid == e.aid);
    activity_obj.on_hide();
  }

  /**
   * Inicia el tratamiento de excepciones en la actividad
   * @param aid AID de la actividad a mostrar
   */
  #activity_fail(aid){
    //this.#activity_obj.on_fail();
  }

  /**
   * Elimina una instancia de la actividad
   * @param aid AID de la actividad a mostrar
   */
  #activity_destroy(aid){
    // Obtener actividad
    let activity_elem = document.getElementById(`aid-${aid}`);
    activity_elem.parentNode.removeChild(activity_elem);
    let activity_obj = this.#states.find(e => aid == e.aid);
    activity_obj.on_destroy();
  }

  async init(){
    // Preparar actividades
    let acts = document.head.querySelectorAll("link[rel='navi:activity']");
    if (!acts.length) throw new Error("No hay actividades definidas");

    // Podemos omitir body
    if (!document.body)
      document.documentElement.appendChild(document.createElement("body"));

    // Definir eventos
    if (!document.body.dataset.events){
      // Lanzado cuando el usuario se mueve en el historial
      window.addEventListener("popstate", (e) => {
        console.debug(`Evento popstate: ${e.state?.aid}`);
        if (e.state && e.state.aid){
          // AID especificado: es un cambio en el historial

          let tentative_idx = this.#states.findIndex(s => s.aid == e.state.aid);
          if (tentative_idx == -1) history.back(); // AID es inválido
          else {
            // AID es válido
            let activity = this.#states[tentative_idx];
            this.#current_state_index = tentative_idx+1;
            this.#activity_show(activity.aid);
          }
        }
        // AID inespecificado: es una URI manual
        else this.activity_go(location.hash.replace("#", "") || "");
      });

      // Lanzado cuando el tamaño de la pantalla cambie
      window.addEventListener("resize", (e) => {
        console.debug(`Evento resize: ${window.innerWidth}, ${window.innerHeight}`);
      });

      // Lanzado cuando el esquema de colores cambie
      window.matchMedia("(prefers-color-scheme: dark)")
      .addEventListener("Evento change",({ matches }) => {
        if (matches){
          console.debug("¡Cambiado a tema oscuro!");
        } else {
          console.debug("¡Cambiado a tema claro!");
        }
      });

      // Lanzaho cuando el idioma del navegador cambie
      window.addEventListener("languagechange", (e) => {
        console.log(`Evento languagechange: ${Navigator.language}`");
      });

      document.body.dataset.events = "events";
    }

    // Por último, lo asíncrono:
    // Descargar actividades...
    Promise.all([
      ...Array.from(acts).map(e => {
        // Leer clase
        let uri = e.getAttribute("href");
        return import(`./src/activities/${uri}.js`)
          // NOTE: Ver si podemos incluir validación acá
          .then(obj_mod => this.#activities_cache[uri] = obj_mod.default )
          .catch(() => console.error(`El módulo ${uri} no pudo ser importado.`) );
      }),
      ...Array.from(acts).map(e => {
        // Preparar vistas
        let uri = e.getAttribute("href");
        return fetch(`./src/views/${uri}.xml`)
          .then(response => response.text())
          .then(text => {
            // FIX: debería haber más validación acá
            let obj_view = (new DOMParser()).parseFromString( text, "application/xml" );
            this.#views_cache[uri] = obj_view;
          })
          .catch(() => console.error(`La vista base del módulo ${uri} no pudo ser importada`) );
      })
    ])
    .then(() => {
      Object.keys(this.#activities_cache).forEach(e => {
        //...generar las expresiones regulares...
        let pattern = this.#activities_cache[e].url_pattern;
        if (pattern === undefined) throw new Error(`No especificaste una ruta para acceder a la actividad ${e}.`);
        // Determinar expresión regular a utilizar
        if (pattern instanceof RegExp){
          // Es instancia de RegExp
          this.#uri_regexes[e] = pattern;
        } else if (typeof(pattern) == "string"){
          // Es una cadena, generar RegExp
          let regex_string = /^[^<]+/;
          let regex_escape = /[.*+?^${}()|[\]\\]/g;
          let regex_placeholder = /^<([a-z]+)(?::([a-z_]+))?>/;

          let new_regex = "^";
          let cad = pattern;
          while (cad.length){
            let match_obj = cad.match(regex_string);
            if (match_obj){
              new_regex += match_obj[0].replace(regex_escape, "\\$&");
              cad = cad.substring(match_obj[0].length);
              if (cad.length){
                match_obj = cad.match(regex_placeholder);
                if (match_obj){
                  if (match_obj[1] == "str"){
                    new_regex += `(${match_obj[2]?("?<"+match_obj[2]+">"):""}[^/<]+)`;
                  } else if (match_obj[1] == "int"){
                    new_regex += `(${match_obj[2]?("?<"+match_obj[2]+">"):""}[0-9]+)`;
                  } else if (match_obj[1] == "dec"){
                    new_regex += `(${match_obj[2]?("?<"+match_obj[2]+">"):""}(?:[0-9]+\\.[0-9]+)|(?:\\.?[0-9]+))`;
                  } else if (match_obj[1] == "path"){
                    new_regex += `(${match_obj[2]?("?<"+match_obj[2]+">"):""}[^<]+)`;
                  } else {
                    throw new Error(`El patrón de URL de la actividad ${e} no reconoce variables de tipo ${match_obj[1]}.`);
                  }
                  cad = cad.substring(match_obj[0].length);
                } else {
                  throw new Error(`El patrón de URL de la actividad ${e} está malformado.`);
                }
              }
            }
          }
          this.#uri_regexes[e] = new RegExp(new_regex+"$");
        } else {
          throw new Error("La función necesita una cadena o una instancia de RegExp.");
        }
      });
    })
    .then(() => {
      // ...determinar e iniciar la actividad a usar cuando todo esté listo
      this.activity_go(location.hash.replace("#", "") || "");
    });
  }

  _acts(){ return this.#activities_cache; }
  _views(){ return this.#views_cache; }
  _regexes(){ return this.#uri_regexes; }
  _states(){ return this.#states; }
  _current_state(){ return this.#current_state_index; }
  _aid_seq(){ return this.#aid_seq; }

  // Funciones de alto nivel: Navegación

  /**
   * Navega a una nueva actividad, en espera de devolver un resultado
   * @param uri URI de la actividad a iniciar
   * @param objs Datos a pasar a la nueva actividad
   */
  activity_call(uri, objs){
    console.debug(`Nueva petición de actividad: #${uri}.`);
    // Comprobar
    let url = this.#validate_uri(uri);
    let activity_class = this.#validate_intent(url);

    // Crear actividad
    let aid = this.#activity_create(activity_class, url.searchParams, objs);
    history.pushState({ "aid": aid }, null, (uri?"#":"")+uri);
    this.#activity_show(aid);
  }

  /**
   * Navega a una nueva actividad, reemplazando la anterior
   * @param uri URI de la actividad a iniciar
   * @param objs Datos a pasar a la nueva actividad
   */
  activity_go(uri, objs){
    // Comprobar
    let url = this.#validate_uri(uri);
    let activity_class = this.#validate_intent(url);

    // Crear actividad
    let aid = this.#activity_create(activity_class, url.searchParams, objs);
    history.replaceState({ "aid": aid }, null, (uri?"#":"")+uri);
    this.#activity_show(aid);
  }

  /**
   * Regresa a la actividad anterior
   * @param uri URI de la actividad a iniciar
   * @param objs Datos a pasar a la nueva actividad
   */
  activity_return(uri, objs){
    
  }

  // Funciones auxiliares de bajo nivel

  /**
   * Comprueba que una URI sea sintácticamente válida para iniciar una actividad
   * @param uri URI de la actividad a iniciar
   * @return El objeto URL con los datos procesados
   */
  #validate_uri(uri){
    let url = new URL(location.href);
    let regex_activity = /^([A-Za-z0-9_-]*)(?:\?(.*))?$/;
    let uri_match = regex_activity.exec(uri);
    if (!uri_match) throw new Error(`La URI #${uri} no es válida`);
    // ↓ Condicional para evitar # vacío
    if (uri_match[1]) url.hash = uri_match[1];
    url.search = uri_match[2] || "";
    return url;
  }

  /**
   * Comprueba si una petición para iniciar una nueva actividad es válida
   * @param uri URI de la actividad a iniciar
   * @return La clase de la cual crear la actividad
   */
  #validate_intent(url){
    // Obtener clase a llamar
    let activity_class = Object.entries(this.#activities_cache)
      .find(e => url.hash.replace("#", "").match(this.#uri_regexes[e[0]]))?.[1];
    if (!activity_class) throw new Error(`La URI ${url.hash} no puede ser procesada por ninguna actividad.`);

    //// Restricciones
    // Comprobar que no exista otra instancia
    if (activity_class.constructor.is_unique && !this.#states.every(e => e.constructor.name == activity_class))
      throw new Error(`Ya has creado otra actividad de la clase ${activity_class}`);

    return activity_class;
  }
}

export { Navi };
