#ifndef UTITERATOR_H
#define UTITERATOR_H

#include "term.h"
#include "struct.h"
#include "variable.h"
#include "atom.h"
#include "number.h"
#include "list.h"
#include "iterator.h"

TEST(iterator, first) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });
    // StructIterator it(&s);
    Iterator <Term *> *itStruct = s.createIterator();
    // Iterator& itStruct = it;
    // ASSERT_EQ(it.first()->symbol());
    itStruct->first();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("t(X, 2)", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("Y", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_TRUE(itStruct->isDone());
}

TEST(iterator, nested_iterator) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });
    Iterator <Term *> *itStruct = s.createIterator();
    //StructIterator it(&s);
    itStruct->first();
    itStruct->next();
    Struct *s2 = dynamic_cast<Struct *>(itStruct->currentItem());
    Iterator <Term *> *itStruct2 = s2->createIterator();
    //StructIterator it2(s2);
    itStruct2->first();
    ASSERT_EQ("X", itStruct2->currentItem()->symbol());
    ASSERT_FALSE(itStruct2->isDone());
    itStruct2->next();
    ASSERT_EQ("2", itStruct2->currentItem()->symbol());
    ASSERT_FALSE(itStruct2->isDone());
    itStruct2->next();
    ASSERT_TRUE(itStruct2->isDone());
}

TEST(iterator, firstList) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    List l({ &one, &t, &Y });
    //ListIterator <Term *> *it(&l);
    Iterator <Term *> *itList = l.createIterator();
    itList->first();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("t(X, 2)", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("Y", itList->currentItem()->symbol());
    itList->next();
    ASSERT_TRUE(itList->isDone());
}

TEST(iterator, NullIterator){
  Number one(1);
  NullIterator <Term *> nullIterator(&one);
  nullIterator.first();
  ASSERT_TRUE(nullIterator.isDone());
  Iterator <Term *> *it = one.createIterator();
  it->first();
  ASSERT_TRUE(it->isDone());
}

// Struct DFS test1
TEST(iterator, createDFSIterator1){
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two }); // t(X,2)
    Struct s(Atom("s"), { &one, &t, &Y }); // s(1, t(X, 2), Y)
    Iterator <Term *> *itStruct = s.createDFSIterator();
    itStruct->first();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("t(X, 2)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("X", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("2", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("Y", itStruct->currentItem()->symbol());
}

// Struct DFS test2
TEST(iterator, createDFSIterator2){
    Number one(1);
    Number two(2);
    Atom tom("tom");
    Variable X("X");
    vector<Term *> args = { &one, &two};
    List l(args);
    Struct u(Atom("u"), { &X }); // u(X)
    Struct t(Atom("t"), { &u, &one }); // t(u(X), 1)
    Struct s(Atom("s"), { &t, &tom, &l }); // s(t(u(X),1), tom, [1, 2])
    Iterator <Term *> *itStruct = s.createDFSIterator();
    itStruct->first();
    ASSERT_EQ("t(u(X), 1)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("u(X)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("X", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("tom", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("[1, 2]", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("2", itStruct->currentItem()->symbol());
}

// Struct BFS test1
TEST(iterator, createBFSIterator1){
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two }); // t(X,2)
    Struct s(Atom("s"), { &one, &t, &Y }); // s(1, t(X, 2), Y)
    Iterator <Term *> *itStruct = s.createBFSIterator();
    itStruct->first();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("t(X, 2)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("Y", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("X", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("2", itStruct->currentItem()->symbol());
}

// Struct BFS test2
TEST(iterator, createBFSIterator2){
    Number one(1);
    Number two(2);
    Atom tom("tom");
    Variable X("X");
    vector<Term *> args = { &one, &two};
    List l(args);
    Struct u(Atom("u"), { &X }); // u(X)
    Struct t(Atom("t"), { &u, &one }); // t(u(X), 1)
    Struct s(Atom("s"), { &t, &tom, &l }); // s(t(u(X),1), tom, [1, 2])
    Iterator <Term *> *itStruct = s.createBFSIterator();
    itStruct->first();
    ASSERT_EQ("t(u(X), 1)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("tom", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("[1, 2]", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("u(X)", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_EQ("2", itStruct->currentItem()->symbol());
}

// List BFS test1
TEST(iterator, createBFSIteratorList1){
    Number one(1);
    Number two(2);
    Variable X("X");
    Variable Y("Y");
    vector<Term *> args1 = {&one, &X, &Y};
    List l1(args1); // [1, X, Y]
    vector<Term *> args2 = {&l1, &two};
    List l2(args2); // [[1, X, Y], 2]
    Iterator <Term *> *itList = l2.createBFSIterator();
    itList->first();
    ASSERT_EQ("[1, X, Y]", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("2", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("X", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("Y", itList->currentItem()->symbol());
}

// List BFS test2
TEST(iterator, createBFSIteratorList2){
    Number one(1);
    vector<Term *> args1 = { &one};
    List l1(args1); // [1]
    Number two(2);
    Atom tom("tom");
    Variable X("X");
    vector<Term *> args2 = { &one, &two};
    List l2(args2); // [1, 2]
    Struct u(Atom("u"), { &X }); // u(X)
    Struct t(Atom("t"), { &u, &one }); // t(u(X), 1)
    vector<Term *> args3 = { &l1, &l2, &t};
    List l3(args3); // [[1], [1, 2], t(u(X), 1)]
    Iterator <Term *> *itList = l3.createBFSIterator();
    itList->first();
    ASSERT_EQ("[1]", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("[1, 2]", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("t(u(X), 1)", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("2", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("u(X)", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("X", itList->currentItem()->symbol());
}

// List DFS test1
TEST(iterator, createDFSIteratorList1){
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two }); // t(X,2)
    Struct s(Atom("s"), { &one, &t, &Y }); // s(1, t(X, 2), Y)
    Iterator <Term *> *itList = s.createDFSIterator();
    itList->first();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("t(X, 2)", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("X", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("2", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("Y", itList->currentItem()->symbol());
}

// List DFS test2
TEST(iterator, createDFSIteratorList2){
    Number one(1);
    Number two(2);
    Atom tom("tom");
    Variable X("X");
    vector<Term *> args = { &one, &two};
    List l(args);
    Struct u(Atom("u"), { &X }); // u(X)
    Struct t(Atom("t"), { &u, &one }); // t(u(X), 1)
    Struct s(Atom("s"), { &t, &tom, &l }); // s(t(u(X),1), tom, [1, 2])
    Iterator <Term *> *itList = s.createDFSIterator();
    itList->first();
    ASSERT_EQ("t(u(X), 1)", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("u(X)", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("X", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("tom", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("[1, 2]", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    itList->next();
    ASSERT_EQ("2", itList->currentItem()->symbol());
}
#endif
