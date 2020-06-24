#include "../lib/gtest.h"
#include "definiciones.h"
#include "auxiliares.h"
#include <fstream>

/************* esSenial *************/
bool freqValida(int freq)
{
    return freq == 10;
}

bool enRango(senial s, int prof)
{
    int umbralSuperior = pow(2, prof - 1) - 1;
    int umbralInferior = -pow(2, prof - 1);
    bool res = true;

    for (int i = 0; i < s.size() && res; i++) {
        res = ( umbralInferior <= s[i]  &&  s[i] <= umbralSuperior );
    }

    return res;
}

bool profValida(int prof)
{
    return prof == 8 || prof == 16 || prof == 32;
}

bool duraMasDe(senial s, int freq, float seg)
{
    return s.size() >= freq * seg;
}

bool esValida(senial s, int prof, int freq)
{
    return freqValida(freq)  &&  enRango(s, prof)  &&  profValida(prof)  &&  duraMasDe(s, freq, 1);
}


/************* seEnojo **************/
float tono(senial s)
{
    float sum = 0;

    for (int i = 0; i < s.size(); i++) {
        sum += abs(s[i]);
    }

    return sum / s.size();
}

bool superaUmbral(senial s, int umbral)
{
    return tono(s) > umbral;
}

bool existeSubseqEnojada(senial s, int umbral, int prof, int freq)
{
    int cantMinimaDeElementos = 2 * freq;

    if ( s.size() < cantMinimaDeElementos ) return false;

    bool res = superaUmbral(s, umbral);

    for (int i = 0; ( i <= s.size() - cantMinimaDeElementos ) && !res; ++i) {
        for (int j = i + cantMinimaDeElementos; ( j <= s.size() ) && !res; ++j) {
            senial subseq = senial(s.begin() + i, s.begin() + j);

            res = superaUmbral(subseq, umbral);
        }
    }

    return res;
}


/************ esReunionValida ******/
bool esMatriz(reunion r)
{
    bool res = true;
    senial senialModelo = r[0].first;

    for (int i = 1; i < r.size() && res; ++i) {
        senial senial_i = r[i].first;

        res = ( senialModelo.size() == senial_i.size() );
    }

    return res;
}

bool senialesValidas(reunion r, int prof, int freq)
{
    bool res = true;

    for (int i = 0; i < r.size() && res; ++i) {
        res = esValida(r[i].first, prof, freq);
    }

    return res;
}

bool todosHablantesDistintos(reunion r)
{
    bool res = true;

    for (int i = 0; i < r.size() && res; ++i) {
        for (int j = i + 1; j < r.size() && res; ++j) {
            res = ( r[i].second != r[j].second );
        }
    }

    return res;
}

bool hablantesDeReunionValidos(reunion r, int prof, int freq)
{
    bool res = todosHablantesDistintos(r);

    for (int i = 0; i < r.size() && res; ++i) {
        res = ( 0 <= r[i].second  &&  r[i].second < r.size() );
    }

    return res;
}

bool reunionValida(reunion r, int prof, int freq)
{
    return r.size() > 0  &&  esMatriz(r)  &&  senialesValidas(r, prof, freq)  &&  hablantesDeReunionValidos(r, prof, freq);
}


/************ acelerar *************/
void acelerarSenial(senial &s, int prof, int freq)
{
    for (int i = 0; i < s.size(); ++i) {
        s.erase(s.begin() + i);
    }
}


/*********** ralentizar ************/
void ralentizarSenial(senial &s, int prof, int freq)
{
    int longFinal = 2 * s.size() - 1;
    senial salida(longFinal, 0);

    for (int i = 0; i < salida.size(); ++i) {
        if (i % 2 == 0) {
            salida[i] = s[i / 2];
        } else {
            salida[i] = (s[(i - 1) / 2] + s[(i + 1) / 2]) / 2;
        }
    }

    s = salida;
}


/********* tonosDeVozElevados ******/
vector<hablante> hablantesConTonosElevados(reunion r, int freq, int prof)
{
    vector<hablante> hablantes = { };
    float mayorTono = 0;

    for (int i = 0; i < r.size(); ++i) {
        float iesimoTono = tono(r[i].first);

        if (iesimoTono > mayorTono) {
            mayorTono = iesimoTono;
            hablantes = { r[i].second };
        }
        else if (iesimoTono == mayorTono) {
            hablantes.push_back(r[i].second);
        }
    }

    return hablantes;
}


/************* ordenar *************/
void agregarParHablanteTono(vector<pair<hablante, float>> &listaDeTonos, reunion r, int posicion)
{
    pair<hablante, float> parHablanteTono;
    parHablanteTono.first = r[posicion].second;
    parHablanteTono.second = tono(r[posicion].first);

    listaDeTonos.push_back(parHablanteTono);
}

