#include "cadena.h"

char* Cadena_Create(void){
    return calloc(1, 1);
}

char* Cadena_Add(char* cad, const char* c){
    size_t tcar, tcad, i = 0;
    char* ncad;
    if (cad == NULL) return cad;
    tcar = Cadena_Size_Char(c);
    tcad = strlen(cad);
    ncad = realloc(cad, tcad + tcar + 1);
    if (ncad == NULL) return cad;
    while (i<tcar){
      ncad[tcad+i] = c[i];
      i++;
    }
    ncad[tcad+tcar] = 0;
    return ncad;
}

char* Cadena_Concat(char* cad, const char* cad2){
    size_t tcad, tcad2, i = 0;
    char* ncad;
    if (cad == NULL) return cad;
    tcad = strlen(cad);
    tcad2 = strlen(cad2);
    ncad = realloc(cad, tcad + tcad2 + 1);
    if (ncad == NULL) return cad;
    while (i<tcad2){
      ncad[tcad+i] = cad2[i];
      i++;
    }
    ncad[tcad+tcad2] = 0;
    return ncad;
}

char* Cadena_Remove(char* cad){
    size_t tcad;
    char* ncad;
    if (cad == NULL) return cad;
    tcad = strlen(cad);
    ncad = realloc(cad, tcad);
    if (ncad == NULL) return cad;
    ncad[tcad-1] = 0;
    return ncad;
}

size_t Cadena_Size_Char(const char* c){
  size_t n;
  if (*c == 0xC0 || *c == 0xC1 || *c >= 0xF5) n=0;
  else if (*c >= 0xF0) n=4;
  else if (*c >= 0xE0) n=3;
  else if (*c >= 0xC2) n=2;
  else n=1;
  return n;
}
