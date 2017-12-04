#include "node.h"
Node::Node(Operators op)
    : payload(op), term(0), left(0), right(0) 
{

}

Node::Node(Operators op, Term *t, Node *l, Node *r)
    : payload(op), term(t), left(l), right(r) 
{

}

bool Node::evaluate()
{
    if (payload == COMMA)
        return left->evaluate() && right->evaluate();
    else if (payload == SEMICOLON)
    {
        //Boolean OR will shortcut.
        //Use bitwise has the same effect, and the matching can be complete. 
        return left->evaluate() | right->evaluate(); 
    }
    else if (payload == EQUALITY)
    {
        return left->term->match(*right->term);
    }
    else
        return false;
}

