#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include <cmath>
#include "Nodo.h"

using namespace std;

//Estructura que define una conexion
class Arista {
    private:
        Nodo* nodo1;
        Nodo* nodo2;
        int velocidad; //M�xima cantidad de MB que permite a la vez.
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
