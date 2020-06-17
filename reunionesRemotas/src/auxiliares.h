#ifndef REUNIONESREMOTAS_AUXILIARES_H
#define REUNIONESREMOTAS_AUXILIARES_H

#include "definiciones.h"
#include <iostream>

senial leerSenial(string nombreArchivo);
void escribirSenial(senial  s, string nombreArchivo);

/************* Ejercicio 4 *************/

vector<pair<hablante, float> > obtenerReunionYSusPromedios(reunion r);

void ordenarPromedios(vector<pair<hablante, float> > &promedios);

void ordenarReunionAcordeAPromedios(reunion &r, vector<pair<hablante, float> > promediosOrdenados);

/************* Ejercicio 4 *************/



void ASSERT_SENIAL_EQ(senial s1, senial s2);
void ASSERT_REUNION_EQ(reunion reunion1, reunion reunion2);
void ASSERT_HABLANTES_EQ(vector<hablante> s1, vector<hablante> s2);
void ASSERT_INTERVALOS_EQ(vector<intervalo> s1, vector<intervalo> s2);

#endif //REUNIONESREMOTAS_AUXILIARES_H
