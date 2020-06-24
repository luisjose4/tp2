#include "solucion.h"
#include "auxiliares.h"
#include "definiciones.h"

// Ejercicios

bool esSenial(vector<int> s, int prof, int freq) {
    return esValida(s, prof, freq);
}

bool seEnojo(senial s, int umbral, int prof, int freq) {
    return existeSubseqEnojada(s, umbral, prof, freq);
}

bool esReunionValida(reunion r, int prof, int freq) {
    return reunionValida(r, prof, freq);
}

void acelerar(reunion& r, int prof, int freq) {
    for (int i = 0; i < r.size(); ++i) {
        acelerarSenial(r[i].first, prof, freq);
    }
}

void ralentizar(reunion& r, int prof, int freq) {
    for (int i = 0; i < r.size(); ++i) {
        ralentizarSenial(r[i].first, prof, freq);
    }
}

vector<hablante> tonosDeVozElevados(reunion r, int freq, int prof) {
    return hablantesConTonosElevados(r, freq, prof);
}

void ordenar(reunion& r, int freq, int prof) {
    // Complejidad O(m*n + 2m^2) que es menor que O(m^2*n) para m >= 2 y n >= 10
    //n >= 10 si o si ya que si no, no seria senial valida y no cumpliria la precondicion
    // m == 0 o m == 1 el algoritmo no hace nada porque ya estan ordenadas las seniales
    ordenarReunionAcordeAPromedios(r);
}

vector<intervalo> silencios(senial s, int prof, int freq, int umbral)
{   // complejidad O(n)
    vector<intervalo> intervalos;
    intervalos = obtenerSilencios(s, freq, umbral);
    return intervalos;
}

bool hablantesSuperpuestos(reunion r, int prof, int freq, int umbral)
{
    bool resp = false;
//    resp = hayHablantesSuperpuestos(r,freq,umbral);
    return resp;
}

senial reconstruir(senial s, int prof, int freq)
{
    senial a;
    return a;
//    return reconstruirSenial(s);
}

void filtradoMediana(senial &s, int R, int prof, int freq)
{
//    filtrada(s,R);
    return;
}
