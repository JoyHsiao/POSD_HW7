#ifndef GLOBAL_H
#define GLOBAL_H

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "term.h"
#include "struct.h"
#include "list.h"
#include "number.h"

#define NONE       (-1) 
#define EOS        (0)
#define NUMBER     (256)
#define ATOM       (257)
#define ATOMSC     (258)
#define VAR        (259)
typedef std::vector<std::pair<std::string, Term*>> Context;
extern std::vector<std::pair<std::string, int>>   symtable;

bool  isSpecialCh(char c);
bool  symbolExist(std::string s, int & val);
bool  termExist(Context *context, std::string s, Term **term);
//void  termAddToContext(std::vector<std::pair<std::string, Term*>> context, Term &term);
void  termAddToContext(Context *context, Term &term);
void  printSymbolTable();
#endif
