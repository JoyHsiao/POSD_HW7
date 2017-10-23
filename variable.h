#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "term.h"
#include "struct.h"

//#define DEBUG

class Variable: public Term{
public:
    Variable(string s):_symbol(s){_value = &v_value; _assignable = true;}
    string symbol() const{return _symbol;}
    string value(){
        if(st_ptr!=0 && (st_ptr->type() =="struct"))
            *_value = st_ptr->value();
        return *_value;
    }
    string type() const{return _type;}
    bool assignable(){return _assignable;}
    bool match( Term &term ){
        bool ret = _assignable;
        bool push;
        if(term.type()=="variable"){
            if(_assignable){
                if(term.assignable()){
                    _value = term._value;
                    push = 1;
                    for (int i =0; i<link.size(); i++){
                        if(link[i]->_value == term._value && push)
                            push = 0;
                    }
                    if(push){
                        link.push_back(&term);
                        term.link.push_back(this);
                    }
                }
                else{
                    _value = term._value;
                    _assignable = false;
                }
            }
            else{
                if(term.assignable()){
                    string temp = *_value, t_var = term.symbol();
                    push = 1;
                    if(temp[0]>64 && temp[0]<91){
                        for (int i =0; i<link.size(); i++){
                            if(link[i]->_value == term._value && push)
                                push = 0;
                        }
                        if(push){
                            term.link.push_back(this);
                            link.push_back(&term);
                            //link = term.link;
                        }
                    }
                    if(temp[0]>96 && temp[0]<123){
                        term._value = _value;
                        *_value = t_var;
                        term.link.push_back(this);
                        _assignable = false;
                    }
                }
            }
            //if(term.assignable() || _assignable){
            //    bool push = 1, push_term = 1;
            //    for (int i =0; i<link.size(); i++){
            //        if(link[i]->_value == _value && push)
            //            push =0;
            //        if(link[i]->_value == term._value && push_term)
            //            push_term = 0;
            //    }
            //    if(push)
            //        link.push_back(this);
            //    if(push_term){
            //        link.push_back(&term);
            //        term.link = link;
            //    }
            //}
        }
        
        if(_assignable && term.type()=="struct" ){
            *_value = term.value();
            
            st_ptr = dynamic_cast<Struct *>(&term);
        }

        if(term.type()=="atom" || term.type()=="list"){
            if(_assignable){
                //link.push_back(&term);
                *_value = term.symbol();
                _assignable = false;
            }
            else{
                if(*_value == term.symbol())
                    return true;
            }
        }
        
        if(_assignable && term.type()=="number"){
            *_value = term.value();
            for (int i =0; i<link.size(); i++){
                link[i]->_value= _value;
                
                for (int j =0; j<link[i]->link.size(); j++){
                    link[i]->link[j]->_value=_value;
                }
            }
            _assignable = false;
        }
        return ret;
    }
    string const _symbol;
    string _type = "variable";
    Struct *st_ptr=0;
private:
    string v_value = _symbol;
};

#endif
