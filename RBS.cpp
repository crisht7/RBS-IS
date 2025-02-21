#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

struct BC {
    int numRegla; 				//numero que identifica cada regla
    list<string> antecedente; 	//lista con los antecedentes de cada regla
    string consecuente; 		//consecuente de una regla
    list<string> operadores;	//lista con los operadores de una regla (y, o)
    float FC; 					//factor de certeza de una regla
};

struct BH {
    string hecho; 				//hecho
    float FC; 					//factor de certeza del hecho
};

list<BH> baseHechos;
list<BC> baseConocimiento;
ofstream file;

float encadenamientoAtras(string objetivo, list<BH> baseHechos);

// Funcion para leer la base de conocimiento desde un fichero
void leerBC(string fichero) {
    ifstream fich(fichero.c_str()); // Abrir el fichero

    if(fich.fail()) { // Comprobar si el fichero no se puede abrir
        cout << "El fichero no se puede abrir" << endl ;
        exit(1); 
    } else {
        string linea;
        unsigned int posicion_igual=0; 		// Posicion donde se encuentra =
        string factorCert;
        getline(fich, linea); 				// Leer la primera linea del fichero
        int numEntero = stoi(linea); 		// Convertir la inea a un entero

        for(int i=0; i<numEntero; i++) { 	// Bucle para leer cada regla
            getline(fich, linea); 			// Leer una linea del fichero
            BC regla;
            regla.numRegla = i + 1; 		// Asignar el numero de regla
            stringstream ss(linea); 		// Crear un stringstream a partir de la linea
            string palabra;
            string antecedente;
            string consecuente;

            while(ss >> palabra) { 								// Leer cada palabra de la linea
                if(palabra == "y" || palabra == "o") {	
                    regla.operadores.push_back(palabra); 		// Añadir operadores a la regla
                }
                if(palabra == "Si" || palabra == "y" || palabra == "o") {
                    ss >> antecedente; 							// Leer el antecedente
                    regla.antecedente.push_back(antecedente); 	// Añadir el antecedente a la regla
                }
                if(palabra == "Entonces") {
                    ss >> consecuente; 							// Leer el consecuente
                    int long_consecuente = consecuente.length();
                    string consecuente_sincoma = consecuente.substr(0, long_consecuente - 1);
                    regla.consecuente = consecuente_sincoma; 	// Asignar el consecuente a la regla
                }
            }
            posicion_igual = linea.find("="); 					// Encontrar la posicion del igual
            if(posicion_igual != string::npos) {
                factorCert = linea.substr(posicion_igual + 1); 	// Leer el factor de certeza
                regla.FC = stof(factorCert); 				   	// Convertir a float y asignar a la regla
            }
            baseConocimiento.push_back(regla); 					// Añadir la regla a la base de conocimiento
        }
    }
}

// Funcion para leer la base de hechos desde un fichero
string leerBH(string fichero) {
    ifstream fich(fichero.c_str()); // Abrir el fichero

    if(fich.fail()) { // Comprobar si el fichero no se puede abrir
        cout << "El fichero no se puede abrir" << endl ;
        exit(1); 
    } else {
        string hecho;
        string linea1;
        string objetivo;
        unsigned int posicion_igual;
        string factorCert;
        getline(fich, linea1); 					// Leer la primera linea del fichero
        int numEntero = stoi(linea1); 			// Convertir la linea a un entero

        for(int i = 0; i < numEntero; i++) { 	// Bucle para leer cada hecho
            getline(fich, linea1); 				// Leer una linea del fichero
            BH regla;
            stringstream ss(linea1); 			// Crear un stringstream a partir de la linea
            ss >> hecho; 						// Leer el hecho
            int long_hecho = hecho.length();
            string hecho_sincoma = hecho.substr(0, long_hecho - 1);
            regla.hecho = hecho_sincoma; 		// Asignar el hecho a la regla

            posicion_igual = linea1.find("="); 	// Encontrar la posicion del igual
            if(posicion_igual != string::npos) {
                regla.FC = stof(linea1.substr(posicion_igual + 1)); // Convertir a float y asignar a la regla
            }
            baseHechos.push_back(regla); 		// Añadir la regla a la base de hechos
        }
        getline(fich, linea1); 					// Leer el objetivo
        getline(fich, linea1);
        stringstream ss(linea1);
        ss >> objetivo;
        return objetivo; 						// Devolver el objetivo
    }
}

