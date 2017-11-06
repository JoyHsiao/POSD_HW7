#ifndef PARSER_H
#define PARSER_H
#include <string>
using std::string;

#include "atom.h"
#include "number.h"
#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "struct.h"

class Parser{
public:
  Parser(Scanner scanner) : _scanner(scanner){}
  Term* createTerm(){
    int token = _scanner.nextToken();
    if(token == VAR){
      return new Variable(symtable[_scanner.tokenValue()].first);
    }else if(token == NUMBER){
      return new Number(_scanner.tokenValue());
    }else if(token == ATOM){
        int str_l=0, str_r=0;
        Atom* atom = new Atom(symtable[_scanner.tokenValue()].first);
        if(_scanner.currentChar() == '(' ) {
          _scanner.nextToken() ;
          vector<Term*> terms = getArgs();
          if(_currentToken == ')')
            return new Struct(*atom, terms);
        }
        else
          return atom;
    }else if(token == ARY){
        int ary_l=0, ary_r=0;
        vector<Term*> terms;
        if(symtable[_scanner.tokenValue()].first == "[")
        {
            ary_l++;
            _scanner.nextToken();
            if(symtable[_scanner.tokenValue()].first == "]"){
                ary_r++;
                if(ary_r > 0 && ary_l == ary_r){
                    if(ary_l > 0)
                        ary_l--;
                    terms = getArgs();
                    if(ary_r > 0)
                        ary_r--;
                }
            }
            return new List(terms);
        }
    }
    return nullptr;
  }

  vector<Term*> getArgs()
  {
    Term* term = createTerm();
    vector<Term*> args;
    if(term)
      args.push_back(term);
    while((_currentToken = _scanner.nextToken()) == ',') {
      args.push_back(createTerm());
    }
    return args;
  }



private:
  Scanner _scanner;
  int _currentToken;
};
#endif
