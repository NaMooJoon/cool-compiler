#!/bin/sh

./lexer ./$1 | ./parser 2> 1out.txt
./mylexer ./$1 | ./myparser 2> 2out.txt

#./lexer ../parser_testcases/$1 | ./parser 2> out1.txt
#./lexer ../parser_testcases/$1 | ./myparser 2> out2.txt
