#ifndef ATOM_H
#define ATOM_H

#include <string>
#include "term.h"

using std::string;

class Atom : public Term{
public:
    Atom (string s):_symbol(s) {_value = &_symbol;}
    string symbol() const{
        return _symbol;
    }
    string type() const{
        return _type;
    }
    string _symbol;
    string _type = "atom";
};
#endif
