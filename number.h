#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include <sstream>
using std::string;

class Number{
public:
    Number(double value):_symbol(){ss << value; ss>> _symbol;}
    string symbol(){return _symbol;}
    string value(){return _symbol;}
    string _symbol;
private:
    std::stringstream ss;
};

#endif
