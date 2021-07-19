#ifndef PALABRAS02_H
#define PALABRAS02_H

//----------------------------------------------------------------------------//

#define EXHAUSTIVO 0
#define CONTEO 1
#define LONGITUD_CARACTERES 95
#define LONGITUD_ERROR 400

//----------------------------------------------------------------------------//

typedef struct t_caracter{
                           char n_orden; // posicion del caracter en el alfabeto
                           struct t_caracter* sgte;
                           struct t_caracter* ant;
                         }t_caracter;

typedef struct t_palabra{
                          char* alfabeto;
                          t_caracter* principio;
                          t_caracter* fin;
                          int longitud;
                        }t_palabra;

//----------------------------------------------------------------------------//

void cargarMensajeError(char* s_error);
int hayError();
char* devolverMensajeError();

char* copiarStr(char* s);
int cadenaVacia(char* s);

char* crearAlfabeto(char* caracteres);
  int caracteresPermitidos(char* caracteres);
  int estaCaracter(char c, char* alfabeto);
  int hayDosPuntosSeguidos(int i, char* caracteres);
  int extremosIguales(char c1, char c2);
  void agregarSecuenciaCaracteres(char c1, char c2, int* i, char* alfabeto);
    int comparacionSegunOrden(int op_cmp, int a, int b);

int caracteresCorrectos(char* palabra, char* alfabeto);

char orden(char c, char* alfabeto);
char caracter(char n_orden, char* alfabeto);

t_caracter* crearCaracter(char n_orden, t_caracter* p_ant, t_caracter* p_sgte);

int esPalabraNula(t_palabra* palabra);
t_palabra* crearPalabraVacia();
int esPalabraVacia(t_palabra* palabra);
int esNulaOvacia(t_palabra* palabra);
t_palabra* crearPalabra(char* palabra, char* alfabeto);
char* devolverPalabra(t_palabra* palabra, char *s);
char* devolverPalabraInvertida(t_palabra* palabra, char *s);
int longitud(t_palabra* palabra);
t_palabra* copiarPalabra(t_palabra* palabra);
void palabraPosterior(t_palabra* palabra, int modo);
void palabraAnterior(t_palabra* palabra, int modo);
int sonIncomparables(t_palabra* palabra1, t_palabra* palabra2);
int compararPalabras(t_palabra* palabra1, t_palabra* palabra2);
int esIgual(t_palabra* palabra1, t_palabra* palabra2);
int esAnterior(t_palabra* palabra1, t_palabra* palabra2);
int esPosterior(t_palabra* palabra1, t_palabra* palabra2);
char caracterI(int i, t_palabra* palabra);
int esPalindromo(t_palabra* palabra);

void liberarStr(char* s);
void liberarCaracteresRecursivamente(t_caracter* c);
void liberarPalabra(t_palabra* palabra);

//----------------------------------------------------------------------------//

#endif
