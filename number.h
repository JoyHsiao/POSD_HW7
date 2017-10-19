#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include <sstream>
#include "term.h"

//#define DEBUG_N

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
    bool match(Term &term){
        if(term.type() == "atom")
            return false;
        else if(term.type() == "number" && term.value()!=_symbol){
            #ifdef DEBUG_N
            std::cout<<"N: "<<term.value()<< " "<< _symbol<<std::endl;
            #endif
            return false;
        }
        else if(term.type() == "variable"){
            bool ret = term._assignable;
            #ifdef DEBUG_N
            std::cout<< "V: "<< term.type() << " assig:" << term._assignable<<" "<< term._value<< " " << _value<< std::endl;
            #endif
            if(ret==false && term._value!=_value)
                return false;
            else{
                term._value =_value;
                term._assignable = false;
            }
        }
        return true;
    }
    string _symbol;
    string _type = "number";
private:
    std::stringstream ss;
};

#endif
