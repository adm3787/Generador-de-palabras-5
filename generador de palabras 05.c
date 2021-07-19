#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palabras02.h"

#define OPCARACTERES     "caracteres="
#define OPINICIO         "inicio="
#define OPFIN            "fin="
#define OPCANTIDAD       "cantidad="
#define OPFIJAR          "fijar="
#define OPUBICACION      "ubicacion="
#define UBICACIONIZQ     "izquierda"
#define UBICACIONDER     "derecha"
#define IZQUIERDA        0
#define DERECHA          1
#define OPLONGITUDFIJA   "longitud_fija="
#define OPARCHIVO        "archivo="
#define OPMODO           "modo="
#define MODOEXHAUSTIVO   "exhaustivo"
#define MODOCONTEO       "conteo"
#define OPCONSECUTIVOS   "consecutivos="
#define OPESCRITURA      "escritura="
#define ESCRITURANORMAL  "normal"
#define ESCRITURAINVERSA "inversa"
#define ESCRITURAMIXTA   "mixta"
#define NORMAL           0
#define INVERSA          1
#define MIXTA            2
#define CRECIENTE        0
#define DECRECIENTE      1
#define DIGITOS          "0123456789"
#define LONGITUD_PALABRA 300
#define LONGITUD_ENTERO  100

typedef struct t_opciones{
                           char* caracteres;
                           t_palabra* inicio;
                           t_palabra* fin;
                           char cantidad[LONGITUD_ENTERO];
                           char fija[LONGITUD_PALABRA];
                           int ubicacion;
                           int longitudfija;
                           char* nombreArchivo;
                           FILE* archivo;
                           int modo;
                           int consecutivos;
                           int escritura;
                         }t_opciones;

int buscarOpcion(char* op, char** args, int n_args);

int hayStr(char* s);

char* cargarCaracteres(char* s_caracteres);
int hayCaracteres(char* caracteres);

t_palabra* cargarPalabra(char* s_palabra, char* alfabeto);
int hayPalabra(t_palabra* palabra);

int tieneSignoNegativo(char* s);
int tieneSignoPositivo(char* s);
int tieneSigno(char* s);
int esNumeroEntero(char* s);
char* cargarEntero(char* s_cantidad, char *s);
int hayEntero(char* cantidad);

int cargarUbicacion(char* s_ubicacion);
int hayUbicacion(int ubicacion);

int hayFija(char* s_fija);
int hayLongitudFija(int x_longitudfija);

char* cargarNombreArchivo(char* s_nombreArchivo);
int hayNombreArchivo(char* nombre);

FILE* cargarArchivo(char* nombre);

int cargarModo(char* s_modo);
int hayModo(int modo);

int cargarNatural(char* s_natural);
int hayNatural(int numero);

int cargarEscritura(char* s_escritura);
int hayEscritura(int escritura);

int hayOpcionesDesconocidas(char** args, int n_args);
void liberarOpciones(t_opciones* op);
t_opciones* cargarOpciones(char** args, int n_args);
int hayOpciones(t_opciones* op);

int cumpleConsecutivos(t_palabra* palabra, int consecutivos);

int cumpleLongitud(int x_longitudfija, t_palabra* palabra);

void componerStr(t_opciones* op, char* p, char* str);

void generarIntervalo(t_opciones* op);
void generarCantidad(t_opciones* op);
void generadorPalabras(t_opciones* op);

void mostrarDetalles(t_opciones* op);
void archivoGenerado(t_opciones* op);

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm//
int main(int argc, char** args){
  t_opciones* opciones = cargarOpciones(args,argc);

  if(!hayOpciones(opciones)){
    printf( "\n%s\n\n" , devolverMensajeError() );
  }else{
    if(hayNombreArchivo(opciones->nombreArchivo)){
      mostrarDetalles(opciones);
    }//hayNombreArchivo

    // generador
    generadorPalabras(opciones);
    // generador

    if(hayNombreArchivo(opciones->nombreArchivo)){
      archivoGenerado(opciones);
      fclose(opciones->archivo);
    }

    liberarOpciones(opciones);
  }

  return 0;
}
/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//----------------------------------------------------------------------------//

int buscarOpcion(char* op, char** args, int n_args){
  int i=1;

  while(i<n_args && memcmp(op,args[i],strlen(op))!=0) i++;

  return (i==n_args)? 0 : i ;
}

