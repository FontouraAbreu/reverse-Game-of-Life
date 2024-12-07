#! /bin/bash

[[ ! -f config ]] && echo -e "\e[0;31mSEM ARQUIVO DE CONFIGURAÇÃO\e[0m" && \
    exit 1

FILE=`cat config | grep ENTRYFILE | cut -d= -f2`
TIMEF=`cat config | grep TIME | cut -d= -f2`

# Compila os programas necessário ##############################################
[[ ! -f revgol ]] && make -s verbose
[[ ! -f rand ]] && make -s rand
[[ ! -f gol ]] && make -s gol
################################################################################

# Roda os testes para cada arquivo #############################################
for i in `cat ${FILE}`
do
    X=`echo ${i} | cut -d, -f1`
    Y=`echo ${i} | cut -d, -f2`
    DIM=`echo ${i} | cut -d, -f3`

    ./rand ${X} ${Y} ${DIM} | ./gol > testes/t${X}x${Y}.t1
    if [ ${TIMEF} = 0 ] ; then
        ./revgol < testes/t${X}x${Y}.t1
    else
        time ./revgol < testes/t${X}x${Y}.t1
    fi
    echo
done
