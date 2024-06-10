// funciones.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <windows.h>
#include <limits>
#include "funciones.h"

using namespace std;

void delayRoutine() {
    cout << "Ejecutando rutina de interrupción: Delay..." << endl;
    Sleep(1000); // Pausa de 1 segundo
    cout << "Rutina de interrupción finalizada." << endl;
}

void monitorMode(InterruptVector &iv, int interruptNumber) {
    if (interruptNumber >= 0 && interruptNumber < iv.positions.size()) {
        cout << "Cambiando a modo monitor." << endl;
        iv.routines[interruptNumber]();
        cout << "Regresando a modo usuario." << endl;
    }
}

void userMode(const string &filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "No se puede abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    Mode mode = USER_MODE;
    InterruptVector iv;
    bool lastWrite = false;
    int lastWriteLine = -1;

    iv.positions.push_back(0);
    iv.routines.push_back(delayRoutine);

    while (getline(infile, line)) {
        cout << "Leyendo línea " << lineNumber + 1 << ": " << line << endl;

        if (line.find("write('tiempo de ejecucion',") != string::npos) {
            // Obtener el tiempo de ejecución (en milisegundos)
            size_t inicio = line.find(("write('tiempo de ejecucion','") + strlen("write('tiempo de ejecucion','"));
            size_t fin = line.find("',");
            string tiempoStr = line.substr(inicio, fin - inicio);
            int tiempo = stoi(tiempoStr);

            // Realizar delay
            cout << "Realizando delay de " << tiempo << " milisegundos..." << endl;
            Sleep(tiempo); // Pausa por el tiempo especificado
            monitorMode(iv, iv.positions.size() - 1);

            // Guardar la dirección de memoria y la siguiente línea del archivo
            lastWrite = true;
            lastWriteLine = lineNumber + 1;
        }

        if (lastWrite && lastWriteLine == lineNumber) {
            // Si el programa detecta un write en la última línea, finalizar el programa
            break;
        }

        // Marcar la línea ejecutada
        fstream outfile(filename, ios::in | ios::out);
        if (outfile.is_open()) {
            outfile.seekp(ios::beg);
            for (int i = 0; i < lineNumber; ++i) {
                outfile.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            outfile << "*" << line << endl;
        }
        outfile.close();

        // Guardar el valor de PC para la siguiente ejecución
        int pc = lineNumber + 1;
        cout << "Guardando el valor de PC: " << pc << endl;

        ++lineNumber;
    }
    infile.close();
}
