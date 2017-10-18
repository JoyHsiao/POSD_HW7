#ifndef UTVARIABLE_H
#define UTVARIABLE_H
#include "term.h"
#include "variable.h"
#include "struct.h"
#include "atom.h"
#include "number.h"

TEST(Variable, constructor){
  Variable X("X");
  ASSERT_EQ("X", X._symbol);
}

TEST(Variable , matching){
  Atom tom("tom");
  Variable X("X");
  X.match(tom);
  ASSERT_EQ( "tom", X.value());
}

TEST (Variable , haveValue){
  Atom tom ("tom");
  Atom jerry ("jerry");
  Variable X("X");
  ASSERT_TRUE(X.match(tom));
  ASSERT_FALSE(X.match(jerry));
}

// --------------
TEST (Variable , Joy){
  Atom tom ("tom");
  Variable X("X");
  Variable Y("Y");
  X.match(tom);
  X.match(Y);
  ASSERT_EQ(X.value(),"Y");
  
  //ASSERT_TRUE(X.match(tom));
  //ASSERT_FALSE(X.match(jerry));
}

// ?- X=2.7182.
// X=2.7182
TEST(Variable , numE_to_varX){
    Variable X("X");
    Number N(2.7182);
    ASSERT_TRUE(X.match(N));
}

// ?- X=Y, X=1.
// Y=1
TEST (Variable, varY_to_varX_and_num1_to_varX) {
    Variable X("X");
    Variable Y("Y");
    Number N(1);
    X.match(Y);
    X.match(N);
    ASSERT_EQ( N.value(), X.value());
}
  
// ?- X=Y, Y=1.
// X=1
TEST (Variable, varY_to_varX_and_num1_to_varY) {
    Variable X("X");
    Variable Y("Y");
    Number N(1);
    X.match(Y);
    Y.match(N);
    ASSERT_EQ( X.value(), N.value());
}

// ?- X=X, X=1.
// X=1
TEST (Variable, varX_match_varX_and_num1_to_varX) {
    Variable X("X");
    Number N(1);
    X.match(X);
    X.match(N);
    ASSERT_EQ( X.value(), N.value());
}

// ?- Y=1, X=Y.
// X=1
TEST (Variable, num1_to_varY_and_varX_match_varY) {
    Variable Y("Y");
    Variable X("X");
    Number N(1);
    Y.match(N);
    X.match(Y);
    ASSERT_EQ( X.value(), N.value());
}

// ?- X=Y, Y=Z, Z=1
// X=1, Y=1, Z=1
TEST (Variable, num1_to_varZ_to_varY_to_varX) {
    Variable X("X");
    Variable Y("Y");
    Variable Z("Z");
    Number N(1);
    X.match(N);
    Y.match(N);
    Z.match(N);
    ASSERT_EQ( X.value(), N.value());
    ASSERT_EQ( Y.value(), N.value());
    ASSERT_EQ( Z.value(), N.value());
}

// ?- X=Y, X=Z, Z=1
// X=1, Y=1, Z=1
TEST (Variable, num1_to_varZ_to_varX_and_varY_to_varX) {
    Variable X("X");
    Variable Y("Y");
    Variable Z("Z");
    Number N(1);
    X.match(Y);
    X.match(Z);
    Z.match(N);
    ASSERT_EQ( X.value(), N.value());
    ASSERT_EQ( Y.value(), N.value());
    ASSERT_EQ( Z.value(), N.value());
}

// Give there is a Struct s contains Variable X
// And another Variable Y
// When Y matches Struct s
// Then #symbol() of Y should return "Y"
// And #value() of Y should return "s(X)"
TEST (Variable, Struct1) {
    Variable X("X");
    Variable Y("Y");
    std::vector<Term *> v ={&X};
    Struct s(Atom("s"), v);
    Y.match(s);
    ASSERT_EQ(Y.symbol(), "Y");
}

// Give there is a Struct s contains Variable X
// And another Variable Y
// When Y matches Struct s
// And X matches Atom "teddy"
// Then #symbol() of Y should return "Y"
// And #value() of Y should return "s(teddy)"
TEST (Variable, Struct2) {
    Variable X("X");
    Variable Y("Y");
    Atom teddy("teddy");
    std::vector<Term *> v ={&X};
    Struct s(Atom("s"), v);
    Y.match(s);
    X.match(teddy);
    ASSERT_EQ(X.value(), "teddy");
    ASSERT_EQ(Y.symbol(), "Y");
    ASSERT_EQ(Y.value(), "s(teddy)");
}

#endif
