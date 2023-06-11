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
        cin.ignore();
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
        return "";
    }
    void travelBetweenVillages(const string &selectedApprentice)
    {
        // Obtener la aldea actual del aprendiz seleccionado
        string currentVillage;
        string destinationVillage;
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

        cout << "Viajando con " << selectedApprentice << endl;
        cout << "Viajando desde " << currentVillage << "..." << endl;

        while (true)
        {
            // Mostrar las aldeas adyacentes disponibles para viajar
            vector<string> adjacentVillages = getAdjacentVillages(currentVillage);
            cout << "Aldeas disponibles para viajar desde " << currentVillage << ":" << endl;

            // Mostrar las opciones numeradas para cada aldea adyacente
            for (size_t i = 0; i < adjacentVillages.size(); ++i)
            {
                cout << i + 1 << ". " << adjacentVillages[i] << endl;
            }
            cout << "0. Volver a la aldea anterior" << endl;

            // Solicitar la opcion al usuario
            int option;
            cout << "Selecciona el numero de la aldea a la que deseas viajar: ";
            cin >> option;

            // Verificar la opcion seleccionada
            if (option == 0)
            {
                cout << "Regresando a la aldea anterior..." << endl;
                return;
            }
            else if (option >= 1 && option <= static_cast<int>(adjacentVillages.size()))
            {
                destinationVillage = adjacentVillages[static_cast<size_t>(option - 1)];
                cout << "Viajando desde " << currentVillage << " hacia " << destinationVillage << "..." << endl;

                // Actualizar los puntos de poder por llegar a la aldea
                Guardian *selectedGuardian = findGuardian(selectedApprentice);
                selectedGuardian->PowerLevel += 1;

                // Mostrar los oponentes en la aldea y realizar enfrentamientos
                vector<Guardian *> opponents = getOpponents(destinationVillage);
                if (!opponents.empty())
                {
                    encounterOpponents(selectedGuardian, opponents);
                }

                currentVillage = destinationVillage;
            }
            else
            {
                cout << "Opcion invalida. Por favor, selecciona un numero valido." << endl;
            }

            // Limpiar el buffer de entrada
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    void encounterOpponents(Guardian *selectedGuardian, const vector<Guardian *> &opponents)
    {
        cout << "Enfrentando a los oponentes..." << endl;

        // Mostrar los oponentes disponibles para enfrentar
        cout << "Oponentes disponibles:" << endl;
        for (size_t i = 0; i < opponents.size(); i++)
        {
            cout << i + 1 << ". " << opponents[i]->Name << endl;
            cout << "Poder: " << opponents[i]->PowerLevel << endl;
        }

        // Sugerir enfrentarse al oponente con menor nivel de poder
        size_t  minPowerIndex = 0;
        int minPower = opponents[0]->PowerLevel;
        for (size_t i = 1; i < opponents.size()- 1; i++)
        {
            if (opponents[i]->PowerLevel < minPower)
            {
                minPower = opponents[i]->PowerLevel;
                minPowerIndex = i;
            }
        }
        cout << "Sugerencia: Enfrentarse a " << opponents[static_cast<size_t>(minPowerIndex)]->Name << " (Poder: " << minPower << ")" << endl;

        // Solicitar al usuario el numero del oponente al que desea enfrentarse
        vector<Guardian*>::size_type chosenOpponent;
        cout << "Ingrese el numero del oponente al que desea enfrentarse: ";
        cin >> chosenOpponent;

        // Verificar si la opcion seleccionada es valida
        if (chosenOpponent < 1 || chosenOpponent > opponents.size())
        {
            cout << "Opcion invalida. Fin del enfrentamiento." << endl;
            return;
        }

        // Obtener el oponente seleccionado
        Guardian *opponent = opponents[chosenOpponent - 1];

        // Calcular las posibilidades de victoria
        int selectedPower = selectedGuardian->PowerLevel;
        int opponentPower = opponent->PowerLevel;
        double winProbability = static_cast<double>(selectedPower) / (selectedPower + opponentPower) * 100.0;

        // Realizar el enfrentamiento
        cout << "Enfrentandose a " << opponent->Name << "..." << endl;

        // Tirar un dado para determinar el resultado del enfrentamiento
        int diceRoll = rand() % 100 + 1; // Generar un numero aleatorio entre 1 y 100

        // Verificar el resultado del dado comparando con las posibilidades de victoria
        if (diceRoll <= winProbability)
        {
            cout << "¡Has ganado el enfrentamiento! Obtienes ";
            if (opponent->Name == selectedGuardian->MainMaster)
            {
                cout << "2 puntos." << endl;
                selectedGuardian->PowerLevel += 2;
            }
            else
            {
                cout << "1 punto." << endl;
                selectedGuardian->PowerLevel += 1;
            }
        }
        else
        {
            cout << "Has perdido el enfrentamiento. No obtienes puntos." << endl;
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
        unordered_map<string, Village *> villageMap; // Mapa para evitar duplicacion de villas
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

            // Verificar y agregar vecino si existe en el mapa
            auto neighborIter = villageMap.find(neighborCity);
            if (neighborIter != villageMap.end())
            {
                Village *neighbor = neighborIter->second;
                village->villasConectadas.push_back(neighbor);

                // Agregar la conexion en sentido contrario (vecino a aldea)
                neighbor->villasConectadas.push_back(village);
            }
            else
            {
                // Si el vecino no existe en el mapa, crear una nueva instancia y agregarla al mapa y al vector
                Village *newNeighbor = new Village;
                newNeighbor->Name = neighborCity;
                newNeighbor->MainMaster = "";
                villageMap[neighborCity] = newNeighbor;
                updatedVillages.push_back(newNeighbor);

                // Agregar el nuevo vecino a la lista de vecinos de la villa actual
                village->villasConectadas.push_back(newNeighbor);

                // Agregar la conexion en sentido contrario (vecino a aldea)
                newNeighbor->villasConectadas.push_back(village);
            }
        }
        file.close();

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
                    cout << villaConectada->Name << "-";
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
    // Funcion para obtener las aldeas adyacentes a una aldea dada
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

    vector<Guardian *> getOpponents(const string &villageName)
    {
        vector<Guardian *> opponents;
        for (Guardian *guardian : guardians)
        {
            if (guardian->Village == villageName)
            {
                opponents.push_back(guardian);
            }
        }
        return opponents;
    }
    int calculateWinProbability(int myPower, int opponentPower)
    {
        if (myPower == opponentPower)
        {
            return 50; // Misma cantidad de poder, 50% de posibilidades de ganar
        }
        else if (myPower > opponentPower)
        {
            int difference = myPower - opponentPower;
            int winProbability = 50 + difference; // Aumenta las posibilidades de ganar por la diferencia de poder
            return min(winProbability, 100);      // Limita el maximo de posibilidades al 100%
        }
        else
        {
            int difference = opponentPower - myPower;
            int winProbability = 50 - difference; // Disminuye las posibilidades de ganar por la diferencia de poder
            return max(winProbability, 0);        // Limita el mínimo de posibilidades al 0%
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

    int opcion;
    string selectedApprentice;
    do
    {
        // Mostrar el menu
        cout << "----- Menu Principal -----" << endl;
        cout << "1. Seleccionar aprendiz" << endl;
        cout << "2. Mostrar guardianes y aldeas" << endl;
        cout << "3. Empezar viaje" << endl;
        cout << "0. Salir" << endl;
        cout << "Ingrese el numero de la opcion deseada: ";

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
            cout << "Opcion invalida. Por favor, ingrese un numero valido." << endl;
            break;
        }

        cout << endl;

    } while (opcion != 0);

    return 0;
}
