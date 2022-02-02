if [ -z "$1" ]
then
    ./compiler/kompilator example.imp program.mr
    maszyna_wirtualna/maszyna-wirtualna-cln program.mr
else
    ./compiler/kompilator $1 program.mr
    maszyna_wirtualna/maszyna-wirtualna-cln program.mr
fi