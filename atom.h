#ifndef ATOM_H
#define ATOM_H

#include <string>
#include "term.h"
#include "list.h"

//#define DEBUG
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
    bool match(Term &term){
        #ifdef DEBUG
        std::cout<< "Atom match: "<< _value << "  " << term._value<< " "<< term.type() <<" assign:"<< term._assignable<< std::endl;
        #endif
        if(term.type() == "number"){
            #ifdef DEBUG
            std::cout<< "Atom match: "<< term.type() << std::endl;
            #endif
            return false;
        }
        else if(term.type() == "atom" && _value != term._value && *_value != term.symbol() || term.type() =="list"){
            #ifdef DEBUG
            std::cout<< "Atom match: "<< term.type()<< _value << " "<< term._value << std::endl;
            std::cout<< *_value<< " "<< term.symbol() << std::endl;
            #endif
            return false;
        }
        else if(term.type() == "variable"){
            bool ret = term._assignable;
            #ifdef DEBUG
            std::cout<< "Atom match: "<< term.type() << " assig:" << term._assignable<<" "<< term._value << " " << _value<< std::endl;
            #endif
            if(ret==false && term._value!=_value){
                #ifdef DEBUG
                std::cout<< "Atom match: false"<< std::endl;
                 #endif
                return false;
            }
            else{
                term._value =_value;
                term._assignable = false;
                #ifdef DEBUG
                std::cout<< "Atom match: true. "<< term._value <<" "<< _value <<" " << term.type() << " assign: " << term._assignable << std::endl;
                 #endif
            }
        }

                #ifdef DEBUG
                std::cout<< "======================="<< std::endl;
                 #endif
        return true;
    }
    string _type = "atom";
    List *l_ptr=0;
};
#endif
