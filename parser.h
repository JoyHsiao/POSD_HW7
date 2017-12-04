#ifndef PARSER_H
#define PARSER_H

#include "atom.h"
#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "number.h"
#include "struct.h"
#include "node.h"
#include <stack>
#ifndef FRIEND_TEST
    #include <gtest/gtest_prod.h>
#endif
/*
BNF Rule:


    TermExp   -> atom | number | var | struct | list
    TermsExp  -> term rest | e 
    rest      -> ',' term rest | e
    MatchExp  -> term & term
    AtomExp   -> Atom
    NumberExp -> Number
    VarExp    -> Var
    StructExp -> atom '(' terms ')'
    ListExp   -> '[' terms ']'
*/

//Interpreter Pattern
class Parser
{
public:
    Parser(Scanner const& scanner);
    Term* createTerm();
    Term* structure();
    void  matchings();
    Node* expressionTree();
    Term* list();
    std::vector<Term*>& getTerms();
private:
    FRIEND_TEST(ParserTest, createArgs);
    FRIEND_TEST(ParserTest, ListOfTermsEmpty);
    FRIEND_TEST(ParserTest, listofTermsTwoNumber);
    FRIEND_TEST(ParserTest, createTerm_nestedStruct3);
    vector<Term*> _terms;
    vector<Node*> _nodes;
    Scanner _scanner;
    int _currentToken;
    Node *_exprRoot;
    void createTerms();
    void generateExpTree();
    void maintainSameTerms(Context *context, Node *node);
};
#endif
