#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "term.h"
#include "struct.h"

#define DEBUG

class Variable: public Term{
public:
    Variable(string s):_symbol(s){_value = &v_value;}
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
        if(term.type()=="variable"){
            if(term.assignable() || _assignable){
                bool push = 1, push_term = 1;
                for (int i =0; i<link.size(); i++){
                    if(link[i]->_value == _value && push)
                        push =0;
                    if(link[i]->_value == term._value && push_term)
                        push_term = 0;
                }
                if(push)
                    link.push_back(this);
                if(push_term){
                    link.push_back(&term);
                    term.link = link;
                }
            }
            if(!term.assignable()){
                _value = term._value;
                _assignable = false;
            }
        }
        
        if(_assignable && term.type()=="struct" ){
            *_value = term.value();
            
            st_ptr = dynamic_cast<Struct *>(&term);
        }

        if(_assignable && term.type()!="variable" && term.type()!="struct"){
            *_value = term.value();
            for (int i =0; i<link.size(); i++){
                link[i]->_value= _value;
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
    bool _assignable = true;
};

#endif
