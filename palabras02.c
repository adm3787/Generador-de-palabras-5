#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palabras02.h"

//----------------------------------------------------------------------------//

char mensajeError[LONGITUD_ERROR];

void cargarMensajeError(char* s_error){
  strcpy(mensajeError,s_error);
}

int hayError(){
  return mensajeError != NULL ;
}

char* devolverMensajeError(){
  return mensajeError;
}

//----------------------------------------------------------------------------//

int cadenaVacia(char* s){
  return s[0] == '\0' ;
}

//----------------------------------------------------------------------------//

char* copiarStr(char* s){
  char* s_aux = (char*)malloc(sizeof(char)*(strlen(s)+1));
  s_aux = strcpy(s_aux,s);

  return s_aux;
}

//----------------------------------------------------------------------------//

char* crearAlfabeto(char* caracteres){
  char* alfabeto = NULL;

  if(!cadenaVacia(caracteres)){
    if(!caracteresPermitidos(caracteres)){
      cargarMensajeError("Se encontro un caracter no permitido\n\
Un caracter \"c\" es permitido si: 32 <= orden(c) <= 126");
    }else{
      char* aux = (char*)malloc(sizeof(char)*(LONGITUD_CARACTERES+1));
      int i_car,i_alf=0;

      for (i_car=0 ; caracteres[i_car]!='\0'; i_car++){
        if (!hayDosPuntosSeguidos(i_car,caracteres)){
          if(!estaCaracter(caracteres[i_car],aux)){
            aux[i_alf++]=caracteres[i_car];
          }
        }else{
          if(!extremosIguales(caracteres[i_car-1],caracteres[i_car+2])){
            agregarSecuenciaCaracteres( caracteres[i_car-1]
                                      , caracteres[i_car+2]
                                      , &i_alf
                                      , aux              );
          }
          i_car+=2;
        }// else - !hayDosPuntosSeguidos
      }// for
      aux[i_alf]='\0';
      alfabeto = copiarStr(aux);
      liberarStr(aux);
    }// else - !caracteresPermitidos
  }// if !cadenaVacia

  return alfabeto;
}

//----------------------------------------------------------------------------//

int caracteresPermitidos(char* caracteres){
  int respuesta = 1;
  int i = 0;

  while(caracteres[i]!='\0'){
    if(' '<=caracteres[i] && caracteres[i]<='~'){
      i++;
    }else{
      respuesta = 0;
      break;
    }
  }// while

  return respuesta;
}

//----------------------------------------------------------------------------//

int estaCaracter(char c, char* alfabeto){
  return strchr(alfabeto,c) != NULL;
}

//----------------------------------------------------------------------------//

int hayDosPuntosSeguidos(int i, char* caracteres)
{
  return 0<i                      &&
         i+1<strlen(caracteres)-1 &&
         caracteres[i]=='.'       &&
         caracteres[i+1]=='.'     ;
}

//----------------------------------------------------------------------------//

int extremosIguales(char c1, char c2)
{
  return c1 == c2 ;
}

//----------------------------------------------------------------------------//

void agregarSecuenciaCaracteres(char c1, char c2, int* i, char* alfabeto)
{
  int paso = (c1<c2)? 1 : -1 ;
  int i_aux;

  for(i_aux=c1 ; comparacionSegunOrden(paso,i_aux,c2) ; i_aux+=paso)
    if(!estaCaracter(i_aux,alfabeto))
      alfabeto[(*i)++]=i_aux;
}

//----------------------------------------------------------------------------//

// op_comp: opcion de comparacion
// op_comp + : creciente     -->   comparacion: <=
// op_comp - : decreciente   -->   comparacion: >=
int comparacionSegunOrden(int op_comp, int a, int b)
{
  return ( op_comp>0 )? (a<=b) : (a>=b) ;
}

//----------------------------------------------------------------------------//

int caracteresCorrectos(char* palabra, char* alfabeto){
  return alfabeto!=NULL && strlen(palabra)==strspn(palabra,alfabeto);
}

//----------------------------------------------------------------------------//

char orden(char c, char* alfabeto){
  return ( estaCaracter(c,alfabeto) ) ?
         strchr(alfabeto,c)-alfabeto  :// diferencia de direcciones de memoria
         -1                           ;
}

//----------------------------------------------------------------------------//

char caracter(char n_orden, char* alfabeto){
  return (0 <= n_orden && n_orden<strlen(alfabeto))
         ? alfabeto[(short)n_orden]
         : -1 ;
}

//----------------------------------------------------------------------------//

t_caracter* crearCaracter(char n_orden, t_caracter* p_ant, t_caracter* p_sgte)
{
  t_caracter* nuevo=(t_caracter*)malloc(sizeof(t_caracter));

  nuevo->n_orden=n_orden;
  nuevo->ant=p_ant;
  nuevo->sgte=p_sgte;

  return nuevo;
}

