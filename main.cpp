/** \file main.cpp
 * \brief Este es el archivo main, en el que est� todo el
 *          código del motor de inferencias y la funci�n main.
 * \author José Luis Abellán Monreal
 * \date 25/01/2024
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "hecho.h"
#include "regla.h"

using namespace std;

vector<Hecho> BH; /**< Base de Hechos. La base se declara como un vector de Hecho. */
vector<Regla> BC; /**< Base de Conocimientos. La base se declara como un vector de Regla. */

/** \brief Este m�todo sirve para imprimir las bases de hechos
 *          y de conocimientos.
 */
void print_bases() {
    cout << "--- BC ---\n";
    for (int i = 0; i < (int) BC.size(); i++) {
        cout << "N? " << i+1 << " = ID: " << BC[i].getID() << ", FC: " << BC[i].getFC() <<
        ", OP: " << BC[i].getOP() << ", OP1: " << BC[i].getOPS()[0] << ", OP2: " << BC[i].getOPS()[1] <<
        ", RES: " << BC[i].getRes() << endl;
    }

    cout << "--- BH ---\n";
    for (int i = 0; i < (int) BH.size(); i++) {
        cout << "N? " << i+1 << " = ID: " << BH[i].getID() << ", FC: " << BH[i].getFC() << endl;
    }
}

/** \brief Este método sirve para separar una cadena en subcadenas
 *          dado un patr�n de entrada.
 * \param cadena La cadena completa a separar.
 * \param patron El patron por el que separar la cadena.
 * \return Un vector de cadenas que contiene todas las subcadenas separadas por el patr�n.
 */
vector<string> separar_campos(const string& cadena, const string& patron) {
    vector<string> resultado;
    int inicio = 0;
    int fin = cadena.find(patron);

    while (fin != (int) string::npos) {
        resultado.push_back(cadena.substr(inicio, fin - inicio));
        inicio = fin + patron.length();
        fin = cadena.find(patron, inicio);
    }

    resultado.push_back(cadena.substr(inicio, fin));

    return resultado;
}

/** \brief Este m�todo sirve para buscar un hecho en la BH.
 * \param meta El identificador (ID) del Hecho.
 * \return Un puntero al hecho de la BH o NULL si el ID no est� en la BH.
 */
Hecho * buscar_hecho(string meta) {
    for (int i = 0; i < (int) BH.size(); i++) {
        if (BH[i].getID() == meta)
            return &BH[i];
    }
    return NULL;
}

/**
 * \brief Este m�todo comprueba si en un string dado por par�metro hay espacios.
 * \param str El string a comprobar.
 * \return Un booleano indicando si hay un espacio en el string o no.
*/
bool hay_espacios(const string& str) {
    for (int i = 0; i < (int) str.length(); i++) {
        if (str[i] == ' ')
            return true;
    }
    return false;
}

/**
 * \brief Este m�todo devuelve las Reglas que derivan en un Hecho dado por par�metro.
 * \param meta El ID del Hecho a comprobar. ?Debe de ser un ID de Hecho!
*/
vector<Regla> equiparar(string meta) {
    vector<Regla> res;
    for (int i = 0; i < (int) BC.size(); i++) {
        if (BC[i].getRes() == meta)
            res.push_back(BC[i]);
    }
    return res;
}

/**
 * \brief Este método calcula los Casos 1 y 3 para las actualizaciones de los FC para hallar el FC meta.
 * \param R La Regla por la que concatenar los FC.
 * \return Un doble que es el resultado de concatenar los FC de los Hechos antecesores con el de la Regla.
*/
double calcular_prob(Regla R) {
    cout << "Regla a comprobar: " << R.getID() << endl;
    switch (R.getOP()) {
    case 1:
        cout << "\tCaso 1: min(" << buscar_hecho(R.getOPS()[0])->getFC() << ", " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        cout << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, min(buscar_hecho(R.getOPS()[0])->getFC(), buscar_hecho(R.getOPS()[1])->getFC()));
    case 2:
        cout << "\tCaso 1: max(" << buscar_hecho(R.getOPS()[0])->getFC() << ", " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        cout << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, max(buscar_hecho(R.getOPS()[0])->getFC(), buscar_hecho(R.getOPS()[1])->getFC()));
    default:
        cout << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[0])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, buscar_hecho(R.getOPS()[0])->getFC());
    }
}

