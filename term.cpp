#include "atom.h"
#include "variable.h"
#include "iterator.h"
#include <typeinfo>

Iterator * Term::createIterator(){
  return new NullIterator(this);
}