//----------------------------------------------------------------------------//

int hayStr(char* s){
  return s != NULL;
}

//----------------------------------------------------------------------------//

char* cargarCaracteres(char* s_caracteres){
  char* s = NULL;

  if(cadenaVacia(s_caracteres)){
    cargarMensajeError("Falto ingresar: <caracteres>");
  }else{
    s = crearAlfabeto(s_caracteres);
  }

  return s;
}

int hayCaracteres(char* caracteres){
  return hayStr(caracteres);
}

//----------------------------------------------------------------------------//

t_palabra* cargarPalabra(char* s_palabra, char* alfabeto){
  t_palabra* palabra = NULL ;

  if(cadenaVacia(s_palabra)){
    cargarMensajeError("Falto ingresar: <palabra>");
  }else{
    palabra = crearPalabra(s_palabra,alfabeto);
  }

  return palabra;
}

int hayPalabra(t_palabra* palabra){
  return palabra != NULL;
}

//----------------------------------------------------------------------------//

int tieneSignoNegativo(char* s){
  return s[0]=='-';
}

int tieneSignoPositivo(char* s){
  return s[0]=='+';
}

int tieneSigno(char* s){
  return tieneSignoNegativo(s) || tieneSignoPositivo(s);
}

int esNumeroEntero(char* s){
  return (tieneSigno(s))
         ? strspn(s+1,DIGITOS) == strlen(s)-1
         : strspn(s,DIGITOS) == strlen(s)
         ;
}

char* cargarEntero(char* s_cantidad, char *s){

  if(cadenaVacia(s_cantidad)){
    cargarMensajeError("Falto ingresar: <entero>");
  }else{
    if(!esNumeroEntero(s_cantidad)){
      char s_aux[LONGITUD_ERROR];
      sprintf(s_aux,"\"%s\" no es un numero entero",s_cantidad);
      cargarMensajeError(s_aux);
    }else{
      char* aux = strpbrk(s_cantidad,DIGITOS+1);// busco el primer digito <> 0
      if(aux == NULL){
        cargarMensajeError("HOLA!!! SENTIDO COMUN!! \"generar 0 palabras\"!?");
      }else{
        int haySignoNegativo = tieneSignoNegativo(s_cantidad);
        sprintf(s,"%s%s",(haySignoNegativo)?"-":"",aux);
      }
    }// else - !esNumeroEntero
  }// else - cadenaVacia

  return s;
}

int hayEntero(char* cantidad){
  return cantidad[0] != '\0' ;
}

//----------------------------------------------------------------------------//

int hayFija(char* s_fija){
  return s_fija[0] != '\0' ;
}

//----------------------------------------------------------------------------//

int hayLongitudFija(int x_longitudfija){
  return x_longitudfija > 0 ;
}

//----------------------------------------------------------------------------//

char* cargarNombreArchivo(char* s_nombreArchivo){
  char* s = NULL ;

  if(cadenaVacia(s_nombreArchivo)){
    cargarMensajeError("Falto ingresar: <nombre_archivo>");
  }else{
    s = s_nombreArchivo;
  }

  return s;
}

int hayNombreArchivo(char* nombre){
  return hayStr(nombre);
}

//----------------------------------------------------------------------------//

FILE* cargarArchivo(char* nombre){
  return (hayNombreArchivo(nombre))? fopen(nombre,"wt") : stdout ;
}

//----------------------------------------------------------------------------//

int cargarModo(char* s_modo){
  int modo = EXHAUSTIVO;

  if(cadenaVacia(s_modo)){
    modo = -1;
    cargarMensajeError("Falto ingresar: exhaustivo | conteo");
  }else{
    modo = ( strcmp(s_modo,MODOCONTEO) == 0 )
           ? CONTEO
           : ( strcmp(s_modo,MODOEXHAUSTIVO) == 0 )
             ? EXHAUSTIVO
             : -1
           ;

    if(modo == -1){
      char s_aux[LONGITUD_ERROR];
      sprintf( s_aux , "\"%s\" %s"
             , s_modo
             , "no es un modo de generacion de palabras valido"
             );
      cargarMensajeError(s_aux);
    }
  }// if cadenaVacia s_modo

  return modo;
}

int hayModo(int modo){
  return modo != -1;
}

//----------------------------------------------------------------------------//