void ordenarReunionAcordeAPromedios(reunion &r)
{
    vector<pair<hablante, float>> listaDeTonos;

    if (r.size() == 0 || r.size() == 1) {
        return;
    }

    for (int i = 0; i < r.size(); i++) {
        agregarParHablanteTono(listaDeTonos, r, i);

        //insertion sort para ordenar la listaDeTonos por numero de hablante
        for (int j = i; j > 0 && listaDeTonos[j - 1].first > listaDeTonos[j].first; j--) {
            iter_swap(listaDeTonos.begin() + j - 1, listaDeTonos.begin() + j);
        }
    }

    
    for (int i = 1; i < r.size(); i++) {
        //insertion sort
        for (int j = i; j > 0 && listaDeTonos[r[j - 1].second].second < listaDeTonos[r[j].second].second; j--) {
            iter_swap(r.begin() + j - 1, r.begin() + j);
        }
    }
    
}


/************* silencios *************/
bool superaUmbral(int valor, int umbral)
{
    return abs(valor) >= umbral;
}

void actualizarIndicesYFlags(int &outInicioSilencio, int indiceActual, bool &outCandidatoSilencio, bool &outEsSilencio, int freq)
{
    if ( !outCandidatoSilencio ) {
        outInicioSilencio = indiceActual;
    }

    outCandidatoSilencio = true;

    if (indiceActual - outInicioSilencio + 1 >= freq * 0.2) {
        outEsSilencio = true;
    }
}

void agregarIntervalo(vector<intervalo> &listaDeSilencios, int inicioSilencio, int finSilencio)
{
    intervalo silencio;

    silencio.first = inicioSilencio;
    silencio.second = finSilencio;

    listaDeSilencios.push_back(silencio);
}

vector<intervalo> obtenerSilencios(senial s, int freq, int umbral)
{
    vector<intervalo> ret(0);
    int inicioSilencio = 0;
    bool candidatoASilencio = false;
    bool esSilencio = false;


    for (int i = 0; i < s.size(); i++) {

        if (superaUmbral(s[i], umbral) == false) {
            actualizarIndicesYFlags(inicioSilencio, i, candidatoASilencio, esSilencio, freq);
        }

        else {
            if (esSilencio) {
                agregarIntervalo(ret, inicioSilencio, i - 1);
            }
            esSilencio = false;
            candidatoASilencio = false;
        }
    }


    if (esSilencio) { /* agregar silencio en caso de que termine al final */
        agregarIntervalo(ret, inicioSilencio, s.size() - 1);
    }

    return ret;
}


/****** hablantesSuperpuestos ********/
//bool haySilencioQueLoContiene(senial s, int i, int freq, int umbral)
//{
//    for (int j = 0; j < s.size(); ++j) {
//        for (int k = j + 1; k < s.size(); ++k) {
//            if (j <= i && k >= i && esSilencio(s, j, k, freq, umbral)) {
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//bool seRespetan(reunion r, int h1, int h2, int freq, int umbral)
//{
//    bool res = true;
//
//    for (int i = 0; i < r[h1].first && res; ++i) {
//        if (!(haySilencioQueLoContiene(r[h1].first, i, freq, umbral))) {
//            if (!haySilencioQueLoContiene(r[h2].first, i, freq, umbral)) {
//                res = false;
//            }
//        }
//    }
//}
//
//bool hayHablantesSuperpuestos(reunion r, int freq, int umbral)
//{
//    for (int h1 = 0; h1 < r.size(); ++h1) {
//        for (int h2 = h1 + 1; h2 < r.size(); ++h2) {
//            if (!(seRespetan(r, h1, h2, freq, umbral))) {
//                return true;
//            }
//        }
//
//    }
//}
//
//
///*********** reconstruir *************/
//int signo(int k)
//{
//    if (k == 0)
//        return 0;
//
//    return (k > 0) ? 1 : -1;
//}
//
//int distancia(int j, int i)
//{
//    return (j - i > 0) ? (j - i) : (i - j);
//}
//
//int valor(senial s, int i)
//{
//    int j = i - 1;
//    int k = i + 1;
//    for (j; j != 0 && s[j] == 0; --j) {
//
//    }
//    for (k; k != s.size(); ++k) {
//    // nada, solo queremos que vaya sumando.
//    }
//
//    return (s[k] + s[j]) / 2;
//}
//
//bool esPasajePorCero(senial s, int i)
//{
//    return signo(s[i - 1]) * signo(s[i + 1]) == -1;
//}
//
//bool masCercanosNoNulos(senial s, int i, int j, int k)
//{
//    return distancia(j, k) <= 5;
//}
//
//bool esValorEnPosicion(senial s, int valor, int i)
//{
//    int j = i - 1;
//    int k = i + 1;
//    while (j != 0 && s[j] == 0) {
//        --j;
//    }
//    while (k != s.size() && s[k] == 0) {
//        ++k;
//    }
//
//    return masCercanosNoNulos(s, i, j, k);
//}
//
//bool reconstruirPosicionSiCorresponde(senial s, int i)
//{
//    return (esPasajePorCero(s, i) && s[0] == 0) || (!esPasajePorCero(s, i) && esValorEnPosicion(s, i));
//}
//
//senial reconstruirSenial(senial s)
//{
//    for (int i = 0; i < s.size(); ++i) {
//        if (s[i] == 0) {
//            if (esPasajePorCero(s, i) && s[0] = 0) {
//                // pasa al siguiente elemento.                                         queda raro...
//            } else if (!esPasajePorCero(s, i) && esValorEnPosicion(s, s[i], i)) {
//                s[i] = valor(s, i);
//            }
//        }
//    }
//
//    return s;
//}
//
//
///************* friltradoMediana *************/
//bool coincidenExtremos(senial s, int i, int r)
//{
//    return i < r || i >= s.size() - r;
//}
//
//void swap(senial &lista, int i, int j)
//{
//    int k = lista[i];
//    lista[i] = lista[j];
//    lista[j] = k;
//}
//
//void insertar(senial &lista, int i)
//{
//    while (i > 0 && lista[i] < lista[i - 1]) {
//        swap(lista, i, i - 1);
//        i--;
//    }
//}
//
//senial insertionSort(senial lista)
//{
//    for (int i = 0; i < lista.size(); i++) {
//        insertar(lista, i);
//    }
//    return lista;
//}
//
//senial ordenarSenialW(senial w)
//{
//    return insertionSort(w);
//}
//
//senial subSec(senial s, int i, int r)
//{
//    senial
//    w = [];
//
//    for (int j = i; j < r; ++j) {
//        w.push_back(s[j]);
//    }
//}
//


