#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include "struct.h"
#include "list.h"

class Iterator {
public:
  virtual void first() = 0;
  virtual void next() = 0;
  virtual Term* currentItem() const = 0;
  virtual bool isDone() const = 0;
};

class NullIterator :public Iterator{
public:
  NullIterator(Term *n){}
  void first(){}
  void next(){}
  Term * currentItem() const{
      return nullptr;
  }
  bool isDone() const{
    return true;
  }

};

class StructIterator :public Iterator {
public:
  friend class Struct;
  void first() {
    _index = 0;
  }

  Term* currentItem() const {
    return _s->args(_index);
  }

  bool isDone() const {
    return _index >= _s->arity();
  }

  void next() {
    _index++;
  }

private:
  StructIterator(Struct *s): _index(0), _s(s) {}
  int _index;
  Struct* _s;
};

class StructBFSIterator :public Iterator {
public:
  friend class Struct;
  void first() {
    _index = 0;
    bfs();
    bfs_sun();
  }

  Term* currentItem() const {
    return _bfs[_index];
  }

  bool isDone() const {
    return _index >= _bfs.size();
  }

  void next() {
    _index++;
  }

  void bfs(){
    for(int i=0; i<_s->arity(); i++){
        if(_s->args(i)->type() == "list"){
            _list(_s->args(i));
        }
        else if(_s->args(i)->type() == "struct"){
            _struct(_s->args(i));
        }
        else
            _bfs.push_back(_s->args(i));
    }
  }

  void bfs_sun(){

    while(!Tempbfs.empty()){
        if(Tempbfs[0]->type() == "list"){
            List *l = dynamic_cast<List *>(Tempbfs[0]);
            for(int i=0; i< l->arity(); i++){
                if(l->args(i)->type() == "list"){
                    _list(l->args(i));
                }
                else if(l->args(i)->type() == "struct"){
                    _struct(l->args(i));
                }
                else
                    _bfs.push_back(l->args(i));
            }
        }
        else if(Tempbfs[0]->type() == "struct"){
            Struct *s = dynamic_cast<Struct *>(Tempbfs[0]);
            for(int i=0; i< s->arity(); i++){
                if(s->args(i)->type() == "list"){
                    _list(s->args(i));
                }
                else if(s->args(i)->type() == "struct"){
                    _struct(s->args(i));
                }
                else
                    _bfs.push_back(s->args(i));
            }
        }
        Tempbfs.erase(Tempbfs.begin());
    }
  }

private:
  StructBFSIterator(Struct *s): _index(0), _s(s) {}
  
  void _struct(Term *sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _bfs.push_back(s);
    Tempbfs.push_back(s);
  }

  void _list(Term *sun){
    List *l = dynamic_cast<List *>(sun);
    _bfs.push_back(l);
    Tempbfs.push_back(l);
  }

  int _index;
  Struct* _s;
  vector <Term *> _bfs;
  vector <Term *> Tempbfs;
};

class StructDFSIterator :public Iterator {
public:
  friend class Struct;
  void first() {
    _index = 0;
    dfs();
  }

  Term* currentItem() const {
    return _dfs[_index];
  }

  bool isDone() const {
    return _index >= _dfs.size();
  }

  void next() {
    _index++;
  }

  void dfs(){
    for(int i=0; i<_s->arity(); i++){
        if(_s->args(i)->type() == "list"){
            _list(_s->args(i));
        }
        else if(_s->args(i)->type() == "struct"){
            _struct(_s->args(i));
        }
        else
            _dfs.push_back(_s->args(i));
    }
  }

private:
  StructDFSIterator(Struct *s): _index(0), _s(s) {}
  
  void _struct(Term *sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _dfs.push_back(s);
    for(int i=0; i<s->arity(); i++){
        if(s->args(i)->type() == "struct"){
            _struct(s->args(i));
        }
        else if(s->args(i)->type() == "list"){
            _list(s->args(i));
        }
        else{
            _dfs.push_back(s->args(i));
        }
    }
  }

  void _list(Term *sun){
    List *l = dynamic_cast<List *>(sun);
    _dfs.push_back(l);
    for(int i=0; i<l->arity(); i++){
        if(l->args(i)->type() == "list"){
            _list(l->args(i));
        }
        else if(l->args(i)->type() == "struct"){
            _struct(l->args(i));
        }
        else
            _dfs.push_back(l->args(i));
    }
  }

