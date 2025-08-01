#include "interpretador.hpp"
#include "instrucao.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

std::vector<Instrucao> interpretar_arquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    std::vector<Instrucao> instrucoes;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        // Remove espaços em branco no início/fim
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        linha.erase(linha.find_last_not_of(" \t\r\n") + 1);

        // Ignora comentários ou linhas em branco
        if (linha.empty() || linha[0] == ';') continue;

        std::istringstream ss(linha);
        std::string opcode;
        ss >> opcode;

        Instrucao inst;
        if (opcode == "NOP") {
            inst.tipo = TipoInstrucao::NOP;
        } else if (opcode == "MOV") {
            std::string dest, val;
            ss >> dest >> val;

            inst.destino = std::stoi(dest.substr(1));

            if (val[0] == '#') {
                inst.tipo = TipoInstrucao::MOV_IMD;
                inst.imediato = std::stoi(val.substr(1));
            } else {
                inst.tipo = TipoInstrucao::MOV_REG;
                inst.origem1 = std::stoi(val.substr(1));
            }
        } else if (opcode == "ADD") {
            std::string dest, op1, op2;
            ss >> dest >> op1 >> op2;
            inst.destino = std::stoi(dest.substr(1));
            inst.origem1 = std::stoi(op1.substr(1));

            if (op2[0] == '#') {
                inst.tipo = TipoInstrucao::ADD_IMD;
                inst.imediato = std::stoi(op2.substr(1));
            } else {
                inst.tipo = TipoInstrucao::ADD_REG;
                inst.origem2 = std::stoi(op2.substr(1));
            }
        } else if (opcode == "CMP") {
            std::string op1, op2;
            ss >> op1 >> op2;
            inst.destino = std::stoi(op1.substr(1));

            if (op2[0] == '#') {
                inst.tipo = TipoInstrucao::CMP_IMD;
                inst.imediato = std::stoi(op2.substr(1));
            } else {
                inst.tipo = TipoInstrucao::CMP_REG;
                inst.origem1 = std::stoi(op2.substr(1));
            }
        } else if (opcode == "BEQ") {
            inst.tipo = TipoInstrucao::BEQ;
            ss >> inst.offset;
        } else if (opcode == "BNE") {
            inst.tipo = TipoInstrucao::BNE;
            ss >> inst.offset;
        } else {
            std::cerr << "[erro] Instrução inválida: " << linha << '\n';
            continue;
        }

        instrucoes.push_back(inst);
    }

    return instrucoes;
}
