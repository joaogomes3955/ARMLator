#include "../include/interpretador.hpp"
#include "../include/instrucao.hpp"
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
        } else if (opcode == "ADD" || opcode == "SUB") {
            std::string dest, op1, op2;
            ss >> dest >> op1 >> op2;
            inst.destino = std::stoi(dest.substr(1));
            inst.origem1 = std::stoi(op1.substr(1));

            if (op2[0] == '#') {
                inst.tipo = (opcode == "ADD") ? TipoInstrucao::ADD_IMD : TipoInstrucao::SUB_IMD;
                inst.imediato = std::stoi(op2.substr(1));
            } else {
                inst.tipo = (opcode == "ADD") ? TipoInstrucao::ADD_REG : TipoInstrucao::SUB_REG;
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

        } else if (opcode == "B") {
            inst.tipo = TipoInstrucao::B;
            ss >> inst.offset;

        } else if (opcode == "MUL" || opcode == "DIV") {
            std::string dest, op1, op2;
            ss >> dest >> op1 >> op2;
            inst.destino = std::stoi(dest.substr(1));
            inst.origem1 = std::stoi(op1.substr(1));
            inst.origem2 = std::stoi(op2.substr(1));

            inst.tipo = (opcode == "MUL") ? TipoInstrucao::MUL : TipoInstrucao::DIV;

        } else if (opcode == "AND" || opcode == "ORR" || opcode == "EOR") {
            std::string dest, op1, op2;
            ss >> dest >> op1 >> op2;
            inst.destino = std::stoi(dest.substr(1));
            inst.origem1 = std::stoi(op1.substr(1));
            inst.origem2 = std::stoi(op2.substr(1));

            if (opcode == "AND") inst.tipo = TipoInstrucao::AND;
            else if (opcode == "ORR") inst.tipo = TipoInstrucao::ORR;
            else inst.tipo = TipoInstrucao::EOR;

        } else if (opcode == "LSL" || opcode == "LSR") {
            std::string dest, src, shift;
            ss >> dest >> src >> shift;
            inst.destino = std::stoi(dest.substr(1));
            inst.origem1 = std::stoi(src.substr(1));
            inst.imediato = std::stoi(shift.substr(1));
            inst.tipo = (opcode == "LSL") ? TipoInstrucao::LSL : TipoInstrucao::LSR;

        } else if (opcode == "LDR" || opcode == "STR") {
            std::string reg, addr;
            ss >> reg >> addr;
            inst.destino = std::stoi(reg.substr(1));
            inst.origem1 = std::stoi(addr.substr(2, addr.find(']') - 2)); // Ex: [R3] -> pega 3
            inst.tipo = (opcode == "LDR") ? TipoInstrucao::LDR : TipoInstrucao::STR;

        } else if (opcode == "LDR" || opcode == "STR") {
            std::string reg, addr1, addr2;
            ss >> reg >> addr1;

            inst.destino = std::stoi(reg.substr(1));
            inst.tipo = (opcode == "LDR") ? TipoInstrucao::LDR : TipoInstrucao::STR;

            if (addr1[0] == '[') {
                // Remove colchetes
                //addr1.erase(std::remove(addr1.begin(), addr1.end(), '['), addr1.end());
                //addr1.erase(std::remove(addr1.begin(), addr1.end(), ']'), addr1.end());

                size_t virgula = addr1.find(',');
                if (virgula != std::string::npos) {
                    // [Rn, #offset]
                    std::string base = addr1.substr(0, virgula);
                    std::string offset = addr1.substr(virgula + 1);
                    inst.origem1 = std::stoi(base.substr(1));
                    inst.imediato = std::stoi(offset.substr(1));
                } else {
                    // [Rn]
                    inst.origem1 = std::stoi(addr1.substr(1));
                    inst.imediato = 0;
                }

            } else {
                if (addr1.back() == ',') {
                    addr1.pop_back(); // Remove vírgula se estiver grudada
                }

                inst.origem1 = std::stoi(addr1.substr(1));

                if (ss >> addr2) {
                    if (addr2[0] == '#') {
                        inst.imediato = std::stoi(addr2.substr(1));
                    } else {
                        std::cerr << "[erro] Offset inválido em LDR/STR: " << addr2 << '\n';
                        continue;
                    }
                } else {
                    inst.imediato = 0;
                }
            }
        } else {
            std::cerr << "[erro] Instrução inválida: " << linha << '\n';
            continue;
        }

        instrucoes.push_back(inst);
    }

    return instrucoes;
}
