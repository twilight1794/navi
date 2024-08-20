"use strict"

/**
 * Clase para manejar una aplicación Navi
 */
export default class Navi {
  #activities_cache;
  #views_cache;
  #uri_regexes;
  #states;
  #aid_seq;
  #previous_uri;

  constructor(){
    // Caché de objetos
    this.#activities_cache = {};
    this.#views_cache = {};
    this.#uri_regexes = {};

    this.#states = []; // Estados de actividades
    this.#aid_seq = 0; // Secuencia de AIDs
  }

  // Funciones de bajo nivel
  //
  // Estas funciones no deben comprobar errores del usuario: en este punto, ya
  // deben haberse validado la entrada del usuario

  /**
   * Crea una nueva instancia de una actividad
   * @param activity_name Nombre de la clase de la cual se creará la actividad
   */
  #activity_create(activity_name){
    let aid = ++this.#aid_seq;

    // Crear instancia de actividad
    let activity_class = this.#activities_cache[activity_name];
    let activity_obj = new activity_class(aid);
    this.#states.push(activity_obj);
    activity_obj.on_create();
    return aid;
  }

  /**
   * Muestra una actividad al usuario
   * @param aid AID de la actividad a mostrar
   */
  #activity_show(aid){
    let activity_obj = this.#states.find(e => aid == e.aid);
    let activity_elem = document.getElementById(`aid-${aid}`);
    if (!activity_elem){
      // Hacer un clon de la vista
      activity_elem = this.#views_cache[activity_obj.constructor.name.slice(0, -8)].documentElement.cloneNode(true);
      activity_elem.id = `aid-${aid}`;
      document.body.appendChild(activity_elem);
    }
    activity_elem.ariaCurrent = "page";
    activity_obj.on_show();
  }

  /**
   * Oculta una actividad al usuario
   */
  #activity_hide(aid){
    let activity_elem = document.querySelector("[aria-current='page']");
    if (activity_elem){
      activity_elem.ariaCurrent = undefined;
    }
    let activity_obj = this.#states.find(e => aid == e.aid);
    activity_obj.on_hide();
  }

  /**
   * Inicia el tratamiento de excepciones en la actividad
   */
  #activity_fail(aid){
    //this.#activity_obj.on_fail();
  }

  /**
   * Elimina una instancia de la actividad
   */
  #activity_destroy(aid){
    // :Obtener actividad
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
      document.documentElement.appendChild(document.createElement('body'));

    // Definir eventos
    if (!document.body.dataset.events){
      // Cuando el usuario se mueve en el historial
      window.addEventListener("popstate", (e) => {
        console.debug(`popstate: ${e.state}`);

        // Comprobar si existe el AID pedido
        if (e.state && e.state.aid && document.getElementById(`aid-${e.state.aid}`)){
          // AID existente
          
          // iniciar
        } else {
          // AID inválido
        }
        
        // Obtener AID del estado actual
        let aid_actual = parseInt(document.querySelector("[aria-current='page']").id.substring(4));
        
        
      });

      // Lanzado cuando el tamaño de la pantalla cambie
      window.addEventListener("resize", (e) => {
        console.debug(`resize: ${window.innerWidth}, ${window.innerHeight}`);
      });

      // Lanzado cuando el esquema de colores cambie
      window.matchMedia("(prefers-color-scheme: dark)")
      .addEventListener("change",({ matches }) => {
        if (matches){
          console.debug("¡Cambiado a tema oscuro!");
        } else {
          console.debug("¡Cambiado a tema claro!");
        }
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
                    throw new Error(`El patrón de URL de la actividad ${e} no reconoce variables de tipo ${match_obj[1]} .`);
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
      // ...e iniciar la primera cuando todo esté listo
      this.#activity_show(this.#activity_create(acts[0].getAttribute("href")));
    });
  }
  
  _acts(){ return this.#activities_cache; }
  _views(){ return this.#views_cache; }
  _regexes(){ return this.#uri_regexes; }
  _states() {return this.#states; }

  // Funciones de alto nivel: Navegación
  /**
   * Navega a una nueva actividad, en espera de devolver un resultado
   */
  activity_call(uri, data){
    // Obtener clase a llamar
    var activity_class = this.get_view_by_uri(uri);
    if (!activity_class){
      console.error(`La URI #${uri} no puede ser procesada por ninguna actividad.`);
      return;
    }
    // Comprobar que no exista otra instancia
    //if (!this.#states.every(e => e.constructor.name == activity_class)){
    //  console.log("Se puede hacer el cambio!");
    //} else {
    //  console.error(`Ya has creado otra actividad de la clase ${activity_class}`);
    //}
    let aid = 0;
    // lanzar evento
    window.dispatchEvent(new PopStateEvent("popstate", {'state': { 'aid': aid}}));
  }

  /**
   * Navega a una nueva actividad, reemplazando la anterior
   */
  activity_go(uri, data){
    
  }

  /**
   * Regresa a la actividad anterior
   */
  activity_return(uri, data){
    
  }

  get_view_by_uri(uri){
    return Object.keys(this.#activities_cache).find(e => uri.match(this.#uri_regexes[e]));
  }
  
  get_current_activity(){
  }
}

export { Navi };
