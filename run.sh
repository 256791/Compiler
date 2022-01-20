#!/bin/bash
if test -f "compiler/last_compile.sha"; then
    sha=$(<compiler/last_compile.sha)
    cursha=$(./sha1dirsum.sh compiler/src)
    if [ "$sha" == "$cursha" ]
    then
        if [ -z "$1" ]
        then
            ./compiler/compiler < example.imp
        else
            ./compiler/compiler < $1
        fi
        exit 0
    fi
else
    touch "compiler/last_compile.sha"
fi

./build.sh

newsha=$(./sha1dirsum.sh compiler/src)
rm compiler/last_compile.sha
echo "$newsha" > "compiler/last_compile.sha"

if [ -z "$1" ]
then
    ./compiler/compiler < example.imp
else
    ./compiler/compiler < $1
fi