// Funcion para comprobar si un objetivo esta contenido en la base de hechos
bool Contenido(string objetivo) {
    for(list<BH>::iterator it = baseHechos.begin(); it != baseHechos.end(); it++) {
        if(it->hecho == objetivo) {
            return true; 						// Devolver true si el objetivo esta en la base de hechos
        }
    }
    return false; 								// Devolver false si el objetivo no esta en la base de hechos
}

// Funcion para equiparar un objetivo con las reglas de la base de conocimiento
list<BC> Equiparar(string objetivo) {
    list<BC> conjuntoConflicto;
    for(list<BC>::iterator it = baseConocimiento.begin(); it != baseConocimiento.end(); it++) {
        if(it->consecuente == objetivo) {
            conjuntoConflicto.push_back(*it); 		// Añadir la regla al conjunto de conflicto
        }
    }
    return conjuntoConflicto; 						// Devolver el conjunto de conflicto
}

// Funcion para resolver un conjunto de conflicto
BC Resolver(list<BC> conjuntoConflicto) {
    BC regla;
    for(list<BC>::iterator it = conjuntoConflicto.begin(); it != conjuntoConflicto.end(); it++) {
        regla = *it; 						// Asignar la ultima regla del conjunto de conflicto
    }
    return regla; 							// Devolver la regla
}

// Funcion para eliminar una regla del conjunto de conflicto
list<BC> Eliminar(list<BC>& conjuntoConflicto, const BC& regla) {
    for (auto it = conjuntoConflicto.begin(); it != conjuntoConflicto.end(); ++it) {
        if (it->numRegla == regla.numRegla) {
            conjuntoConflicto.erase(it); 			// Eliminar la regla del conjunto de conflicto
            break;
        }
    }
    return conjuntoConflicto; 						// Devolver el conjunto de conflicto actualizado
}

// Funcion para calcular el factor de certeza en el caso 1
float Caso1(const BC& regla, const list<BH>& baseHechos) {
    if (regla.antecedente.empty()) return 0; // Si no hay antecedentes, no hay FC que calcular

    // Empezamos con el primer antecedente
    float FCant = encadenamientoAtras(regla.antecedente.front(), baseHechos);
    auto op_it = regla.operadores.begin();

    // Iterar sobre los siguientes antecedentes
    for (auto it = next(regla.antecedente.begin()); it != regla.antecedente.end(); ++it) {
        float aux = encadenamientoAtras(*it, baseHechos);
        if (op_it != regla.operadores.end()) {
            if (*op_it == "y") {
                FCant = min(FCant, aux); 			// Calcular el minimo si el operador es "y"
            } else if (*op_it == "o") {
                FCant = max(FCant, aux); 			// Calcular el maximo si el operador es "o"
            }
            ++op_it; 								// Avanzar al siguiente operador si existe
        }
    }

    return FCant; // Devolver el factor de certeza
}

// Funcion para calcular el factor de certeza en el caso 2
float Caso2(vector<float> factoresCert) {
    while(factoresCert.size() > 1 ) {
        float FC1 = factoresCert[factoresCert.size()-1];
        float FC2 = factoresCert[factoresCert.size()-2];
        factoresCert.resize(factoresCert.size()-1);

        if(FC1 >=0 && FC2 >= 0) {
            factoresCert[factoresCert.size()-1] = FC1 + (FC2*(1-FC1)); // Calculo si ambos factores son positivos
        } else if(FC1 <= 0 && FC2 <= 0) {
            factoresCert[factoresCert.size()-1] = FC1 + (FC2*(1+FC1)); // Calculo si ambos factores son negativos
        } else {
            //Si se ejecuta la orden abs propia de C++ ocurre un error de ambigüedad.
            if(std::fabs(FC1) < std::fabs(FC2)) {
                factoresCert[factoresCert.size()-1] = (FC1 + FC2) / (1 - std::fabs(FC1)); // Calculo si uno es positivo y el otro negativo
            } else {
                factoresCert[factoresCert.size()-1] = (FC1 + FC2) / (1 - std::fabs(FC2));	// calculo si es al reves
            }
        }
    }
    return factoresCert[0]; // Devolver el factor de certeza final
}

