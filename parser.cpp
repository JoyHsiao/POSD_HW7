#include "parser.h"

Parser::Parser(Scanner const& scanner)
    : _scanner(scanner), _currentToken(NONE), _exprRoot(nullptr) 

{

}
Term* Parser::createTerm()
{
    int token = _scanner.nextToken();
    _currentToken = token;
    if(token == VAR)
    {
        return new Variable(symtable[_scanner.tokenValue()].first);
    }
    else if(token == NUMBER)
    {
        return new Number(_scanner.tokenValue());
    }
    else if(token == ATOM || token == ATOMSC)
    {
        Atom* atom = new Atom(symtable[_scanner.tokenValue()].first);
        if(_scanner.currentChar() == '(' ) 
        {
            return structure();
        }
        else
            return atom;
    }
    else if(token == '[')
    {
        return list();
    }
    return nullptr;
}

Term* Parser::structure() 
{
    Atom structName = Atom(symtable[_scanner.tokenValue()].first);
    int startIndexOfStructArgs = _terms.size();
    _scanner.nextToken();
    createTerms();
    if(_currentToken == ')')

    {
        vector<Term *> args(_terms.begin() + startIndexOfStructArgs, _terms.end());
        _terms.erase(_terms.begin() + startIndexOfStructArgs, _terms.end());
        return new Struct(structName, args);
    }
    else 
    {
        throw std::string("unexpected token");
    }
}

void Parser::matchings()
{
    Term *term;
    Node *node;
    _terms.clear();
    _nodes.clear();
    while((term = createTerm()))
    {
        int c = _scanner.currentChar();
        _terms.push_back(term);
        _nodes.push_back(new Node(TERM, term, nullptr, nullptr));
        if(c == '.')
        {
            break;
        }
        else if(c == ',' ||  c == '=' ||  c == ';')
        {
            switch(c)
            {
            case ',':
                node = new Node(COMMA);
                break;
            case '=':
                node = new Node(EQUALITY);
                break;
            case ';':
                node = new Node(SEMICOLON);
                break;
            }
            _nodes.push_back(node);
            _scanner.nextToken();
        }
    }
    generateExpTree();
    maintainSameTerms(new Context, _exprRoot);
}

Node* Parser::expressionTree()
{
    return _exprRoot;
}

Term* Parser::list() 
{
    int startIndexOfListArgs = _terms.size();
    createTerms();
    if(_currentToken == ']')

    {
        vector<Term *> args(_terms.begin() + startIndexOfListArgs, _terms.end());
        _terms.erase(_terms.begin() + startIndexOfListArgs, _terms.end());
        return new List(args);
    }
    else 
    {
        throw std::string("unexpected token");
    }
}

std::vector<Term *>& Parser::getTerms() 
{
    return _terms;
}


void Parser::createTerms()
{
    Term* term = createTerm();
    if(term != nullptr)
    {
        _terms.push_back(term);
        while((_currentToken = _scanner.nextToken()) == ',')  
            _terms.push_back(createTerm());
    }
}

void Parser::generateExpTree()
{
//        0      1         2     3
    //SEMICOLON, COMMA, EQUALITY, TERM
    Node *node, *n1, *n2;
    std::stack<Node*> term_stk, op_stk, stk; 
    int priority[] = {2, 3, 4}; 
    std::vector<Node*> prefix(_nodes.size());
    int p = prefix.size() - 1;
    for(int i=static_cast<int>(_nodes.size()-1); i>=0; i--)
    {
        node = _nodes[i]; 
        if(node->payload == TERM)
        {
            term_stk.push(node);
        }
        else
        {
            while(!op_stk.empty() &&
                   priority[op_stk.top()->payload] >= priority[node->payload])
            {
                if(!term_stk.empty())
                {
                    n2 = term_stk.top();
                    term_stk.pop();
                    n1 = term_stk.top();
                    term_stk.pop();
                    prefix[p--] = n1;
                    prefix[p--] = n2;
                }
                prefix[p--] = op_stk.top();
                op_stk.pop();
            }
            op_stk.push(node);
        }
    }
    while(!op_stk.empty())
    {
        if(!term_stk.empty())
        {
            n2 = term_stk.top();
            term_stk.pop();
            n1 = term_stk.top();
            term_stk.pop();
            prefix[p--] = n1;
            prefix[p--] = n2;
        }
        prefix[p--] = op_stk.top();
        op_stk.pop();
    }

    int payload;
    for(int i = static_cast<int>(prefix.size() - 1); i>=0; i--)
    {
        node    = prefix[i]; 
        payload = node->payload;
        if(payload != TERM)
        {
            n1 = stk.top();
            stk.pop();
            n2 = stk.top();
            stk.pop();
            node->left  = n1;
            node->right = n2;
        }
        stk.push(node);
    }  
    _exprRoot = stk.top();
    stk.pop();
}

void Parser::maintainSameTerms(Context *context, Node *node)
{
    if(node->payload == TERM)
    {
        Term *t;
        if(termExist(context, node->term->symbol(), &t))
        {
            for(size_t i=0;i<_terms.size();i++)
            {
                if(node->term == _terms[i])
                    _terms[i] = t;
            }
            node->term = t;
        }
        else
        {
            termAddToContext(context, *node->term);
        }
    }
    if(node->left)
        maintainSameTerms(context, node->left);
    if(node->right)
    {
        if(node->payload == SEMICOLON)
            context = new Context;    
        maintainSameTerms(context, node->right);
    }
}