//----------------------------------------------------------------------------//

int esPalabraNula(t_palabra* palabra){
  return palabra == NULL;
}

//----------------------------------------------------------------------------//

t_palabra* crearPalabraVacia(){
  t_palabra* nueva=(t_palabra*)malloc(sizeof(t_palabra));

  nueva->alfabeto  = NULL;
  nueva->principio = NULL;
  nueva->fin       = NULL;
  nueva->longitud  = 0;

  return nueva;
}

//----------------------------------------------------------------------------//

int esPalabraVacia(t_palabra* palabra){
  return palabra->alfabeto  == NULL &&
         palabra->principio == NULL &&
         palabra->fin       == NULL &&
         palabra->longitud  == 0    ;
}

//----------------------------------------------------------------------------//

int esNulaOvacia(t_palabra* palabra){
  return esPalabraNula(palabra) || esPalabraVacia(palabra);
}

//----------------------------------------------------------------------------//

t_palabra* crearPalabra(char* palabra, char* alfabeto){
  t_palabra* nueva = NULL;
  int n = strlen(palabra);

  if(n > 0){
    if(!caracteresCorrectos(palabra,alfabeto)){
      sprintf( mensajeError
             , "%s%s%s%s[%s]"
             , "En la palabra \""
             , palabra
             , "\" se encontro un caracter\n"
             , "que no pertenece al alfabeto "
             , alfabeto
             );
    }else{
      nueva = crearPalabraVacia();
      int i=0;

      nueva->alfabeto  = copiarStr(alfabeto);
      nueva->principio =
      nueva->fin       = crearCaracter( orden(palabra[i++],alfabeto) ,
                                      NULL                         ,
                                      NULL                         );

      while(palabra[i]!='\0'){
        nueva->fin = crearCaracter( orden(palabra[i++],alfabeto)
                                  , nueva->fin
                                  , NULL                       );
        nueva->fin->ant->sgte=nueva->fin; // enlazo al anteultimo con el ultimo
      }

      nueva->longitud = n;
    }// else - !caracteresCorrectos
  }// if n > 0

  return nueva;
}

//----------------------------------------------------------------------------//

char* devolverPalabra(t_palabra* palabra, char *s){

  if(esNulaOvacia(palabra)){
    s[0] = '\0' ;
  }else{
    t_caracter* c = palabra->principio;
    int i=0;

    while(c != NULL){
      s[i++] = caracter(c->n_orden,palabra->alfabeto);
      c = c->sgte;
    }
    s[i] = '\0';
  }

  return s;
}

//----------------------------------------------------------------------------//

char* devolverPalabraInvertida(t_palabra* palabra, char *s){

  if(esNulaOvacia(palabra)){
    s[0] = '\0' ;
  }else{
    t_caracter* c = palabra->fin;
    int i=0;

    while(c != NULL){
      s[i++] = caracter(c->n_orden,palabra->alfabeto);
      c = c->ant;
    }
    s[i] = '\0';
  }

  return s;
}

//----------------------------------------------------------------------------//

int longitud(t_palabra* palabra){
  return (esNulaOvacia(palabra))? 0 : palabra->longitud;
}

//----------------------------------------------------------------------------//

t_palabra* copiarPalabra(t_palabra* palabra){
  t_palabra* nueva;

  if(esPalabraNula(palabra)){
    nueva = NULL;
  }else{
    nueva = crearPalabraVacia();
    if(!esPalabraVacia(palabra)){
      t_caracter* aux = palabra->principio;

      nueva->alfabeto  = copiarStr(palabra->alfabeto);
      nueva->principio =
      nueva->fin       = crearCaracter(aux->n_orden,NULL,NULL);
      aux = aux->sgte;
      while(aux!=NULL){
        nueva->fin = crearCaracter(aux->n_orden,nueva->fin,NULL);
        nueva->fin->ant->sgte = nueva->fin;
        aux = aux->sgte;
      }
      nueva->longitud = palabra->longitud;
    }
  }

  return nueva;
}

//----------------------------------------------------------------------------//

void palabraPosterior(t_palabra* palabra, int modo){
  if(!esNulaOvacia(palabra)){
    t_caracter* aux = palabra->fin;
    int tam_alfabeto = strlen(palabra->alfabeto);

    while(aux!=NULL && ++(aux->n_orden)==tam_alfabeto){
      aux->n_orden=0;
      aux=aux->ant;
    }
    if( aux==NULL ){
      aux = crearCaracter((tam_alfabeto==1)? 0:modo,NULL,palabra->principio);
      palabra->principio->ant=aux;
      palabra->principio=aux;
      palabra->longitud++;
    }
  }
}

//----------------------------------------------------------------------------//

