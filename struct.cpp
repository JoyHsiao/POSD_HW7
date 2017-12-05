#include "struct.h"
#include "iterator.h"
Iterator * Struct::createIterator()
{
  return new StructIterator(this);
}

Iterator * Struct::createBFSIterator()
{
  return new StructBFSIterator(this);
}

Iterator * Struct::createDFSIterator()
{
  return new StructDFSIterator(this);
}

