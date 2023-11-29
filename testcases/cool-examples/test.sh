#!/bin/sh

cp ~/compiler/pa3/src/semant ./semant_my

./lexer ./$1 | ./parser | ./semant_ref  1> 1.txt
./lexer ./$1 | ./parser | ./semant_my 1> 2.txt

