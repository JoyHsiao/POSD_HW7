#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "term.h"
#include "struct.h"
#include "list.h"

//#define DEBUG

class Variable: public Term{
public:
    Variable(string s):_symbol(s){_value = &v_value; _assignable = true;}
    string symbol() const{return _symbol;}
    string value(){
        if(st_ptr!=0 && (st_ptr->type() =="struct"))
            *_value = st_ptr->value();
        if(list_ptr!=0 && (list_ptr->type() =="list"))
            *_value = list_ptr->value();
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

        if(term.type()=="list"){
            list_ptr = dynamic_cast<List *>(&term);
            if(_assignable){
                for(int i=0; i< list_ptr->_elements.size()-1; i++){
                    if(*_value == list_ptr->_elements[i]->symbol()){
                        return false;
                    }
                }
                //link.push_back(&term);
                *_value = term.value();
                _assignable = false;
            }
            else{
                if(*_value == term.value())
                    return true;
            }
        }

        if(term.type()=="atom"){
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
            std::cout<<"=== "<< symbol()<<" "<< term.value() <<std::endl;
            *_value = term.value();
            for (int i =0; i<link.size(); i++){
                link[i]->_value= _value;
                
                for (int j =0; j<link[i]->link.size(); j++){
                    link[i]->link[j]->_value=_value;
                }
            }
            _assignable = false;
        }
            std::cout<<"=== "<< value()<<std::endl;
        return ret;
    }
    string const _symbol;
    string _type = "variable";
    Struct *st_ptr=0;
    List *list_ptr=0;
private:
    string v_value = _symbol;
};

#endif
