#!/bin/bash
if test -f "compiler/last_compile.sha"; then
    sha=$(<compiler/last_compile.sha)
    cursha=$(./sha1dirsum.sh compiler/src)
    if [ "$sha" == "$cursha" ]
    then
        if [ -z "$1" ]
        then
            ./compiler/compiler example.imp
            maszyna_wirtualna/maszyna-wirtualna-cln prog.mr
        else
            ./compiler/compiler $1
            maszyna_wirtualna/maszyna-wirtualna-cln prog.mr
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
    ./compiler/compiler example.imp
    maszyna_wirtualna/maszyna-wirtualna-cln prog.mr
else
    ./compiler/compiler $1
    maszyna_wirtualna/maszyna-wirtualna-cln prog.mr
fi