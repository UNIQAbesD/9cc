#!/bin/bash
assert()
{
    

    expected="$1";
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit a
    fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 "12 + 34 - 5"
assert 35 "48-(2+11)"
assert 50 "25*(10-6)/(5-3)"
assert 0 "25<10"
assert 1 "(1+2*(3+4)*5)==71"

echo OK