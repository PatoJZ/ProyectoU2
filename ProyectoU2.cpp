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
    void printVillages()
    {
        imprimirVillas(villages, guardians);
    }
    string selectApprentice()
    {
        cout << "Lista de aprendices generales:" << endl;
        for (const Guardian *guardian : guardians)
        {
            if (guardian->Name != "Stormheart")
            {
                cout << "- " << guardian->Name << endl;
                cout << "Power level: " << guardian->PowerLevel << endl;
            }
        }

        string name;
        cout << "Ingrese el nombre del aprendiz: ";
        getline(cin, name);

        Guardian *selectedApprentice = findGuardian(name);
        if (selectedApprentice == nullptr)
        {
            cout << "Aprendiz no encontrado: " << name << endl;
            return "";
        }
        else
        {
            cout << "Aprendiz encontrado: " << selectedApprentice->Name << " fue seleccionado" << endl;
            return selectedApprentice->Name;
        }

        Guardian *master = findGuardian(selectedApprentice->MainMaster);
        if (master == nullptr)
        {
            cout << "Master not found for apprentice: " << name << endl;
            return "";
        }

        // Eliminar el aprendiz de la lista de aprendices del maestro
        for (size_t i = 0; i < master->aprendices.size(); i++)
        {
            if (master->aprendices[i]->Name == name)
            {
                master->aprendices.erase(master->aprendices.begin() + static_cast<vector<Guardian *>::difference_type>(i));
                break;
            }
        }

        // Encontrar el siguiente aprendiz con mayor poder
        Guardian *nextApprentice = nullptr;
        int maxPowerLevel = numeric_limits<int>::min();

        for (Guardian *apprentice : master->aprendices)
        {
            if (apprentice->PowerLevel > maxPowerLevel)
            {
                maxPowerLevel = apprentice->PowerLevel;
                nextApprentice = apprentice;
            }
        }

        // Actualizar el MainMaster del aprendiz seleccionado
        selectedApprentice->MainMaster = "";

        // Si hay un siguiente aprendiz, asignarlo como el nuevo MainMaster
        if (nextApprentice != nullptr)
        {
            selectedApprentice->MainMaster = nextApprentice->Name;
        }

        // Actualizar la jerarquía de maestros y aprendices
        updateTreeStructure();
    }
    void travelBetweenVillages(const string &selectedApprentice)
    {
        // Obtener la aldea actual del aprendiz seleccionado
        string currentVillage;
        for (const Guardian *guardian : guardians)
        {
            if (guardian->Name == selectedApprentice)
            {
                currentVillage = guardian->Village;
                break;
            }
        }

        if (currentVillage.empty())
        {
            cout << "El aprendiz seleccionado no tiene una aldea asignada." << endl;
            return;
        }

        cout << "Viajando desde " << currentVillage << "..." << endl;

        while (true)
        {
            // Mostrar las aldeas adyacentes disponibles para viajar
            vector<string> adjacentVillages = getAdjacentVillages(currentVillage);
            cout << "Aldeas disponibles para viajar desde " << currentVillage << ":" << endl;
            for (const string &adjacentVillage : adjacentVillages)
            {
                cout << "- " << adjacentVillage << endl;
            }

            // Solicitar la aldea a la que desea viajar el usuario
            cout << "Ingresa el nombre de la aldea a la que deseas viajar (o 'volver' para regresar a la aldea anterior): ";
            string destinationVillage;
            getline(cin, destinationVillage);

            // Verificar si el usuario desea regresar a la aldea anterior
            if (destinationVillage == "volver")
            {
                cout << "Regresando a la aldea anterior..." << endl;
                return;
            }

            // Verificar si la aldea ingresada es adyacente a la aldea actual
            bool isValidDestination = false;
            for (const string &adjacentVillage : adjacentVillages)
            {
                if (adjacentVillage == destinationVillage)
                {
                    isValidDestination = true;
                    break;
                }
            }

            if (isValidDestination)
            {
                cout << "Viajando desde " << currentVillage << " hacia " << destinationVillage << "..." << endl;
                currentVillage = destinationVillage;
            }
            else
            {
                cout << "La aldea ingresada no es una opción válida para viajar desde " << currentVillage << "." << endl;
            }
        }
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
    // Función para obtener las aldeas adyacentes a una aldea dada
    vector<string> getAdjacentVillages(const string &village)
    {
        vector<string> adjacentVillages;

        for (const Village *villageObj : villages)
        {
            if (villageObj->Name == village)
            {
                for (const Village *adjacentVillage : villageObj->villasConectadas)
                {
                    adjacentVillages.push_back(adjacentVillage->Name);
                }
                break;
            }
        }

        return adjacentVillages;
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

    int opcion;
    string selectedApprentice;
    do
    {
        // Mostrar el menú
        cout << "----- Menú Principal -----" << endl;
        cout << "1. Seleccionar aprendiz" << endl;
        cout << "2. Mostrar guardianes y aldeas" << endl;
        cout << "3. Viajar entre aldeas" << endl;
        cout << "0. Salir" << endl;
        cout << "Ingrese el número de la opción deseada: ";

        cin >> opcion;

        switch (opcion)
        {
        case 1:
            selectedApprentice = guardians.selectApprentice();
            break;
        case 2:
            guardians.printGuardians();
            guardians.printVillages();
            break;
        case 3:
            guardians.travelBetweenVillages(selectedApprentice);
            break;
        case 0:
            cout << "Saliendo del programa..." << endl;
            return 0;
        default:
            cout << "Opción inválida. Por favor, ingrese un número válido." << endl;
            break;
        }

        cout << endl;

    } while (opcion != 0);

    return 0;
}