// Funcion para calcular el factor de certeza en el caso 3
float Caso3(float FCant, float fcRegla) {
    if(FCant < 0) {
        return 0; 						// Devolver 0 si el factor de certeza es negativo
    } else {
        return fcRegla * FCant; 		// Devolver el producto de los factores de certeza
    }
}

// Funcion para escribir en un fichero
void escribirFichero(string c) {
    file << c << endl; // Escribir la cadena en el fichero
}

// Funcion para realizar encadenamiento hacia atras
float encadenamientoAtras(string objetivo, list<BH> baseHechos) {
    if(Contenido(objetivo)) { // Comprobar si el objetivo esta en la base de hechos
        for(list<BH>::iterator it = baseHechos.begin(); it != baseHechos.end(); it++) {
            if(it->hecho == objetivo) {
                return it->FC; // Devolver el factor de certeza del hecho
            }
        }
    }

    float factorCert=0;
    list<BC> conjuntoConflicto = Equiparar(objetivo); 			// Equiparar el objetivo con las reglas
    vector<float> factoresCert;

    while (!conjuntoConflicto.empty()) {
        BC regla = Resolver(conjuntoConflicto); 				// Resolver el conjunto de conflicto
        escribirFichero("R" + to_string(regla.numRegla) + " (regla activada)");
        conjuntoConflicto = Eliminar(conjuntoConflicto, regla); // Eliminar la regla del conjunto de conflicto

        float FCant;
        if(regla.antecedente.size() > 1) {
            FCant = Caso1(regla, baseHechos); 					// Calcular el factor de certeza en el caso 1
            escribirFichero("   Caso 1: " + objetivo + ", " + to_string(FCant));
        } else {
            FCant = encadenamientoAtras(regla.antecedente.front(), baseHechos); // Realizar encadenamiento hacia atr�s para el antecedente
        }

        factoresCert.resize(factoresCert.size() + 1);
        factoresCert[factoresCert.size() - 1] = Caso3(FCant, regla.FC); // Calcular el factor de certeza en el caso 3
        escribirFichero("   Caso 3: " + objetivo + ", " + to_string(factoresCert[factoresCert.size() - 1]));
    }
    if(factoresCert.size() > 1) {
        factorCert = Caso2(factoresCert); 							// Calcular el factor de certeza en el caso 2
        escribirFichero("   Caso 2: " + objetivo + ", " + to_string(factorCert));
    } else if (factoresCert.size() == 1) {
        factorCert = factoresCert[0]; 								// Asignar el unico factor de certeza
    }
    return factorCert; // Devolver el factor de certeza final
}



int main(int argc, char *argv[]) {
    string nombreBC = argv[1];
    string nombreBH = argv[2];
    string salida = "salida-" + nombreBC.substr(5, 1) + ".txt"; //Siguiendo la plantilla .\BC-n.txt, el archivo se generará con el numero n en el nombre
                                                                //(Si el fichero está en otra carpeta, el nombre generado será incorrecto)
    file.open(salida);

    if(!file) {
        cerr << "No ha sido posible crear el archivo de salida";
    } else {
        leerBC(nombreBC);
        string objetivo = leerBH(nombreBH);
        escribirFichero("Base de conocimiento -> " + nombreBC + " \nBase de hechos -> " + nombreBH);
        escribirFichero("Objetivo -> " + objetivo);
        escribirFichero("Se ha obtenido el objetivo " + objetivo + " con un factor de certeza de " + to_string(encadenamientoAtras(objetivo, baseHechos)));
    }
    file.close();
    return 0;
}


