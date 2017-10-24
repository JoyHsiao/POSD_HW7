#ifndef LIST_H
#define LIST_H

#include "term.h"

#include <vector>
using std::vector;

class List : public Term {
public:
  string symbol() const{
    string ret ="[";
    if(_elements.size()>0){
        for (int i=0; i<_elements.size()-1; i++){
            ret += _elements[i]->symbol() + ", ";
        }
        ret += _elements[_elements.size()-1]->symbol() + "]";
    }
    else
        ret +="]";
    return ret;
  }

  string value() {
    string ret ="[";
    _value = &ret;
    if(_elements.size()>0){
        for (int i=0; i<_elements.size()-1; i++){
            ret += _elements[i]->value() + ", ";
        }
        ret += _elements[_elements.size()-1]->value() + "]";
    }
    else
        ret +="]";
    return ret;
  };

  bool match(Term & term) {
    bool ret = true;
    if(term.type()=="list"){
        List * ps = dynamic_cast<List *>(&term);
        if(ps){
            for (int i=0; i<_elements.size(); i++){
                if(_elements.size() == ps->_elements.size()){
                    if((_elements[i]->type() == ps->_elements[i]->type())){
                        if(_elements[i]->value() == ps->_elements[i]->value())
                            continue;
                    }
                    else if(_elements[i]->type() == "variable" && _elements[i]->_assignable)
                        _elements[i]->_value = ps->_elements[i]->_value;
                    else if(ps->_elements[i]->type() == "variable" && ps->_elements[i]->_assignable)
                        ps->_elements[i]->_value = _elements[i]->_value;
                    else
                        ret = false;
                }
            }
        }
    }
    else
        ret = false;

    return ret;
  };

  string type() const{
    return _type;
  }

  string _type = "list";
public:
  List (): _elements() {}
  List (vector<Term *> const & elements):_elements(elements){}
  Term * head() {
    if(_elements.size()<1)
        throw string("Accessing head in an empty list");

    return _elements[0];
  };

  List * tail() {
    List *_tail = new List;
    if(_elements.size()<1)
        throw string("Accessing tail in an empty list");

    if(_elements.size()>1){
        for (int i=1; i<_elements.size(); i++){
            _tail->_elements.push_back(_elements[i]);
        }
    }
    return _tail;
  };

  vector<Term *> _elements;
};

#endif
