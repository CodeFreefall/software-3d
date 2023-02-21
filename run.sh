#!/bin/bash

if [ "$1" ];
then
    echo "Executing $1 branch...";

    EXECUTABLE="main"

    if [[ "$1" == "main" ]]; then
        g++ -g -o main main.cpp -Isrc/
    elif [[ "$1" == "debug" ]]; then
        g++ -g -o main main.cpp -Isrc/
    elif [[ "$1" == "tests" ]]; then
        EXECUTABLE="tests"
        g++ -g -o tests main_tests.cpp -Isrc/ -DRUN_TESTS
    else
        echo "Invalid usage of this command!";
        exit 1
    fi

    if [ $? -eq 0 ]; then
        echo "Compilation successful. Running the application ${EXECUTABLE}..."
        ./$EXECUTABLE
    else
        echo "Compilation failed!"
    fi

else
    echo "usage: ./run [main|debug|tests]";
    exit 1
fi