/************* Ejercicio 4 *************/
senial leerSenial(string nombreArchivo)
{
    senial s(0);
    string StringDeLaSenial;
    ifstream inputArchivo;


    inputArchivo.open(nombreArchivo.c_str(), ios_base::in);
    if (inputArchivo.is_open() == false) {
        return s;
    }

    getline(inputArchivo, StringDeLaSenial);

    for (int k = 0; k < StringDeLaSenial.size(); k++) {

        int numeroDeSenial = 0;

        if (StringDeLaSenial[k] != ' ') {
            numeroDeSenial *= 10;
            numeroDeSenial += StringDeLaSenial[k] - '0';
        }
        else {
            s.push_back(numeroDeSenial);
            numeroDeSenial = 0;
        }
    }

    inputArchivo.close();
    return s;
}

void escribirSenial(senial s, string nombreArchivo)
{
    ofstream outputArchivo;

    outputArchivo.open(nombreArchivo.c_str(), ios_base::app);
    if ( !outputArchivo.is_open() ) {
        return;
    }

    for (int k = 0; k < s.size(); k++) {
        outputArchivo << s[k] << ' ';
    }

    outputArchivo.close();
}

/************* Ejercicio 4 *************/



bool senialesOrdenadasIguales(senial s1, senial s2){

    if(s1.size() != s2.size())
        return false;

    for (int i = 0; i < s1.size(); ++i){
        if (s1[i] != s2[i]){
        	return false;
        }
    }

    return true;
}

bool reunionesIguales(reunion reunion1, reunion reunion2){
    if(reunion1.size() != reunion2.size())
        return false;

    for (int i = 0; i < reunion1.size(); ++i) {
        if(reunion1[i].second != reunion2[i].second)
            return false;

        if(reunion1[i].first.size() != reunion2[i].first.size())
            return false;

        if(!senialesOrdenadasIguales(reunion1[i].first, reunion2[i].first))
            return false;
    }

    return true;
}

bool hablantesOrdenadosIguales(vector<hablante> s1, vector<hablante> s2){
    if(s1.size() != s2.size())
        return false;

    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());

    for (int i = 0; i < s1.size(); ++i)
        if (s1[i] != s2[i]){return false;}

    return true;
}

bool intervalosOrdenadosIguales(vector<intervalo> s1, vector<intervalo> s2){
    if(s1.size() != s2.size())
        return false;

    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());

    for (int i = 0; i < s1.size(); ++i)
        if (s1[i] != s2[i]){return false;}

    return true;
}

void ASSERT_SENIAL_EQ(senial s1, senial s2) {
    ASSERT_TRUE(senialesOrdenadasIguales(s1, s2));
}

void ASSERT_REUNION_EQ(reunion reunion1, reunion reunion2) {
    ASSERT_TRUE(reunionesIguales(reunion1, reunion2));
}

void ASSERT_HABLANTES_EQ(vector<hablante> s1, vector<hablante> s2) {
    ASSERT_TRUE(hablantesOrdenadosIguales(s1, s2));
}

void ASSERT_INTERVALOS_EQ(vector<intervalo> s1, vector<intervalo> s2) {
    ASSERT_TRUE(intervalosOrdenadosIguales(s1, s2));
}
