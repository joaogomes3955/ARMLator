#include <iostream>
#include "../include/interpretador.hpp"
#include "../include/simulador.hpp"

int main() {
    std::string caminho = "program.txt";
    auto instrucoes = interpretar_arquivo(caminho);
    executar_programa(instrucoes);
    return 0;
}
