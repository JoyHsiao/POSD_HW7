#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <stack>
using std::string;
using std::stack;

#include "atom.h"
#include "number.h"
#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "struct.h"
#include "list.h"
#include "node.h"

#include "utParser.h"

#define DEBUG1
//#define DEBUG2
//#define DEBUG3

class Parser{
public:
  Parser(Scanner scanner) : _scanner(scanner), _terms(){}

  Term* createTerm(){
    int token = _scanner.nextToken();
    _currentToken = token;
    if(token == VAR){
      return new Variable(symtable[_scanner.tokenValue()].first);
    }else if(token == NUMBER){
      return new Number(_scanner.tokenValue());
    }else if(token == ATOM || token == ATOMSC){
      Atom* atom = new Atom(symtable[_scanner.tokenValue()].first);
      if(_scanner.currentChar() == '(' ) {
        std::cout<<"~~~~~"<<std::endl;
        return structure();
      }
      else{
        std::cout<<"~~~~~~~~~"<<std::endl;
        return atom;
      }
    }
    else if(token == '['){
      return list();
    }

    return nullptr;
  }

  void matchings(){
    Node *root;
    createTerms();
    prefix();
    createTree();
  }

  void createTree(){
    stack<Node *> tree;
    vector<Node *> eq;
    Node *root;
    int next;
    std::string cvt[] = {"SEMICOLON","COMMA", "EQUALITY","TERM"};
    for(int i=0; i<preNode.size(); i++){
        #ifdef DEBUG3
        if(preNode[i]->term)
            std::cout <<i<<"\t "<<preNode[i]->term->symbol() << std::endl;
        else
            std::cout<<i<<"\t "<<cvt[preNode[i]->payload]<< std::endl;
        #endif    

        if(preNode[i]->term){
            bool push = 1;
            if(tree.top()->left == NULL){
                if(preNode[i]->term->type() == "variable"){
                    if(eq.size()>0){
                        for(int j=0; j<eq.size(); j++){
                            if(eq[j]->term->symbol() == preNode[i]->term->symbol()){
                                push = 0;
                                preNode[i] = eq[j];
                                break;
                            }
                        }
                    }
                    else{
                        if(push)
                            eq.push_back(preNode[i]);
                    }
                }
                tree.top()->left = preNode[i];
            }
            else if(tree.top()->right == NULL){
                if(preNode[i]->term->type() == "variable")
                    eq.push_back(preNode[i]);
                tree.top()->right = preNode[i];
                root = tree.top();
                tree.pop();
                
                if(!tree.empty()){
                    if(tree.top()->left == NULL)
                        tree.top()->left = root;
                    else if(tree.top()->right == NULL){
                        tree.top()->right = root;
                        root = tree.top();
                        tree.pop();
                    }
                }
            }
        }
        else{ //op
            tree.push(preNode[i]);
        }
    }


    while(!tree.empty()){
        if(tree.top()->right != NULL){
            root = tree.top();
            tree.pop();
            if(tree.top()->left == NULL)
                tree.top()->left = root;
            else if(tree.top()->right == NULL)
                tree.top()->right = root;
        }
        else if(tree.top()->right == NULL){
            tree.top()->right = root;
            tree.pop();
        }
    }

    #ifdef DEBUG3
    for(int i=0; i<preNode.size(); i++){
        std::cout<<i<<std::endl;
        if(preNode[i]->term)
            std::cout<< preNode[i]->term->symbol() << std::endl;
        else //op
            std::cout<<cvt[preNode[i]->payload]<< std::endl;

        if(preNode[i]->left != NULL){
            if(preNode[i]->left->term){
                std::cout <<"left  "<<preNode[i]->left->term->symbol() << ' ';
            }
            else
                std::cout<<"left  "<<cvt[preNode[i]->left->payload]<< ' ';
        }
        if(preNode[i]->right != NULL){
            if(preNode[i]->right->term){
                std::cout <<"right "<<preNode[i]->right->term->symbol() << std::endl;
            }
            else
                std::cout<<"right "<<cvt[preNode[i]->right->payload]<< std::endl;
        }
    }
    #endif
  }

  Node* expressionTree(){
    return preNode[0];
  }

  Term * structure() {
        std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    Atom structName = Atom(symtable[_scanner.tokenValue()].first);
    int startIndexOfStructArgs = _terms.size();
    _scanner.nextToken();
    createTerms();
    if(_currentToken == ')')
    {
      vector<Term *> args(_terms.begin() + startIndexOfStructArgs, _terms.end());
      _terms.erase(_terms.begin() + startIndexOfStructArgs, _terms.end());
        std::cout<<"#### "<<std::endl;
      return new Struct(structName, args);
    } else {
      throw string("unexpected token");
    }
  }

