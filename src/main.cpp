#include "interpretador.hpp"
#include "simulador.hpp"
#include <iostream>

int main() {
    std::string caminho = "programa.txt";
    auto instrucoes = interpretar_arquivo(caminho);
    executar_programa(instrucoes);
    return 0;
}
