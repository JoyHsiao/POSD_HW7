#include "global.h"
std::vector<std::pair<std::string, int>> symtable;
bool isSpecialCh(char c)
{
  return c == '+'
      // || c == '=' // ... the matching operator
         || c == '-'
         || c == '*'
         || c == '/'
         || c == '<'
         || c == '>'
         || c == '.'
         || c == '&'
         || c == '\\'
         || c == '~'
         || c == '^'
         || c == '$'
         || c == '#'
         || c == '@'
         || c == '?'
         || c == ':';
}

bool symbolExist(std::string s, int & val)
{
    bool found = false;
    val = -1;
    std::vector<std::pair<std::string, int>>::iterator it =
        find_if(symtable.begin(), symtable.end(),
                [s](std::pair<std::string, int> ele)
                {
                    return ele.first == s;
                });
    found = symtable.end() != it;
    if (found)
        val = it - symtable.begin();
    return found;
}
    
bool termExist(Context *context, std::string s, Term **term)
{
    bool found = false;
    *term = nullptr;
    std::vector<std::pair<std::string, Term*>>::iterator it =
        find_if(context->begin(), context->end(),
                [s](std::pair<std::string, Term*> ele)
                {
                    return ele.first == s;
                });
    found = context->end() != it;
    if (found)
        *term = it->second;
    return found;
}

void termAddToContext(Context *context, Term &term)
{
    Struct *s = dynamic_cast<Struct*>(&term);
    List   *l = dynamic_cast<List*>(&term);    
    Term   *t;
    if(s)
    {
        for(int i=0;i<s->arity();i++) 
        {
            t = s->args(i);
            if(!termExist(context, t->symbol(), &t))
                termAddToContext(context, *s->args(i));
            else
                s->setArgs(i, t);        
        }
    }
    else if(l)
    {
        for(int i=0;i<l->arity();i++) 
        {
            t = l->args(i);
            if(!termExist(context, t->symbol(), &t))
                termAddToContext(context, *l->args(i));
            else
                l->setArgs(i, t);        
        }
    }
    context->push_back(std::pair<std::string, Term*>(term.symbol(), &term));
}

void printSymbolTable()
{
    for(size_t i=0;i<symtable.size();i++)
    {
        printf("[%zd] -> %s, %d\n", i, symtable[i].first.c_str(), symtable[i].second);
    }
}
