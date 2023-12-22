#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include <cmath>

using namespace std;

// Estructura para representar un nodo en el grafo
class Nodo {
    private:
        int id;
        string nombre;
    public:
        Nodo(int _id, string _nombre){
            id = _id;
            nombre = _nombre;
        };
        virtual ~Nodo(){};
        virtual void conectar(Arista* a){};
        int getID(){ return id; };
        string getNombre(){ return nombre; };
        virtual bool isRouter(){return false;};
        virtual bool checkConexion(){return false;};
        virtual bool checkConexion(int ID){
             return false;
        };
        virtual Arista* getConexion(){return NULL;};
        virtual Arista* getConexion(int _ID){return NULL;};
        virtual vector<Arista*> getConexiones(){return NULL;};
};

//Estructura que define una conexion
class Arista {
    private:
        Nodo* nodo1;
        Nodo* nodo2;
        int velocidad; //Máxima cantidad de MB que permite a la vez.
        int costo_efectivo; //Segundos que tarda un paquete X en transmitirse.
        int distancia; //Segundos de latencia de transferencia de un paquete.
        bool hn1;
        bool hn2;
    public:
        Arista(int _velocidad, int _distancia, Nodo* uno, Nodo* dos) 
            : velocidad(_velocidad), distancia(_distancia), hn1(false), hn2(false), nodo1(nullptr), nodo2(nullptr) {
            if (uno) {
                nodo1 = uno;
                hn1 = true;
            }
            if (dos) {
                nodo2 = dos;
                hn2 = true;
            }
            costo_efectivo = distancia;
        }
        
        ~Arista(){};
        
        bool checkNodo(int ID){
            if (nodo1->getID()==ID) return true;
            if (nodo2->getID()==ID) return true;
            return false;
        }
        
        bool isOpen(){
             if (hn1 && hn2) return true;
             return false;
        }
        
        int getVelocidad(){
            return this.velocidad;
        }
        int getDistancia(){
            return this.distancia;
        }
        
        void cortar(int id_nodo) {
            
            if (hn1) {
                if (nodo1->getID() == id_nodo) {
                    delete nodo1;
                    nodo1 = nullptr;
                    hn1 = false;
                }
            }
            if (hn2) {
                if (nodo2->getID() == id_nodo) {
                    delete nodo2;
                    nodo2 = nullptr;
                    hn2 = false;
                }
            }
            
            if (!hn1 && !hn2) {
                delete this;
            }
        }
        
        void setCostoEfectivo(int MBPaquete){
             if (MBPaquete < this.velocidad){
                this.costo_efectivo = this.distancia;
                return;
             }
             double aux = ((double) MBPaquete) / ((double) this.velocidad);
             this.costo_efectivo = (int) ceil(aux * (double) this.distancia);
        }
        
        int getCostoEfectivo(){
            return this.costo_efectivo;
        }
        
        Nodo* getNodoOpuesto(int ID_antinodo){
              if (!this.checkNodo(ID_antinodo)) return NULL;
              if (ID_antinodo == nodo1->getID()) return nodo2;
              if (ID_antinodo == nodo2->getID()) return nodo1;
        }
        
};



// Estructura para representar una conexion en el grafo


// Implementacion de las funciones para leer archivos CSV
vector<Nodo*> leerNodosDesdeCSV(bool showcase) {
    vector<Nodo*> nodos;
    ifstream file("servidores.csv");
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, nombre, tipo;
            bool isrouter = false;

            getline(ss, id, ',');
            getline(ss, nombre, ',');
            getline(ss, tipo, ',');
            
            if (tipo=="router"){
                isrouter = true;
            }
            
            if (showcase && !isrouter){
                cout << "Nombre: '"<<nombre<<"'. ID: "<<id<<"."<<endl;
                continue;
            }
            if (!isrouter && !showcase){
                Cliente* c = new Cliente(stoi(id),nombre);
                nodos.push_back(c);
            } else if (isrouter && !showcase) {
                Router* c = new Router(stoi(id),nombre);
                nodos.push_back(c);
            }
        }

        file.close();
        
        
    } else {
        cerr << "Error al abrir el archivo de nodos." << endl;
    }
    return nodos;
}


vector<Arista*> leerConexionesDesdeCSV(vector<Nodo*> nodos) {
    vector<Arista*> conexiones;
    ifstream file("conexiones.csv");

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idCliente, idServidor, velocidad, distancia;

            getline(ss, idCliente, ',');
            getline(ss, idServidor, ',');
            getline(ss, velocidad, ',');
            getline(ss, distancia, ',');
            
            int idClienteInt = stoi(idCliente);
            int idRouterInt = stoi(idServidor);
            int velocidadInt = stoi(velocidad);
            int distanciaInt = stoi(distancia);
            
            map<int, Nodo*> nodosPorID;
            for (Nodo* nodo : nodos) {
                nodosPorID[nodo->getID()] = nodo;
            }

            // Verificar si los nodos existen antes de crear la arista
            if (nodosPorID.count(idClienteInt) && nodosPorID.count(idServidorInt)) {
                Arista* conexion = new Arista(
                    nodosPorID[idClienteInt],
                    nodosPorID[idServidorInt],
                    velocidadInt,
                    distanciaInt
                );
                conexiones.push_back(conexion);
                nodosPorID[idClienteInt]->conectar(conexion);
                nodosPorID[idServidorInt]->conectar(conexion);
            }
            
        }

        file.close();
    } else {
        cerr << "Error al abrir el archivo: " << archivo << endl;
    }

    return conexiones;
}

