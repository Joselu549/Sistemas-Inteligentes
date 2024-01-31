#include "regla.h"

Regla::Regla() {
    id = "";
    fc = 0.0;
    op = 0;
    res = "";
}

Regla::Regla(const string& nid, const double& nfc, const unsigned short int& nop, const string& op1, const string& op2, const string& nres) {
    id = nid;
    fc = nfc;
    op = nop;
    ops.push_back(op1);
    ops.push_back(op2);
    res = nres;
}

string Regla::getID() { return id; }

double Regla::getFC() { return fc; }

unsigned short int Regla::getOP() { return op; }

vector<string> Regla::getOPS() { return ops; }

string Regla::getRes() { return res; }
