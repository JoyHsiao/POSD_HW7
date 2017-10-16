#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "term.h"

class Variable: public Term{
public:
    Variable(string s):_symbol(s){}
    string symbol() const{
        return _symbol;
    }
    bool match( Term &term ){
        bool ret = _assignable;
        if(_assignable){
            _value = term.symbol() ;
            _assignable = false;
        }
        return ret;
    }
    string const _symbol;
    string _type = "number";
private:
    string _value;
    bool _assignable = true;
};

#endif
