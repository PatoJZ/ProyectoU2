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
    vector<Guardian*> aprendices;
};
struct Village
{
    string Name;
    vector<Village> villasConectadas;
};

class GuardiansTree
{
public:
    GuardiansTree() : root(nullptr) {}

    void insertGuardian(const string &name, const string& powerLevel, const string &mainMaster, const string &village)
    {
        Guardian *guardian = new Guardian;
        guardian->Name = name;
        guardian->PowerLevel = stoi(powerLevel);
        guardian->MainMaster = mainMaster;
        guardian->Village = village;
        guardians.push_back(guardian);

        // If the mainMaster Guardian is found, add the guardian as an Aprendiz (apprentice)
        if (root == nullptr)
        {
            root = guardian;
            cout << "root" << endl;
        }
        else
        {
            Guardian *master = findGuardian(mainMaster);
            if (master != nullptr)
            {
                master->aprendices.push_back(guardian);
            }
        }
    }

    void loadGuardiansFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }
        string line;

        while (getline(file, line))
        {
            istringstream iss(line);
            string name, mainMaster, village;
            string powerLevel = "0";
            getline(iss, name, ',');
            getline(iss, powerLevel, ',');
            getline(iss, mainMaster, ',');
            getline(iss, village);
            insertGuardian(name, powerLevel, mainMaster, village);
        }
        file.close();
    }

    void printGuardians()
    {
        printGuardian(root, 0);
    }

private:
    vector<Guardian *> guardians;
    Guardian *root;

    Guardian *findGuardian(const string &name)
    {
        for (Guardian *guardian : guardians)
        {
            if (guardian->Name == name)
            {
                return guardian;
            }
        }
        return nullptr;
    }

    void updateTreeStructure()
    {
        for (Guardian *guardian : guardians)
        {
            if (guardian->MainMaster != "")
            {
                Guardian *master = findGuardian(guardian->MainMaster);
                if (master != nullptr)
                {
                    master->aprendices.push_back(guardian);
                }
            }
            else
            {
                root = guardian;
            }
        }
    }

    void printGuardian(Guardian *guardian, unsigned int indent)
    {
        if (guardian != nullptr)
        {
            cout << string(indent, ' ') << "- " << guardian->Name << " (Power Level: " << guardian->PowerLevel << ", Village: " << guardian->Village << ")" << endl;
            for (Guardian *aprendiz : guardian->aprendices)
            {
                printGuardian(aprendiz, indent + 4);
            }
        }
    }
    vector<Village> cargarVillas(const string &nombreArchivo)
    {
        vector<Village> villas;
        ifstream archivo(nombreArchivo);
        if (archivo.is_open())
        {

            string linea;
            while (getline(archivo, linea))
            {
                if (!linea.empty())
                {
                    istringstream tokenStream(linea);
                    string token;
                    vector<string> tokens;
                    Village villa;

                    getline(tokenStream, villa.Name, ',');

                    while (getline(tokenStream, token, ','))
                    {
                        Village villaConectada;
                        villaConectada.Name = token;
                        villa.villasConectadas.push_back(villaConectada);
                    }

                    villas.push_back(villa);
                }
            }

            cout << "Villas cargadas exitosamente desde el archivo: " << nombreArchivo << endl;
        }
        else
        {
            cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        }

        archivo.close();

        return villas;
    }
};

int main()
{
    GuardiansTree guardianes;
    //vector<Village> villas;

    // Cargar guardianes desde un archivo
   
    //string nombreArchivo2 = "villas.csv";
    guardianes.loadGuardiansFromFile("guardianes.csv");
    //villas = cargarVillas(nombreArchivo2);

    // Mostrar los guardianes cargados
    guardianes.printGuardians();
    /*for (Village &villa : villas)
    {
        cout << "Nombre de la villa: " << villa.Name << endl;

        if (!villa.villasConectadas.empty())
        {
            cout << "Villas conectadas: ";

            for (Village &villaConectada : villa.villasConectadas)
            {
                cout << villaConectada.Name << " ";
            }

            cout << endl;
        }

        cout << endl;
    }
    */
    return 0;
}