  int _index;
  Struct* _s;
  vector <Term *> _dfs;
};

class ListIterator :public Iterator {
public:
  ListIterator(List *list): _index(0), _list(list) {}

  void first() {
    _index = 0;
  }

  Term* currentItem() const {
    return _list->args(_index);
  }

  bool isDone() const {
    return _index >= _list->arity();
  }

  void next() {
    _index++;
  }
private:
  int _index;
  List* _list;
};

class ListBFSIterator :public Iterator {
public:
  friend class List;
  void first() {
    _index = 0;
    bfs();
    bfs_sun();
  }

  Term* currentItem() const {
    return _bfs[_index];
  }

  bool isDone() const {
    return _index >= _bfs.size();
  }

  void next() {
    _index++;
  }

  void bfs(){
    for(int i=0; i<_s->arity(); i++){
        if(_s->args(i)->type() == "list"){
            _list(_s->args(i));
        }
        else if(_s->args(i)->type() == "struct"){
            _struct(_s->args(i));
        }
        else
            _bfs.push_back(_s->args(i));
    }
  }

  void bfs_sun(){

    while(!Tempbfs.empty()){
        if(Tempbfs[0]->type() == "list"){
            List *l = dynamic_cast<List *>(Tempbfs[0]);
            for(int i=0; i< l->arity(); i++){
                if(l->args(i)->type() == "list"){
                    _list(l->args(i));
                }
                else if(l->args(i)->type() == "struct"){
                    _struct(l->args(i));
                }
                else
                    _bfs.push_back(l->args(i));
            }
        }
        else if(Tempbfs[0]->type() == "struct"){
            Struct *s = dynamic_cast<Struct *>(Tempbfs[0]);
            for(int i=0; i< s->arity(); i++){
                if(s->args(i)->type() == "list"){
                    _list(s->args(i));
                }
                else if(s->args(i)->type() == "struct"){
                    _struct(s->args(i));
                }
                else
                    _bfs.push_back(s->args(i));
            }
        }
        Tempbfs.erase(Tempbfs.begin());
    }
  }

private:
  ListBFSIterator(List *s): _index(0), _s(s) {}
  
  void _struct(Term *sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _bfs.push_back(s);
    Tempbfs.push_back(s);
  }

  void _list(Term *sun){
    List *l = dynamic_cast<List *>(sun);
    _bfs.push_back(l);
    Tempbfs.push_back(l);
  }

  int _index;
  List* _s;
  vector <Term *> _bfs;
  vector <Term *> Tempbfs;
};

class ListDFSIterator :public Iterator {
public:
  friend class List;
  void first() {
    _index = 0;
    dfs();
  }

  Term* currentItem() const {
    return _dfs[_index];
  }

  bool isDone() const {
    return _index >= _dfs.size();
  }

  void next() {
    _index++;
  }

  void dfs(){
    for(int i=0; i<_s->arity(); i++){
        if(_s->args(i)->type() == "list"){
            _list(_s->args(i));
        }
        else if(_s->args(i)->type() == "struct"){
            _struct(_s->args(i));
        }
        else
            _dfs.push_back(_s->args(i));
    }
  }

private:
  ListDFSIterator(List *s): _index(0), _s(s) {}
  
  void _struct(Term *sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _dfs.push_back(s);
    for(int i=0; i<s->arity(); i++){
        if(s->args(i)->type() == "struct"){
            _struct(s->args(i));
        }
        else if(s->args(i)->type() == "list"){
            _list(s->args(i));
        }
        else{
            _dfs.push_back(s->args(i));
        }
    }
  }

  void _list(Term *sun){
    List *l = dynamic_cast<List *>(sun);
    _dfs.push_back(l);
    for(int i=0; i<l->arity(); i++){
        if(l->args(i)->type() == "list"){
            _list(l->args(i));
        }
        else if(l->args(i)->type() == "struct"){
            _struct(l->args(i));
        }
        else
            _dfs.push_back(l->args(i));
    }
  }

  int _index;
  List* _s;
  vector <Term *> _dfs;
};

#endif
