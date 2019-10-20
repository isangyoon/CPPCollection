#!/bin/bash

tn=1
t() {
    echo Test $tn $@
}
pass() {
    echo Test $tn OK
    tn=$(($tn+1))
}
fail() {
    echo Test $tn FAILED
    exit 1
}

t program should run
./main >/dev/null 2>&1 && pass || fail
t program should print Hello world
cmp <(./main) <(echo Hello world) && pass || fail

echo ALL TESTS PASSED
