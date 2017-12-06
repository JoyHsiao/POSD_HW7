#include "struct.h"
#include "iterator.h"
Iterator <Term *> * Struct::createIterator()
{
  return new StructIterator<Term *>(this);
}

Iterator <Term *> * Struct::createBFSIterator()
{
  return new StructBFSIterator<Term *>(this);
}

Iterator <Term *> * Struct::createDFSIterator()
{
  return new StructDFSIterator<Term *>(this);
}