  Term * list() {
    int startIndexOfListArgs = _terms.size();
    createTerms();
    if(_currentToken == ']')
    {
      vector<Term *> args(_terms.begin() + startIndexOfListArgs, _terms.end());
      _terms.erase(_terms.begin() + startIndexOfListArgs, _terms.end());
      return new List(args);
    } else {
      throw string("unexpected token");
    }
  }

  vector<Term *> & getTerms() {
    return _terms;
  }

private:
  FRIEND_TEST(ParserTest, createArgs);
  FRIEND_TEST(ParserTest,ListOfTermsEmpty);
  FRIEND_TEST(ParserTest,listofTermsTwoNumber);
  FRIEND_TEST(ParserTest, createTerm_nestedStruct3);

  void createTerms() {
    Term* term = createTerm();
    Node* node = new Node(TERM,term, NULL, NULL);
    _node.push_back(node);
    if(term!=nullptr)
    {
      _terms.push_back(term);
      _currentToken = _scanner.nextToken();
      while(_currentToken == ',' || _currentToken == ';' || _currentToken == '=') {
        switch (_currentToken){
            case ',':
                node = new Node(COMMA,NULL, NULL, NULL);
                _node.push_back(node);
                break;
            case ';':
                node = new Node(SEMICOLON,NULL, NULL, NULL);
                _node.push_back(node);
                break;
            case '=':
                node = new Node(EQUALITY,NULL, NULL, NULL);
                _node.push_back(node);
                break;
        }
            
        term = createTerm();
        _terms.push_back(term);
        Node* node = new Node(TERM,term, NULL, NULL);
        _node.push_back(node);
        _currentToken = _scanner.nextToken();
      }
    }
    
    #ifdef DEBUG1
    std::string cvt[] = {"SEMICOLON","COMMA", "EQUALITY","TERM"};
    for(int i=0; i<_node.size();i++)
    {
        if(_node[i]->term)
            std::cout << _node[i]->term->symbol() << ' ';
        else //op
            std::cout<< cvt[_node[i]->payload]<< ' ';
    }
    std::cout << std::endl;
    #endif
  }

  void prefix(){
    vector<Node *> var;
    for(int i=0;i<_node.size();i++){
        if(_node[i]->term){
            //std::cout << "operators push "<<_node[i]->term->symbol() << std::endl;
            stackOperators.push(_node[i]);
        }
        else{ //op
            std::string cvt[] = {"SEMICOLON","COMMA", "EQUALITY","TERM"};
            //std::cout<< cvt[_node[i]->payload]<< std::endl;
            if(stackOperator.empty()){
                stackOperator.push(_node[i]);
            }
            else if(stackOperator.top()->payload>_node[i]->payload){
                if(_node[i]->payload == SEMICOLON || _node[i]->payload == COMMA){
                    Node *trNode;
                    preNode.push_back(_node[i]);
                    preNode.push_back(stackOperator.top());
                    stackOperator.pop();
                    trNode = stackOperators.top();
                    stackOperators.pop();
                    preNode.push_back(stackOperators.top());
                    preNode.push_back(trNode);
                    stackOperators.pop();
                }
                else{
                    preNode.push_back(stackOperator.top());
                    stackOperator.pop();
                }
            }
            else{
                stackOperator.push(_node[i]);
            }
        }
    }
    if(!stackOperator.empty()){
        for(int i=0; i<stackOperator.size(); i++){
            Node *trNode;
            preNode.push_back(stackOperator.top());
            stackOperator.pop();
            trNode = stackOperators.top();
            stackOperators.pop();
            preNode.push_back(stackOperators.top());
            preNode.push_back(trNode);
            stackOperators.pop();
        }
    }
    #ifdef DEBUG2
    std::string cvt[] = {"SEMICOLON","COMMA", "EQUALITY","TERM"};
    std::cout<<"pre size= "<< preNode.size()<<std::endl;
    
    for(int i=0; i<preNode.size(); i++){
        if(preNode[i]->term)
            std::cout << preNode[i]->term->symbol() << ' ';
        else //op
            std::cout<< cvt[preNode[i]->payload]<< ' ';
    }
    std::cout << std::endl;
    #endif

  }

  stack<Node *> stackOperator;
  stack<Node *> stackOperators;
  vector<Term *> _terms;
  vector<Node *> _node;
  vector<Node *> preNode;
  Scanner _scanner;
  int _currentToken;
};
#endif