int cargarNatural(char* s_natural){
  int n = -1 ;

  if(cadenaVacia(s_natural)){
    cargarMensajeError("Falto ingresar: <natural>");
  }else{
    if(sscanf(s_natural,"%d",&n) != 1   ||   n < 0){
      char s_aux[LONGITUD_ERROR];
      sprintf(s_aux,"\"%s\" no es un numero natural",s_natural);
      cargarMensajeError(s_aux);
    }
  }

  return n;
}

int hayNatural(int numero){
  return numero >= 0;
}

//----------------------------------------------------------------------------//

int cargarEscritura(char* s_escritura){
  int e = -1;

  if(cadenaVacia(s_escritura)){
    cargarMensajeError("Falto ingresar: normal | inversa | mixta");
  }else{
    e = ( strcmp(s_escritura,ESCRITURAMIXTA) == 0 )
        ? MIXTA
        : ( strcmp(s_escritura,ESCRITURAINVERSA) == 0 )
          ? INVERSA
          : ( strcmp(s_escritura,ESCRITURANORMAL) == 0 )
            ? NORMAL
            : -1
        ;

    if(e == -1){
      char s_aux[LONGITUD_ERROR];
      sprintf(s_aux,"\"%s\" no es un modo de escritura valido",s_escritura);
      cargarMensajeError(s_aux);
    }
  }// else - cadenaVacia

  return e;
}

int hayEscritura(int escritura){
  return escritura != -1;
}

//----------------------------------------------------------------------------//

int cargarUbicacion(char* s_ubicacion){
  int ubicacion = -1 ;

  if(cadenaVacia(s_ubicacion)){
    cargarMensajeError("Falto ingresar: izquierda | derecha");
  }else{
    ubicacion = ( strcmp(s_ubicacion,UBICACIONIZQ) == 0 )
                ? IZQUIERDA
                : ( strcmp(s_ubicacion,UBICACIONDER) == 0 )
                  ? DERECHA
                  : -1
                ;

    if(ubicacion == -1){
      char s_aux[LONGITUD_ERROR];
      sprintf(s_aux,"\"%s\" no es una ubicacion valida",s_ubicacion);
      cargarMensajeError(s_aux);
    }
  }

  return ubicacion;
}

int hayUbicacion(int ubicacion){
  return ubicacion != -1 ;
}

//----------------------------------------------------------------------------//

int hayOpcionesDesconocidas(char** args, int n_args){
  int respuesta = 0;
  char* opciones[] = { OPCARACTERES , OPINICIO , OPFIN , OPCANTIDAD
                     , OPARCHIVO , OPMODO , OPCONSECUTIVOS
                     , OPESCRITURA , OPFIJAR , OPUBICACION , OPLONGITUDFIJA
                     };
  int i=1;
  int j;

  while(respuesta==0 && i<n_args){
    j=0;
    while(j<11 && memcmp(args[i],opciones[j],strlen(opciones[j]))!=0){
      j++;
    }
    respuesta = j==11;
    i++;
  }

  if(respuesta){
    char s_aux[LONGITUD_ERROR];
    sprintf(s_aux,"\"%s\" no es una opcion valida",args[--i]);
    cargarMensajeError(s_aux);
  }

  return respuesta;
}

//----------------------------------------------------------------------------//

void liberarOpciones(t_opciones* op){
  if(hayCaracteres(op->caracteres))
    free(op->caracteres);

  if(hayPalabra(op->inicio))
    liberarPalabra(op->inicio);

  if(hayPalabra(op->fin))
    liberarPalabra(op->fin);

  //~ if(hayEntero(op->cantidad))
      //~ liberarStr(op->cantidad);

  free(op);
}

//----------------------------------------------------------------------------//

