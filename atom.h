#ifndef ATOM_H
#define ATOM_H

#include <string>
#include "term.h"

using std::string;

class Atom : public Term{
public:
    Atom (string s):_symbol(s) {}
    string symbol() const{
        return _symbol;
    }
    string _symbol;
    string _type = "atom";
};
#endif
