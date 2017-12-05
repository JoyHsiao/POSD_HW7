#ifndef STRUCT_H
#define STRUCT_H

#include <vector>
#include <string>
#include "term.h"
#include "atom.h"

//#define DEBUG1

using std::string;

class Struct:public Term
{
public:
    Struct(Atom const & name, std::vector<Term *> args):_name(name), _args(args) {}

    Term * args(int index) {
        return _args[index];
    }

    Atom & name() {
        return _name;
    }

    void setArgs(int i, Term *t){
        _args[i] = t;
    }

    string value(){
    #ifdef DEBUG1
    std::cout<<"= "<<_args[0]->type() << _args[0]->symbol()<<std::endl;
    #endif
        string ret =_name.symbol() + "(";
        _value = &ret;
        if(_args[0]->type() == "variable"){
            for(int i = 0; i < _args.size() - 1 ; i++){
                ret += _args[i]-> value() + ", ";
            }
            ret += _args[_args.size()-1]-> value() + ")";
        }
        if(_args[0]->type() != "variable" && _args[0]->type() != "struct"){
            for(int i = 0; i < _args.size() - 1 ; i++){
                ret += _args[i]-> symbol() + ", ";
            }
            ret += _args[_args.size()-1]-> symbol() + ")";
        }
        if(_args[0]->type() == "struct"){
            #ifdef DEBUG1
            std::cout<<"=== "<<_args[0]->type()<< " "<< _args[0]->symbol()<<" " <<_args.size() ;
            std::cout<< " "<< _args[0]->link.size()<< " "<< _args[0]->value()<<std::endl;
            #endif
            for(int i = 0; i < _args.size() - 1 ; i++){
                ret += _args[i]-> value() + ", ";
            }
            ret += _args[_args.size()-1]-> value() + ")";
        }
    #ifdef DEBUG1
    std::cout<<"===== "<<_args[0]->symbol()<< ": "<<_args[0]->value() << " "<< ret <<std::endl;
    #endif
        return  ret;
        //return *_value;
    }

    string symbol() const{
        if(_args.empty())
            return  _name.symbol() + "()";
        string ret = _name.symbol() + "(";
        std::vector<Term *>::const_iterator it = _args.begin();
        for (; it != _args.end()-1; ++it)
            ret += (*it)->symbol()+", ";
        ret  += (*it)->symbol()+")";
        return  ret;
    }

    int arity() const{
        return _args.size();
    }

    bool match(Term &term){
        Struct * ps = dynamic_cast<Struct *>(&term);
        if (ps){
            if (!_name.match(ps->_name)){
                return false;
            }
            if(_args.size()!= ps->_args.size()){
                return false;
            }
            for(int i=0;i<_args.size();i++){
                if(_args[i]->symbol() != ps->_args[i]->symbol())
                    return false;
            }
            return true;
        }
        return false;
    }

    Iterator * createBFSIterator();
    Iterator * createDFSIterator();

    Iterator * createIterator();
    string type() const{return _type;}
    string _type = "struct";
private:
    Atom _name;
    std::vector<Term *> _args;
};

#endif
