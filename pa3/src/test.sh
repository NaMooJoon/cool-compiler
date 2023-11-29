#! /bin/bash
test_in_directory() {
    for filename in $1; do
        ./lexer_ref $filename | ./parser_ref 1> a.out 2> a.err
        ./lexer_ref $filename | ./parser 1> b.out 2> b.err

        output=`diff a.out b.out; diff a.err b.err`
        if [ "$output" ]; then
            echo "![ERR] $filename"
            diff a.err b.err
        else
            echo "[DONE] $filename"
        fi
    done
}