#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include <sstream>
using std::string;

class Number{
public:
    Number(double value){
        ss << value;
        ss >> _symbol;
    }
    string symbol(){return _symbol;}
    string value(){return _symbol;}
    string _symbol;
    string _type = "number";
private:
    std::stringstream ss;
};

#endif
