#!/bin/sh

cp ~/compiler/pa3/src/semant ./semant_my

./lexer ./$1 | ./parser | ./semant_ref 2> 1.txt
./lexer ./$1 | ./parser | ./semant_my 2> 2.txt

./lexer ./$1 | ./parser | ./semant_ref 1> 3.txt
./lexer ./$1 | ./parser | ./semant_my 1> 4.txt
