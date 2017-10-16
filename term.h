#ifndef TERM_H
#define TERM_H

#include <string>

using std::string;

class Term{                           
public:                              
    virtual string symbol() const= 0;
    virtual string value() {
        return *_value;        
    }                 
    virtual bool match(Term & term) {
        return symbol() == term.symbol();
    }
    virtual string type() const=0;                                 
    virtual bool assignable() {return false;}
    string *_value=0;
    std::vector<Term *>link;
};
#endif                                   
