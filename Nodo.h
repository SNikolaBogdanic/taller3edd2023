#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include <cmath>
#include "Arista.h"

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

//Clase Cliente
class Cliente : public Nodo {
    private:
        Arista* conexion;  // Referencia a una conexion que deberia de dar a un router.
        bool conectado;
    
    public:
        Cliente(int _id, string _nombre) : Nodo(_id, _nombre) {
            conectado = false;
        }
        ~Cliente(){
            if (conectado){
                 conexion->cortar(id);
                 delete conexion;
            }               
        }
        void conectar(Arista* a){
            this.conexion = a;
            conectado = true;
        };
        
        bool checkConexion(){
             return conectado;
        };
        
        bool isRouter(){
             return false;
        };
        
        bool checkConexion(int _ID){
             if (conexion->checkNodo(_ID)) return true;
             return false;
        };
        
        Arista* getConexion(){
                return conexion;
        }
};

//Nodo Router
class Router : public Nodo {
    private:
        vector<Arista*> conexiones;  // Lista de todas las conexiones del router
    
    public:
        Router(int _id, string _nombre) : Nodo(_id, _nombre) {}
        ~Router() {
            for (Arista* conexion : conexiones) {
                conexion->cortar(this.getID());
                delete conexion;
            }
        }

        // Funciones para gestionar clientes
        void agregarConexion(Arista* arista) {
            conexiones.push_back(arista);
        }
    
        void eliminarCliente(int ID) {
            if (ID == this.getID()) return;
        }
        
        void conectar(Arista* a){
            this.conexiones.push_back(a);
        };
        
        bool isRouter(){
             return true;
        };
        
        bool checkConexion(){
             if (conexiones.empty()) return false;
             return true;
        };
        
        bool checkConexion(int _ID){
             if (!this.checkConexion()) return false;
             for (Arista* conexion : conexiones) {
                if (conexion->checkNodo(_ID)) return true;
             }
             return false;
        };
        
        vector<Arista*> getConexiones(){
                        return conexiones;
        };
        
        Arista* getConexion(int _ID){
                for (Arista* conexion : conexiones) {
                    if (conexion->checkNodo(_ID)) return conexion;
                 }
                return NULL;
        };
};
