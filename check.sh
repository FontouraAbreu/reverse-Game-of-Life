#! /bin/bash
# ./check.sh [DIMENSAO X] [DIMENSAO Y]
# o script ira procurar por testes/tXxY.t1 e rodar o solver e o visualizador

# Checa se argumentos estão corretos ###########################################
[[ ${#} != 2 ]] && \
    echo -e "\e[1;31mERRO:\e[0m\tDimensões para teste!
\e[1;32mUSO:\e[0m\tcheck.sh [DIMENSAO X] [DIMENSAO Y]" && \
    exit 1

X=${1}
Y=${2}
DIMS="${X}x${Y}"

# Checa se arquivo testes/tXxY.in existe #######################################
[[ ! -f "testes/t"${DIMS}".t1" ]] && \
    echo -e "\e[1;31mERRO:\e[0m\tArquivo de teste não existe no diretório
testes/t${DIMS}.t1" && \
    exit 1

# Compila o solver e o simulador de regras #####################################
echo -e "\e[1;32mCOMPILANDO OS ARQUIVOS\e[0m"
make dimInclude
make gol
make visual
echo

# Roda e compara os resultados #################################################
./revgol < testes/t${DIMS}.t1 > testes/t${DIMS}.t0
./gol < testes/t${DIMS}.t0 > testes/t${DIMS}.t1.CORRETO
if diff testes/t${DIMS}.t1 testes/t${DIMS}.t1.CORRETO > /dev/null; then
    echo -e "\e[1;32mSEM DIFERENÇAS\e[0m\nRemovendo arquivos gerados..."
    rm testes/t${DIMS}.{t0,t1.CORRETO}
    exit 0
fi

./visual < testes/t${DIMS}.t1
echo
./visual < testes/t${DIMS}.t1.CORRETO
exit 1
