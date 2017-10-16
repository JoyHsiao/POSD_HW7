#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include <sstream>
#include "term.h"

using std::string;

class Number: public Term{
public:
    Number(double value){
        ss << value;
        ss >> _symbol;
        _value = &_symbol;
    }
    string symbol() const{return _symbol;}
    string type() const{
        return _type;
    }
    string _symbol;
    string _type = "number";
private:
    std::stringstream ss;
};

#endif
