#ifndef NODE_H
#define NODE_H
#include "atom.h"

enum Operators {SEMICOLON, COMMA, EQUALITY, TERM};

class Node {
public:
  Node(Operators op):payload(op), term(0), left(0), right(0) {}
  Node(Operators op, Term *t, Node *l, Node *r):payload(op), term(t), left(l), right(r) {}

  bool evaluate(){
    std::cout<<"**** "<<payload<<std::endl;
    if(payload == EQUALITY)
        return left->term->match(*right->term);
    else{
        return left->evaluate() && right->evaluate();
    }
  }

  Term *term;
  Node *left, *right;
  Operators payload;
};

#endif