void palabraAnterior(t_palabra* palabra, int modo){
  if( !esNulaOvacia(palabra)  &&
      !( longitud(palabra)==1 && palabra->principio->n_orden==0) ){
    t_caracter* aux = palabra->fin;
    int ultimo_n_orden = strlen(palabra->alfabeto) - 1;

    while(aux!=NULL && --(aux->n_orden)==-1){
      aux->n_orden=ultimo_n_orden;
      aux=aux->ant;
    }

    if( aux == NULL                  ||
        ( modo == CONTEO             &&
          aux  == palabra->principio &&
          aux  != palabra->fin       &&
          aux->n_orden == 0          )
      ){
      aux = palabra->principio;
      palabra->principio = aux->sgte;
      palabra->principio->ant = NULL;
      free(aux);
      palabra->longitud--;
    }
  }
}

//----------------------------------------------------------------------------//

int sonIncomparables(t_palabra* palabra1, t_palabra* palabra2){
  return !esNulaOvacia(palabra1)                          &&
         !esNulaOvacia(palabra2)                          &&
         strcmp(palabra1->alfabeto,palabra2->alfabeto)!=0 ;
}

//----------------------------------------------------------------------------//

int compararPalabras(t_palabra* palabra1, t_palabra* palabra2){
  // diferencia <  0: palabra1 < palabra2  -> comparacion = -1
  // diferencia =  0: palabra1 = palabra2  -> comparacion =  0
  // diferencia >  0: palabra1 > palabra2  -> comparacion =  1
  // son incomparables -> comparacion = strlen(incomparables) = 13 ;D
  int comparacion;

  if(sonIncomparables(palabra1,palabra2)){
    comparacion = 13;
  }else{// son comparables
    int diferencia  = longitud(palabra1) - longitud(palabra2);
    comparacion = (diferencia < 0)
                  ? -1
                  : (diferencia > 0)
                    ? 1
                    : 0
                  ;

    if(comparacion==0 && !esNulaOvacia(palabra1)){
      t_caracter* aux1 = palabra1->principio;
      t_caracter* aux2 = palabra2->principio;

      while(aux1!=NULL && aux1->n_orden==aux2->n_orden){
        aux1 = aux1->sgte;
        aux2 = aux2->sgte;
      }

      if(aux1 != NULL){
        comparacion = (aux1->n_orden-aux2->n_orden < 0)? -1 : 1;
      }
    }
  }// son comparables

  return comparacion;
}

//----------------------------------------------------------------------------//

int esIgual(t_palabra* palabra1, t_palabra* palabra2){
  return compararPalabras(palabra1,palabra2) == 0 ;
}

//----------------------------------------------------------------------------//

int esAnterior(t_palabra* palabra1, t_palabra* palabra2){
  return compararPalabras(palabra1,palabra2) == -1 ;
}

//----------------------------------------------------------------------------//

int esPosterior(t_palabra* palabra1, t_palabra* palabra2){
  return compararPalabras(palabra1,palabra2) == 1 ;
}

//----------------------------------------------------------------------------//

char caracterI(int i, t_palabra* palabra){
  char c = -1;
  int  l = longitud(palabra);

  if(!esNulaOvacia(palabra) && ((1<=i && i<=l) || (-l<=i && i<=-1))){
    t_caracter* aux = (i>0)? palabra->principio : palabra->fin;
    int j = (i>0)? 1 : -1;

    while(aux!=NULL && j!=i){
      if(i>0){
        aux = aux->sgte;
        j++;
      }else{
        aux = aux->ant;
        j--;
      }
    }// while
    c = caracter(aux->n_orden,palabra->alfabeto);
  }// if

  return c;
}

//----------------------------------------------------------------------------//

int esPalindromo(t_palabra* palabra){
  int respuesta = 0;

  if(!esNulaOvacia(palabra)){
    if(longitud(palabra) == 1){
      respuesta = 1;
    }else{
      int maximoComparaciones = longitud(palabra) / 2;
      int comparacion = 1;
      t_caracter* izq = palabra->principio;
      t_caracter* der = palabra->fin;

      while(comparacion <= maximoComparaciones){
        if(izq->n_orden == der->n_orden){
          izq = izq->sgte;
          der = der->ant;
          comparacion++;
        }else{
          break;
        }
      }

      respuesta = comparacion > maximoComparaciones;
    }
  }

  return respuesta;
}

//----------------------------------------------------------------------------//

void liberarStr(char* s){
  free(s);
}

//----------------------------------------------------------------------------//

void liberarCaracteresRecursivamente(t_caracter* c){
  if(c == NULL)
    /* no hago nada */;
  else{
    liberarCaracteresRecursivamente(c->sgte);
    free(c);
  }
}

//----------------------------------------------------------------------------//

void liberarPalabra(t_palabra* palabra){
  liberarStr(palabra->alfabeto);
  liberarCaracteresRecursivamente(palabra->principio);
  free(palabra);
}

//----------------------------------------------------------------------------//