/**
 * \brief M�todo principal para el motor de inferencias con encadenamiento hacia atr�s.
 *          Desde este m�todo se comprueba si el Hecho pasado por par�metro puede ser resultado de los Hechos iniciales.
 * \param meta El ID de la meta a verificar.
 * \return Un booleano que indica si la meta se puede derivar de los Hechos iniciales.
*/
bool verificar(string meta) {
    bool verificado = false;
    // Si la meta est� ya contenida en la BH, devuelve true y el m�todo no sigue.
    if (buscar_hecho(meta) != NULL)
        verificado = true;
    else {
        // Saca las reglas que derivan en la meta
        vector<Regla> CC = equiparar(meta);
        // Printea el Conjunto Conflicto
        cout << "Conjunto Conflicto para la meta " << meta << ":" << endl;
        for (int i = 0; i < (int) CC.size(); i++) {
            cout << "\tN? " << i+1 << " = ID: " << CC[i].getID() << ", FC: " << CC[i].getFC() <<
            ", OP: " << CC[i].getOP() << ", OP1: " << CC[i].getOPS()[0] << ", OP2: " << CC[i].getOPS()[1] <<
            ", RES: " << CC[i].getRes() << endl;
        }
        // Recorremos todas las reglas que derivan en la meta
        while (!CC.empty()) {
            Regla R = CC[0];        // Seleccionamos la primera Regla
            CC.erase(CC.begin());   // Y la eliminamos
            cout << "Regla " << R.getID() << " (seleccionada)" << endl;
            // Cogemos los Hechos antecesores a la Regla escogida
            vector<string> nuevasMetas = R.getOPS();
            // Si es unitario, el segundo operando lo eliminamos porque no existe
            if (R.getOP() == 0)
                nuevasMetas.erase(nuevasMetas.begin() + 1);
            // Y printeamos los antecesores 
            cout << "Antecesores de " << R.getID() << ":" << endl;
            for (int i = 0; i < (int) nuevasMetas.size(); i++) {
                cout << "\tN? " << i+1 << ": ID: " << nuevasMetas[i] << endl;
            }
            // Verificamos el Hecho
            verificado = true;
            // Recorremos todos los Hechos antecesores a la Regla
            while (!nuevasMetas.empty()) {
                string nMet = nuevasMetas[0];               // Seleccionamos el primer Hecho
                nuevasMetas.erase(nuevasMetas.begin());     // Y eliminamos
                verificado = verificar(nMet);               // Recursividad
            }

            if (verificado) {
                // Si est� verificado y no est� en la BH
                if (buscar_hecho(meta) == NULL) {
                    // Creamos el Hecho y lo a?adimos a la BH
                    Hecho aux(meta, calcular_prob(R));
                    BH.push_back(aux);
                    cout << "A?adido " << meta << " a la BH con FC = " << buscar_hecho(R.getRes())->getFC() << endl;
                } else {
                    /* Si est� en la BH, concatenamos el anterior FC calculado
                       Aqu� es donde se comprueba el caso 2 para concatenar los FC */
                    double prob_ant = buscar_hecho(meta)->getFC();
                    double prob_calc = calcular_prob(R);
                    double prob_nueva = 0.0;
                    cout << "Concatenar " << prob_ant << " y " << prob_calc << endl;
                    cout << "\tCaso 2: ";
                    if (prob_calc >= 0.0 && prob_ant >= 0.0) {
                        prob_nueva = prob_ant + prob_calc * (1 - prob_ant);
                        cout << prob_ant << " + " << prob_calc << " * (1 - " << prob_ant << ")" << endl;
                    } else if (prob_calc <= 0.0 && prob_ant <= 0.0) {
                        prob_nueva = prob_ant + prob_calc * (1 + prob_ant);
                        cout << prob_ant << " + " << prob_calc << " * (1 + " << prob_ant << ")" << endl;
                    } else {
                        prob_nueva = (prob_ant + prob_calc) / (1 - min(abs(prob_ant), abs(prob_calc)));
                        cout << "(" << prob_ant << " + " << prob_calc << ") / (1 - min(abs(" << prob_ant << "), abs(" << prob_calc << ")))" << endl;
                    }
                    buscar_hecho(meta)->setFC(prob_nueva);
                    cout << "Actualizada " << meta << " a la BH con FC = " << prob_nueva << endl;
                }
            }
        }
    }
    return verificado;
}

