//
// Created by Luijo on 13/06/2020.
//

#include "funciones_auxiliares.h"

/************* Ejercicio 1 - esSenial *************/

bool esValida(senial s, int prof, int freq){
    return freqValida(freq) && enRango(s, prof) && profValida(prof) && duraMasDe(s, freq, 1);
}

bool freqValida(int freq){
    return freq == 10; //preguntar si efectivamente esto es 10Hz o 10kHz
}

bool enRango(senial s, int prof){
    int umbralSuperior = pow(2, prof - 1) - 1;
    int umbralInferior = - pow(2, prof - 1);
    bool res = true;
    for (int i = 0; i < s.size() && res; i++)
    {
        res = res && (umbralInferior <= s[i]) && (s[i] <= umbralSuperior);
    }
    return res;
}

bool profValida(int prof){
    return prof == 8 || prof == 16 || prof == 32;
}

bool duraMasDe(senial s, int freq, float seg){//preguntar
    return s.size() >= freq * seg;
}

/************* Ejercicio 2 - seEnojo *************/

float tono(senial s){
    float sum = 0;
    for (int i = 0; i < s.size(); i++)
    {
        sum = sum + abs(s[i]);	//hace falta pasarlo a double??
    }
    return sum / s.size();
}

bool superaUmbral(senial s, int umbral){
    return tono(s) > umbral;
}

bool existeSubseqEnojada(senial s, int umbral, int prof, int freq){
    for (int i = 0; i < s.size() - 2*freq; ++i) {
        for (int j = i + 2*freq; j < s.size(); ++j) {
            senial subseq = senial ( s.begin() + i, s.begin() + j );
            if ( superaUmbral(subseq, umbral) ) return true;
        }
    }
    return false;
}

/*
bool umbralValido(int umbral){
  return umbral > 0;
}
*/

/************* Ejercicio 3 - esReunionValida *************/

bool esMatriz(vector<pair<senial, hablante>> r){
    for (int i = 0; i < r.size(); ++i) {
        if ( typeid(r[i]) != typeid(pair<senial, hablante>) ) return false;
    } //comparo los tipos (que sean de tipo pair<senial, hablante>
    return true;
}//esta bien que sea asi la entrada? me parece que entonces no tendria sentido ver si es matriz
//preguntar si mas bien no seria comprobar la longitud de las señales

bool senialesValidas(reunion r, int prof, int freq){
    for (int i = 0; i < r.size(); ++i) {
        if ( !esValida(r[i].first, prof, freq) ) return false;
    }
    return true;
}

bool todosHablantesDistintos(reunion r){
    for (int i = 0; i < r.size(); ++i) {
        for (int j = i + 1; j < r.size(); ++j) {
            if ( r[i].second == r[j].second ) return false;
        }
    }
    return true;
}

bool hablantesDeReunionValidos(reunion r, int prof, int freq) {//para que recibe aca prof y freq??
    bool res = todosHablantesDistintos(r);
    for (int i = 0; i < r.size() && res; ++i) {
        res = res && (0 <= r[i].second) && (r[i].second < r.size());
    }
    return res;
}

bool reunionValida(reunion r, int prof, int freq){
    return (r.size() > 0) && esMatriz(r) && senialesValidas(r, prof, freq) && hablantesDeReunionValidos(r, prof, freq);
}

/************* Ejercicio 4 - acelerar *************/

void acelerarSenial(senial& s, int prof, int freq){
    int i = 0;
    while ( i < s.size() ){
        s.erase(s.begin()+i);
        i = i + 1;//es i+1 porque al hacer el erase modifico la longitud de s y no necesito ir sumando 2
    }
    return;
}

/************* Ejercicio 9 - hablantesSuperpuestos *************/

int valorAbsoluto(int x){
    if(x<0){
        return x=-x;
    }
}

bool hayHablantesSuperpuestos(reunion r,int freq,int umbral){
    for (int i = 0; i < r[0].first.size(); ++i) {
        int personasHablando = 0;
        for (int j = 0; j < r.size(); ++j) {
            if(valorAbsoluto(r[j].first[i]) >= umbral){
                ++personasHablando;
            }
            if (personasHablando == 2){
                return true;
            }
        }

    }
    return false;
}





/************* Ejercicio 10 - reconstruir *************/

int valor(vector<int> s, int i){
    int m = 0;
    int n = 0;
    for (int j = i-1; j!=0 && n==0 ; --j) {
        if(s[j] == 0){
            // skip
        } else {
            n = j;
        }
    }
    for (int k = i+1; k!=s.size() && m==0 ; ++k) {
        if(s[k] == 0){
            // skip
        } else {
            m = k;
        }
    }

    return (s[n] + s[m]) / 2;
}



int signo(int k){
    if(k > 0){
        return 1;
    } else if(k<0){
        return -1;
    }else{
        return 0;
    }
}

bool esPasajePorCero(vector<int> s, int i){
    return signo(s[i - 1])* signo(s[i + 1]) == -1;
}

senial reconstruirSenial(senial s){
    vector <int> sCero = s;
    for (int i = 0; i <= s.size() ; ++i) {
        if (s[i] == 0){
            if(esPasajePorCero(sCero, i)){
                // skip
            } else {
                s[i] = valor(sCero,i);
            }
        }
    }

    return s;
}





/************* Ejercicio 11 - friltradoMediana *************/

bool coincidenExtremos(senial s,int i,int r){
    return i < r || i>= s.size() - r;
}

senial subSec(senial s,int i, int r){
    senial w;
    for (int j = i; j < r ; ++j) {
        w.push_back(s[j]);
    }
    return w;
}

void swap ( senial &lista , int i, int j) {
    int k= lista [i];
    lista [i]= lista [j];
    lista [j]=k;
}


void insertar ( senial &lista , int i) {
    while (i > 0 && lista [i] < lista [i -1]) {
        swap (lista ,i,i -1) ;
        i --;
    }
}

senial insertionSort (senial lista ) {
    for(int i=0; i < lista . size () ; i++) {
        insertar (lista ,i) ;
    }
    return lista ;
}
senial ordenarSenialW (senial w){
    return insertionSort(w);
}


senial filtrada(senial s, int r){ //O(n).                               ejercicio 3.
    senial w; // 1
    senial sCero = s;
    for(int i = 0; i < s.size(); ++i) { // n
        if(!coincidenExtremos(sCero,i,r)){ // 1
            w = ordenarSenialW(subSec(sCero,i-r,i+r+1)); // 1
            s[i]=w[r]; // 1
        }
    }
    return s;
}

