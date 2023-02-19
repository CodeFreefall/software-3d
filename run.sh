#!/bin/bash

if [ "$1" ];
then
    echo "Executing $1 branch...";

    if [[ "$1" == "main" ]]; then
        sh compile.sh
        ./main
    elif [[ "$1" == "debug" ]]; then
        sh compile.sh
        ./main
    elif [[ "$1" == "tests" ]]; then
        sh compile-tests.sh
        ./main
    elif [[ "$1" == "execute" ]]; then
        ./main
    else
        echo "Invalid usage of this command!";
    fi

else
    echo "usage: ./run [main|debug|tests|execute]";
fi