#!/bin/sh

cp ~/compiler/pa3/src/semant ./semant_my

./lexer ./$1 | ./parser | ./semant_ref 2> 1.txt
./lexer ./$1 | ./parser | ./semant_my 2> 2.txt

