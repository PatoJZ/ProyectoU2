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
    vector<Guardian *> aprendices;
};
struct Village
{
    string Name;
    string MainMaster;
    vector<Village *> villasConectadas;
};

class GuardiansTree
{
public:
    GuardiansTree() : root(nullptr) {}
    vector<Guardian *> getGuardians()
    {
        return guardians;
    }
    void insertGuardian(const string &name, const string &powerLevel, const string &mainMaster, const string &village)
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
    void loadVillagesFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }
        string line;

        // Leer el archivo y cargar las villas
        vector<Village *> loadedVillages = cargarVillas(filename);

        // Actualizar el vector villages
        villages = loadedVillages;
        imprimirVillas(villages, guardians);

        file.close();
    }

    void printGuardians()
    {
        printGuardian(root, 0);
    }

private:
    vector<Guardian *> guardians;
    vector<Village *> villages;
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
    vector<Village *> cargarVillas(const string &filename)
    {
        vector<Village *> updatedVillages;
        unordered_map<string, Village *> villageMap; // Mapa para evitar duplicación de villas
        ifstream file(filename);
        if (!file)
        {
            cerr << "Failed to open file: " << filename << endl;
            return updatedVillages;
        }
        string line;

        while (getline(file, line))
        {
            istringstream iss(line);
            string villageName, neighborCity;
            getline(iss, villageName, ',');
            getline(iss, neighborCity);

            Village *village = nullptr;
            auto villageIter = villageMap.find(villageName);
            if (villageIter != villageMap.end())
            {
                village = villageIter->second; // Recuperar la instancia existente
            }
            else
            {
                village = new Village; // Crear nueva instancia de la villa
                village->Name = villageName;
                village->MainMaster = "";
                villageMap[villageName] = village;  // Agregar al mapa
                updatedVillages.push_back(village); // Agregar al vector de villas actualizadas
            }

            // Buscar el MainMaster correspondiente en el vector de guardianes
            for (Guardian *guardian : guardians)
            {
                if (guardian->Village == villageName)
                {
                    village->MainMaster = guardian->Name;
                    break;
                }
            }
            auto neighborIter = villageMap.find(neighborCity);
            if (neighborIter != villageMap.end())
            {
                Village *neighbor = neighborIter->second;
                village->villasConectadas.push_back(neighbor);
            }
        }
        file.close();

        villages = updatedVillages;
        return updatedVillages;
    }
    void imprimirVillas(const vector<Village *> &villas, vector<Guardian *> &updatedGuardians, unsigned int indent = 0)
    {
        for (const Village *villa : villas)
        {
            cout << string(indent, ' ') << "Nombre de la villa: " << villa->Name << endl;
            cout << string(indent, ' ') << "Maestro principal: " << villa->MainMaster << endl;

            if (!villa->villasConectadas.empty())
            {
                cout << string(indent, ' ') << "Villas conectadas: ";
                for (const Village *villaConectada : villa->villasConectadas)
                {
                    cout << villaConectada->Name << " ";
                }
                cout << endl;
            }

            cout << string(indent, ' ') << "Guardianes asociados:" << endl;
            for (const Guardian *guardian : updatedGuardians)
            {
                if (guardian->MainMaster == villa->MainMaster)
                {
                    cout << string(indent + 4, ' ') << "Nombre: " << guardian->Name << endl;
                    cout << string(indent + 4, ' ') << "Nivel de poder: " << guardian->PowerLevel << endl;
                    cout << string(indent + 4, ' ') << "Maestro principal: " << guardian->MainMaster << endl;
                    cout << endl;
                }
            }

            cout << endl;
        }
    }
};

int main()
{
    GuardiansTree guardians;

    // Cargar guardianes desde un archivo
    guardians.loadGuardiansFromFile("guardianes.csv");

    // Mostrar los guardianes cargados
    guardians.printGuardians();

    // Cargar villas desde un archivo y actualizar los main master de los guardianes
    guardians.loadVillagesFromFile("villas.csv");

    return 0;
}
