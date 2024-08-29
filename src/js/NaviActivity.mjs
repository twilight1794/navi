"use strict"

/**
 * Clase para una actividad
 */
export default class NaviActivity {

  constructor(aid, ctx){
    this.aid = aid; // AID = Activity ID
    this.ctx = ctx; // Contexto
  }

  // Eventos del ciclo de vida
  /**
   * Cuando se crea una actividad: nuevo estado
   * @param data Datos a pasar a la nueva actividad
   */
  on_create(data){
    console.debug(`Creando actividad ${this.constructor.name}`);
  }

  /**
   * Cuando se muestra una actividad al usuario
   */
  on_show(result){
    console.debug(`Mostrando actividad ${this.constructor.name}`);
    // Por mientras:
    document.querySelector("[aria-current='page']").ariaCurrent = undefined;
  }

  /**
   * Cuando ocurre un evento que debe ser procesado
   */
  on_state_change(event){
    console.debug(`Redibujando actividad ${this.constructor.name}`);
  }

  /**
   * Cuando ocurre una excepción en una actividad
   */
  on_fail(error){
    console.debug(`Capturando excepción de la actividad ${this.constructor.name}`);
  }

  /**
   * Cuando se muestra otra actividad al usuario
   */
  on_hide(){
    console.debug(`Ocultando actividad ${this.constructor.name}`);
  }

  /**
   * Cuando se destruye una actividad: estado eliminado
   */
  on_destroy(){
    console.debug(`Destruyendo actividad ${this.constructor.name}`);
  }

  // Indica si de una vista solo podemos crear una sola actividad
  static get is_unique(){
    // En general, podremos crear varias actividades de una sola actividad.
    return false;
  }
}
