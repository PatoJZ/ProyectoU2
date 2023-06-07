#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <string>

using namespace std;
struct Guardian
{
    string Name;
    int PowerLevel;
    string MainMaster;
    string Village;
    vector<Guardian> aprendices;
};
struct Village {
    string Name;
    vector<Village> villasConectadas;
};
vector<Guardian> cargarGuardianes(string& nombreArchivo) 
{
    vector<Guardian> guardianes;
    ifstream archivo(nombreArchivo);

    if (archivo.is_open()) 
    {
        Guardian guardian;
        string linea;

        while (getline(archivo, linea)) 
        {
            string token;
            istringstream tokenStream(linea);
            vector<string> tokens;

            while (getline(tokenStream, token, ',')) 
            {
                tokens.push_back(token);
            }

            if (tokens.size() >= 4) 
            {
                guardian.Name = tokens[0];
                guardian.PowerLevel = stoi(tokens[1]);
                guardian.MainMaster = tokens[2];
                guardian.Village = tokens[3];

                guardianes.push_back(guardian);
            }
        }

        cout << "Guardianes cargados exitosamente desde el archivo: " << nombreArchivo << endl;
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }

    archivo.close();

    return guardianes;
}
vector<Village> cargarVillas(const string& nombreArchivo) 
{
    vector<Village> villas;
    ifstream archivo(nombreArchivo);
     if (archivo.is_open()) 
     {
        
        string linea;
        while (getline(archivo, linea)) 
        {
            if (!linea.empty()) {
                istringstream tokenStream(linea);
                string token;
                vector<string> tokens;
                Village villa;

                getline(tokenStream, villa.Name, ',');

                while (getline(tokenStream, token, ',')) {
                    Village villaConectada;
                    villaConectada.Name = token;
                    villa.villasConectadas.push_back(villaConectada);
                }

                villas.push_back(villa);
            }
        }

        

        cout << "Villas cargadas exitosamente desde el archivo: " << nombreArchivo << endl;
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }

    archivo.close();

    return villas;
}
int main() 
{
    vector<Guardian> guardianes;
    vector<Village> villas;

    // Cargar guardianes desde un archivo
    string nombreArchivo = "guardianes.csv";
    string nombreArchivo2 = "villas.csv";
    guardianes = cargarGuardianes(nombreArchivo);
    villas = cargarVillas(nombreArchivo2);

    // Mostrar los guardianes cargados
    for (Guardian& guardian : guardianes) 
    {
        cout << "Nombre: " << guardian.Name << endl;
        cout << "Nivel de poder: " << guardian.PowerLevel << endl;
        cout << "Maestro principal: " << guardian.MainMaster << endl;
        cout << "Aldea: " << guardian.Village << endl;
        cout << endl;
    }
    for (Village& villa : villas) 
    {
        cout << "Nombre de la villa: " << villa.Name << endl;

        if (!villa.villasConectadas.empty()) {
            cout << "Villas conectadas: ";

            for ( Village& villaConectada : villa.villasConectadas) {
                cout << villaConectada.Name << " ";
            }

            cout << endl;
        }

        cout << endl;
    }

    return 0;
}