*/

int main() {

    // Declaracion de variables
    int nodoOrigen, nodoDestino, pesoArchivo, aux;
    Nodo* remitente, destinatario;
    string saux;
    stringstream ss;
    ss.clear();
    bool keep = true;
    
    cout << "----Cargando datos...----" << endl;
    vector<Nodo*> nodos_grafo = leerNodosDesdeCSV(false);
    vector<Aristas*> conexiones = leerConexionesDesdeCSV(nodos_grafo);
    cout << "----Grafo cargado----" << endl;

    // Menu principal
    while(keep){
        if (!keep) break;
        // Display de decisiones.
        cout << "=== Aplicacion de Mensajeria ===" << endl;
        cout << "Que desea hacer?\nIngrese el numero correspondiente a su decision:" << endl;
        cout << "1: Calcular distancia entre 2 nodos." << endl;
        cout << "2: Ver todos los nodos de la red." << endl;
        cout << "3: Salir." << endl;
        cin >> saux;
        ss<<saux;
        ss>>aux;
        ss.clear();
        bool found = false;
        // Switch diferenciador de instrucciones.
        switch(aux){
            case 1:
            cout << "NO COLOCAR OTRA COSA QUE NO SEAN NUMEROS (O el programa falla)." << endl;
            cout << "Ingrese el ID del nodo de origen: ";
            cin >> saux;
            ss<<saux;
            ss>>nodoOrigen;
            ss.clear();
            
            for (Nodo* nodo : nodos_grafo) {
                if (nodo->getID(nodoOrigen)) {
                   remitente = nodo;
                   found = true;
                   break;                             
                }
            }
            
            if (!found){
               cout << "No se ha encontrado el nodo indicado." << endl;
               break;        
            }
            cout << "Ingrese el ID del nodo de destino: ";
            cin >> saux;
            ss<<saux;
            ss>>nodoDestino;
            ss.clear();
            
            for (Nodo* nodo : nodos_grafo) {
                if (nodo->getID(nodoDestino)) {
                   destinatario = nodo;
                   found = true;
                   break;                             
                }
            }
            
            if (!found){
               cout << "No se ha encontrado el nodo indicado." << endl;
               break;        
            }
        
            cout << "Ingrese el peso del archivo (en MB): ";
            cin >> saux;
            ss<<saux;
            ss>>pesoArchivo;
            ss.clear();
            
            // TODO: Implementar el algoritmo de Bellman-Ford y mostrar la ruta y tiempos entre nodos.
            
            if (!remitente->checkConexion()){
               cout << "Error: El nodo de origen no tiene conexion a un router." << endl;
               break;
            }
            
            //Esta linea busca entregar el router al cual el nodo de origen esta conectado.
            Nodo* router = remitente->getConexion()->getNodoOpuesto(remitente->getID());
            
            //Establece los valores de costo efectivo de antemano.
            for (Arista* arista : conexiones) {
                conexion.setCostoEfectivo(pesoArchivo);
            }
            
            //Verifica que el destinatario exista en la lista de conexiones del router
            if (!router->checkConexion(destinatario->getID()){
               //go to bellmanford;
               cout << "Error: El nodo no esta conectado al mismo router. Envio de paquetes entre routers WIP." << endl;
               break;
            }
            
            //Seccion donde el router busca la arista conectada al nodo destinatario
            vector<Arista*> conx = router->getConexiones();
            
            for (Arista* auxist : conx){
                if (auxist->checkNodo(destinatario->getID()){
                   cout << "Destinatario encontrado en red local." << endl;
                   cout << "Router intermediario: "<< router->getNombre() << "." << endl;
                   cout << "Latencia total de la conexión: "<<
                   (remitente->getConexion()->getCostoEfectivo() + destinatario->getConexion()->getCostoEfectivo())<<
                   " segundos."<<endl;
                   break;
                }
            }
            bellmanford:
            break;
            
            case 2:
            cout << "Leyendo todos los nodos clientes disponibles." << endl;
            leerNodosDesdeCSV(true);
            break;
            
            case 3:
            cout << "Saliendo..." << endl;
            keep = false;
            break;
            
            default:
            cout << "\nValor invalido. Prueba de nuevo.\n" << endl;
            break;
        }
    }
    
    for (Arista* conexion : conexiones) {
        delete conexion;
    }
    
    for (Nodo* nodo : nodos_grafo) {
        delete nodo;
    }
    

    return 0;
};