/**
 * \brief Funci�n que llama a la funci�n principal "verificar".
 * \param meta La meta inicial a comprobar.
 * \return Un booleano que indica si la meta se verifica o no.
*/
bool encadenamiento_hacia_atras(string meta) {
    if (verificar(meta))
        return true;
    return false;
}

int main(int argc, char ** argv) {
    // Comprobamos si el n�mero de par�metros es correcto
    if (argc != 3) {
        cerr << "ERROR: N�mero de par�metros incorrecto" << endl;
        cerr << "USO: " << argv[0] << "BaseConocimientos.txt BaseHechos.txt" << endl;
        return 1;
    }

    ifstream bh, bc;
    // Abrimos la BC
    bc.open(argv[1]);
    if (!bc) {
        cerr << "ERROR: El archivo " << argv[1] << " no se pudo abrir correctamente" << endl;
        return 2;
    }
    // Abrimos la BH
    bh.open(argv[2]);
    if (!bh) {
        cerr << "ERROR: El archivo " << argv[2] << " no se pudo abrir correctamente" << endl;
        return 3;
    }

    int numHechos, numConocimientos;
    string linea;
    bh >> numHechos;
    getline(bh, linea);
    // Leemos la BH
    for (int i = 0; i < numHechos; i++) {
        getline(bh, linea);
        vector<string> campos = separar_campos(linea, ", FC=");
        istringstream iss(campos[1].c_str());
        double valor;
        iss >> valor;
        Hecho aux(campos[0], valor);
        BH.push_back(aux);
    }
    getline(bh, linea);
    getline(bh, linea);
    vector<string> metas = separar_campos(linea, ", ");

    bc >> numConocimientos;
    getline(bc, linea);
    // Y aqu� leemos la BC
    for (int i = 0; i < numConocimientos; i++) {
        getline(bc, linea);
        vector<string> campos = separar_campos(linea, ":");                 // campos[0] = id, campos[1] = resto
        vector<string> campos2 = separar_campos(campos[1], ", FC=");        // campos2[0] = regla, campos2[1] = fc
        vector<string> campos3 = separar_campos(campos2[0], " Entonces ");  // campos3[0] = cond. regla, campos3[1] = res
        string cond = campos3[0].substr(4);
        unsigned short int op = 0;
        string op1 = "";
        string op2 = "";
        string res = campos3[1];
        if (!hay_espacios(cond)) {
            op1 = cond;
        } else {
            vector<string> operacion = separar_campos(cond, " ");
            op1 = operacion[0];
            if (operacion[1] == "y") {
                op = 1;
                op2 = operacion[2];
            } else if (operacion[1] == "o") {
                op = 2;
                op2 = operacion[2];
            }

        }
        istringstream iss(campos2[1].c_str());
        double valor;
        iss >> valor;
        Regla aux(campos[0], valor, op, op1, op2, res);
        BC.push_back(aux);
    }

    print_bases();
    int numMetas = metas.size();

    if (numMetas == 1)
        cout << "Encontrada 1 meta" << endl;
    else
        cout << "Encontradas " << numMetas << " metas" << endl;

    for (int i = 0; i < (int) metas.size(); i++) {
        cout << "Objetivo " << i+1 << ": " << metas[i] << endl;
        if (encadenamiento_hacia_atras(metas[i])) {
            Hecho * fin = buscar_hecho(metas[i]);
            cout << "El algoritmo SBR con encadenamiento hacia atr�s verifica la meta " << metas[i] << " con FC = " << fin->getFC() << endl;
        } else
            cout << "El algoritmo SBR con encadenamiento hacia atr�s NO verifica la meta " << metas[i] << endl;
    }
    return 0;
}