t_opciones* cargarOpciones(char** args, int n_args){
  t_opciones* op = NULL;
  char s_aux[LONGITUD_ERROR];

  if(n_args == 1){
  sprintf( s_aux
         ,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"
         , "Opciones obligatorias\n - "
         , OPCARACTERES
         , "<caracteres>\n - "
         , OPINICIO
         , "<palabra>\n - "
         , OPFIN
         , "<palabra> | "
         , OPCANTIDAD
         , "<entero>\n\nOpciones no obligatorias\n - "
         , OPFIJAR
         , "<palabra>\n   - "
         , OPUBICACION
         , "( izquierda | derecha )\n   - "
         , OPLONGITUDFIJA
         , "<natural>\n - "
         , OPARCHIVO
         , "<nombre_archivo>\n - "
         , OPESCRITURA
         , "( "
         , ESCRITURANORMAL
         , " | "
         , ESCRITURAINVERSA
         , " | "
         , ESCRITURAMIXTA
         , " )\n - "
         , OPMODO
         , "( "
         , MODOEXHAUSTIVO
         , " | "
         , MODOCONTEO
         , " )\n - "
         , OPCONSECUTIVOS
         , "<natural>"
         );
    cargarMensajeError(s_aux);
  }else{
    if( !hayOpcionesDesconocidas(args,n_args) ){
      op = (t_opciones*)malloc(sizeof(t_opciones));
      op->caracteres    = NULL;
      op->inicio        = NULL;
      op->fin           = NULL;
      op->cantidad[0]   = '\0';
      op->fija[0]       = '\0';
      op->ubicacion     = -1;
      op->longitudfija  = -1;
      op->nombreArchivo = NULL;
      op->modo          = EXHAUSTIVO;
      op->consecutivos  = -1;
      op->escritura     = NORMAL;
      int i;

      // --- caracteres --- //
      i = buscarOpcion(OPCARACTERES,args,n_args);
      if(i == 0){
        liberarOpciones(op);
        sprintf( s_aux , "%s%s%s"
               , "Falto ingresar: ",OPCARACTERES,"<caracteres> (obligatorio)"
               );
        cargarMensajeError(s_aux);
        return NULL;
      }else{
        op->caracteres = cargarCaracteres(args[i]+strlen(OPCARACTERES));
        if(!hayCaracteres(op->caracteres)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- carcteres --- //

      // --- inicio --- //
      i = buscarOpcion(OPINICIO,args,n_args);
      if(i == 0){
        liberarOpciones(op);
        sprintf( s_aux , "%s%s%s"
               , "Falto ingresar: ",OPINICIO,"<palabra> (obligatorio)"
               );
        cargarMensajeError(s_aux);
        return NULL;
      }else{
        op->inicio = cargarPalabra(args[i] + strlen(OPINICIO),op->caracteres);
        if(!hayPalabra(op->inicio)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- inicio --- //

      // --- fin --- cantidad --- //
      i = buscarOpcion(OPFIN,args,n_args);
      if(i == 0){
        i = buscarOpcion(OPCANTIDAD,args,n_args);
        if(i == 0){
          liberarOpciones(op);
          sprintf( s_aux , "%s%s%s%s%s"
                 , "Falto ingresar: "
                 , OPFIN
                 , "<palabra> | "
                 , OPCANTIDAD
                 , "<entero> (obligatorio)"
                 );
          cargarMensajeError(s_aux);
          return NULL;
        }else{
          strcpy(op->cantidad,cargarEntero(args[i]+strlen(OPCANTIDAD),s_aux));
          if(!hayEntero(op->cantidad)){
            liberarOpciones(op);
            return NULL;
          }
        }// else - i==0 OPCANTIDAD
      }else{
        op->fin = cargarPalabra( args[i] + strlen(OPFIN) , op->caracteres );
        if(!hayPalabra(op->fin)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- fin --- cantidad --- //

      // --- fijar --- //
      i = buscarOpcion(OPFIJAR,args,n_args);
      if(i > 0){
        t_palabra* p_aux;
        char *s_alf = crearAlfabeto(" ..~");
        p_aux = cargarPalabra(args[i]+strlen(OPFIJAR),s_alf);
        liberarStr(s_alf);
        if(!hayPalabra(p_aux)){
          liberarOpciones(op);
          return NULL;
        }else{
          char s_aux[LONGITUD_PALABRA];
          strcpy(op->fija,devolverPalabra(p_aux,s_aux));
          liberarPalabra(p_aux);
          i = buscarOpcion(OPUBICACION,args,n_args);
          if(i == 0){
            liberarOpciones(op);
            sprintf( s_aux , "%s%s%s%s%s%s%s%s%s"
                   , "Falto ingresar: "
                   , OPUBICACION
                   , "( "
                   , UBICACIONIZQ
                   , " | "
                   , UBICACIONDER
                   , " )\n(obligatorio cuando \""
                   , OPFIJAR
                   , "<palabra>\" existe)"
                   );
            cargarMensajeError(s_aux);
            return NULL;
          }else{
            op->ubicacion = cargarUbicacion( args[i] + strlen(OPUBICACION) );
            if(!hayUbicacion(op->ubicacion)){
              liberarOpciones(op);
              return NULL;
            }else{
              i = buscarOpcion(OPLONGITUDFIJA,args,n_args);
              if(i > 0){
                op->longitudfija=cargarNatural(args[i]+strlen(OPLONGITUDFIJA));
                if(!hayNatural(op->longitudfija)){
                  liberarOpciones(op);
                  return NULL;
                }else{
                  if(!hayLongitudFija(op->longitudfija)){
                    cargarMensajeError("Debe ser: longitud_fija > 0");
                    liberarOpciones(op);
                    return NULL;
                  }else{
                    if(longitud(op->inicio) < op->longitudfija){
                      // sprintf( s_aux
                             // , "%s\"%s\"\n%s\"%d\""
                             // , "La longitud de la palabra inicial "
                             // , devolverPalabra(op->inicio)
                             // , "es menor a la longitud fija "
                             // , op->longitudfija
                             // );
   cargarMensajeError("Debe ser: longitud(<palabra_inicial>) >= longitud_fija");
                      liberarOpciones(op);
                      return NULL;
                    }
                  }// else - !hayLongitudFija
                }// else - !hayNatural longitudfija
              }// if i>0 oplongitudfija
            }// else - !hayUbicacion
          }// else - i==0 opubicacion
        }// else - !hayPalabra fija
      }
      // --- fijar --- //

      // --- nombre archivo --- //
      i = buscarOpcion(OPARCHIVO,args,n_args);
      if(i > 0){
        op->nombreArchivo = cargarNombreArchivo(args[i]+strlen(OPARCHIVO));
        if(!hayNombreArchivo(op->nombreArchivo)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- nombre archivo --- //

      // --- archivo --- //
      op->archivo = cargarArchivo(op->nombreArchivo);
      // --- archivo --- //

      // --- modo --- //
      i = buscarOpcion(OPMODO,args,n_args);
      if(i > 0){
        op->modo = cargarModo(args[i]+strlen(OPMODO));
        if(!hayModo(op->modo)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- modo --- //

      // --- consecutivos --- //
      i = buscarOpcion(OPCONSECUTIVOS,args,n_args);
      if(i > 0){
        op->consecutivos = cargarNatural(args[i]+strlen(OPCONSECUTIVOS));
        if(!hayNatural(op->consecutivos)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- consecutivos --- //

      // --- escritura --- //
      i = buscarOpcion(OPESCRITURA,args,n_args);
      if(i > 0){
        op->escritura = cargarEscritura(args[i]+strlen(OPESCRITURA));
        if(!hayEscritura(op->escritura)){
          liberarOpciones(op);
          return NULL;
        }
      }
      // --- escritura --- //

    }// if !hayOpcionesDesconocidas
  }// else - n_args==1

  return op;
}

//----------------------------------------------------------------------------//

int hayOpciones(t_opciones* op){
  return op != NULL ;
}

//----------------------------------------------------------------------------//

int cumpleConsecutivos(t_palabra* palabra, int consecutivos){
  char caracterActual = palabra->principio->n_orden;
  t_caracter* aux = palabra->principio->sgte;
  int x_consecutivos = 0;

  while(aux!=NULL && x_consecutivos<=consecutivos){
    if(aux->n_orden==caracterActual){
      x_consecutivos++;
    }else{
      x_consecutivos = 0;
      caracterActual = aux->n_orden;
    }
    aux=aux->sgte;
  }

  return x_consecutivos <= consecutivos;
}

//----------------------------------------------------------------------------//

int cumpleLongitud(int x_longitudfija, t_palabra* palabra){
  return longitud(palabra) >= x_longitudfija;
}

//----------------------------------------------------------------------------//

void componerStr(t_opciones* op, char* p, char* str){
  char s1[strlen(p)+1];
  char s2[strlen(p)+1];
  int b_hayLongitud = hayLongitudFija(op->longitudfija);

  // --- s1 --- //
  if(op->ubicacion == DERECHA){
    if(b_hayLongitud){
      strncpy(s1,p,op->longitudfija);
      s1[op->longitudfija] = '\0';
    }else{
      sprintf(s1,"%s",p);
    }
  }else{// op->ubicacion == IZQUIERDA
    if(b_hayLongitud){
      strncpy(s1,p,strlen(p)-op->longitudfija);
      s1[strlen(p)-op->longitudfija]='\0';
    }else{
      s1[0] = '\0';
    }
  }
  // --- s1 --- //

  // --- s2 --- //
  if(op->ubicacion == IZQUIERDA){
    if(b_hayLongitud){
      sprintf(s2,"%s",p + (strlen(p)-op->longitudfija));
    }else{
      sprintf(s2,"%s",p);
    }
  }else{// op->ubicacion == DERECHA
    if(b_hayLongitud){
      sprintf(s2,"%s",p+op->longitudfija);
    }else{
      s2[0] = '\0';
    }
  }
  // --- s2 --- //

  sprintf( str , "%s%s%s" , s1 , op->fija , s2 );
}

//----------------------------------------------------------------------------//

void generarIntervalo(t_opciones* op){
  int orden = (compararPalabras(op->inicio,op->fin)<=0)? CRECIENTE:DECRECIENTE;
  t_palabra* palabra1 = (orden==CRECIENTE)? op->inicio : op->fin;
  t_palabra* palabra2 = (orden==CRECIENTE)? op->fin    : op->inicio;
  t_palabra* palabra  = copiarPalabra(op->inicio);
  char anterior[LONGITUD_PALABRA];
  int b_hayNombreArchivo = hayNombreArchivo(op->nombreArchivo);
  t_palabra* i = (b_hayNombreArchivo)? crearPalabra("0",DIGITOS) : NULL;
  int b_hayConsecutivos = hayNatural(op->consecutivos);
  int b_hayLongitud = hayLongitudFija(op->longitudfija);
  char strCompuesta[LONGITUD_PALABRA];
  char s_aux[LONGITUD_PALABRA];

  while( (esAnterior(palabra1,palabra) || esIgual(palabra1,palabra)) &&
         (esAnterior(palabra,palabra2) || esIgual(palabra,palabra2)) ){
    if( !( !b_hayLongitud || cumpleLongitud(op->longitudfija,palabra) ) ){
      break;
    }else{
      if( !b_hayConsecutivos || cumpleConsecutivos(palabra,op->consecutivos) ){
        // mostrar palabra
        if(op->escritura==NORMAL || op->escritura==MIXTA){
          if(hayUbicacion(op->ubicacion)){
            componerStr(op,devolverPalabra(palabra,s_aux),strCompuesta);
            fprintf(op->archivo,"%s\n",strCompuesta);
          }else{
            fprintf(op->archivo,"%s\n",devolverPalabra(palabra,s_aux));
          }
          if(b_hayNombreArchivo){
            palabraPosterior(i,CONTEO);
          }
        }

        if(  op->escritura==INVERSA
          || (op->escritura==MIXTA && !esPalindromo(palabra)) ){
          if(hayUbicacion(op->ubicacion)){
            componerStr(op,devolverPalabraInvertida(palabra,s_aux),strCompuesta);
            fprintf(op->archivo,"%s\n",strCompuesta);
          }else{
            fprintf(op->archivo,"%s\n",devolverPalabraInvertida(palabra,s_aux));
          }
          if(b_hayNombreArchivo){
            palabraPosterior(i,CONTEO);
          }
        }
        // mostrar palabra
      }

      if(orden==CRECIENTE){
        palabraPosterior(palabra,op->modo);
      }else{
        strcpy(anterior,devolverPalabra(palabra,s_aux));
        palabraAnterior(palabra,op->modo);
        if( strcmp(anterior,devolverPalabra(palabra,s_aux)) == 0 ) break;
      }
    }
  }// while
  strcpy( op->cantidad , (b_hayNombreArchivo)? devolverPalabra(i,s_aux) : "" );

  liberarPalabra(palabra);
  if(hayPalabra(i))
    liberarPalabra(i);
}

//----------------------------------------------------------------------------//

void generarCantidad(t_opciones* op){
  int orden = (tieneSignoNegativo(op->cantidad))? DECRECIENTE : CRECIENTE;
  t_palabra* i = crearPalabra("1",DIGITOS);
  t_palabra* n = crearPalabra( op->cantidad + tieneSignoNegativo(op->cantidad)
                             , DIGITOS
                             );
  t_palabra* palabra  = copiarPalabra(op->inicio);
  char anterior[LONGITUD_PALABRA];
  int b_hayConsecutivos = hayNatural(op->consecutivos);
  int b_hayLongitud = hayLongitudFija(op->longitudfija);
  char strCompuesta[LONGITUD_PALABRA];
  char s_aux[LONGITUD_PALABRA];

  while(esAnterior(i,n) || esIgual(i,n)){
    if( !( !b_hayLongitud || cumpleLongitud(op->longitudfija,palabra) ) ){
      break;
    }else{
      if( !b_hayConsecutivos || cumpleConsecutivos(palabra,op->consecutivos) ){
        // mostrar palabra
        if(op->escritura==NORMAL || op->escritura==MIXTA){
          if(hayUbicacion(op->ubicacion)){
            componerStr(op,devolverPalabra(palabra,s_aux),strCompuesta);
            fprintf(op->archivo,"%s\n",strCompuesta);
          }else{
            fprintf(op->archivo,"%s\n",devolverPalabra(palabra,s_aux));
          }
          palabraPosterior(i,CONTEO);
        }

        if( (esAnterior(i,n) || esIgual(i,n))              &&
            (op->escritura==INVERSA                        ||
            (op->escritura==MIXTA && !esPalindromo(palabra)))
          ){
          if(hayUbicacion(op->ubicacion)){
            componerStr(op,devolverPalabraInvertida(palabra,s_aux),strCompuesta);
            fprintf(op->archivo,"%s\n",strCompuesta);
          }else{
            fprintf(op->archivo,"%s\n",devolverPalabraInvertida(palabra,s_aux));
          }
          palabraPosterior(i,CONTEO);
        }
        // mostrar palabra
      }

      if(orden==CRECIENTE){
        palabraPosterior(palabra,op->modo);
        if( b_hayConsecutivos                             &&
            strlen(op->caracteres) == 1                   &&
            !cumpleConsecutivos(palabra,op->consecutivos) ) break;
      }else{
        strcpy(anterior,devolverPalabra(palabra,s_aux));
        palabraAnterior(palabra,op->modo);
        if( strcmp(anterior,devolverPalabra(palabra,s_aux)) == 0 ) break;
      }
    }
  }// while

  if(hayNombreArchivo(op->nombreArchivo)){
    palabraAnterior(i,CONTEO);
    strcpy(op->cantidad,devolverPalabra(i,s_aux));
  }

  liberarPalabra(i);
  liberarPalabra(n);
  liberarPalabra(palabra);
}

//----------------------------------------------------------------------------//

void generadorPalabras(t_opciones* op){
  if(hayPalabra(op->fin)){
    generarIntervalo(op);
  }else{
    generarCantidad(op);
  }
}

//----------------------------------------------------------------------------//

void mostrarDetalles(t_opciones* op){
  char s_aux[LONGITUD_PALABRA];

  printf("\nArchivo: \"%s\"\n",op->nombreArchivo);
  printf("Alfabeto: [%s]\n",op->caracteres);
  printf("Palabra inicial: %s\n",devolverPalabra(op->inicio,s_aux));

  if(hayPalabra(op->fin)){
    printf( "Palabra final: %s\n",devolverPalabra(op->fin,s_aux));
  }

  if(hayFija(op->fija)){
    printf( "Palabra fija: %s\nUbicacion: %s\n"
          , op->fija
          , (op->ubicacion == DERECHA)? UBICACIONDER : UBICACIONIZQ
          );
    if(hayLongitudFija(op->longitudfija)){
      printf("Longitud fija: %d\n",op->longitudfija);
    }
  }

  printf( "Escritura: %s\n"
        , (op->escritura==NORMAL)
          ? ESCRITURANORMAL
          : (op->escritura==INVERSA)
            ? ESCRITURAINVERSA
            : ESCRITURAMIXTA
        );

  printf("Modo: %s\n",(op->modo==CONTEO)? MODOCONTEO:MODOEXHAUSTIVO);

  if(hayNatural(op->consecutivos)){
    printf("Consecutivos: %d\n",op->consecutivos);
  }
}

//----------------------------------------------------------------------------//

void archivoGenerado(t_opciones* op){
  printf("Cantidad de palabras: %s\n",op->cantidad);
  printf( "%s%s%s"
        , "\n ------------------\n"
        ,  "| Archivo generado |\n"
        ,  " ------------------\n\n"
        );
}

//----------------------------------------------------------------------------//
