#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include "struct.h"
#include "list.h"

template <class T>
class Iterator{
public:
  virtual void first() = 0;
  virtual void next() = 0;
  virtual T currentItem() const = 0;
  virtual bool isDone() const = 0;
};

template <class T>
class NullIterator :public Iterator <T>{
public:
  NullIterator(Term *n){}
  void first(){}
  void next(){}
  T currentItem() const{
      return nullptr;
  }
  bool isDone() const{
    return true;
  }

};

template <class T>
class StructIterator :public Iterator <T>{
public:
  friend class Struct;
  void first() {
    _index = 0;
  }

  T currentItem() const {
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

template <class T>
class StructBFSIterator :public Iterator <T> {
public:
  friend class Struct;
  void first() {
    _index = 0;
    bfs();
    bfs_sun();
  }

  T currentItem() const {
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
  
  void _struct(T sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _bfs.push_back(s);
    Tempbfs.push_back(s);
  }

  void _list(T sun){
    List *l = dynamic_cast<List *>(sun);
    _bfs.push_back(l);
    Tempbfs.push_back(l);
  }

  int _index;
  Struct* _s;
  vector <T> _bfs;
  vector <T> Tempbfs;
};

template <class T>
class StructDFSIterator :public Iterator <T>{
public:
  friend class Struct;
  void first() {
    _index = 0;
    dfs();
  }

  T currentItem() const {
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
  
  void _struct(T sun){
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

  void _list(T sun){
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
  vector <T> _dfs;
};

template <class T>
class ListIterator :public Iterator <T>{
public:
  ListIterator(List *list): _index(0), _list(list) {}

  void first() {
    _index = 0;
  }

  T currentItem() const {
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

template <class T>
class ListBFSIterator :public Iterator <T>{
public:
  friend class List;
  void first() {
    _index = 0;
    bfs();
    bfs_sun();
  }

  T currentItem() const {
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
  
  void _struct(T sun){
    Struct *s = dynamic_cast<Struct *>(sun);
    _bfs.push_back(s);
    Tempbfs.push_back(s);
  }

  void _list(T sun){
    List *l = dynamic_cast<List *>(sun);
    _bfs.push_back(l);
    Tempbfs.push_back(l);
  }

  int _index;
  List* _s;
  vector <T> _bfs;
  vector <T> Tempbfs;
};

template <class T>
class ListDFSIterator :public Iterator <T>{
public:
  friend class List;
  void first() {
    _index = 0;
    dfs();
  }

  T currentItem() const {
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
  
  void _struct(T sun){
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

  void _list(T sun){
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
  vector <T> _dfs;
};

#endif
