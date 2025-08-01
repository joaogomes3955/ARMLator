#include "simulador.hpp"
#include <iostream>
#include <array>
#include <string>

void imprimir_registradores(const std::array<int, 16>& regs, int pc) {
    std::cout << "[PC = " << pc << "] Estado dos registradores:\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << i << " = " << regs[i] << '\n';
    }
    std::cout << "--------------------------\n";
}

void executar_programa(const std::vector<Instrucao>& instrucoes) {
    std::array<int, 16> registradores{};
    int pc = 0;
    int flags_z = 0;
    int flags_n = 0;
    bool modo_step = true;

    while (pc < static_cast<int>(instrucoes.size())) {
        const auto& inst = instrucoes[pc];

        switch (inst.tipo) {
            case TipoInstrucao::NOP:        pc++; break;
            case TipoInstrucao::MOV_IMD:    registradores[inst.destino] = inst.imediato; pc++; break;
            case TipoInstrucao::MOV_REG:    registradores[inst.destino] = registradores[inst.origem1]; pc++; break;
            case TipoInstrucao::ADD_IMD:    registradores[inst.destino] = registradores[inst.origem1] + inst.imediato; pc++; break;
            case TipoInstrucao::ADD_REG:    registradores[inst.destino] = registradores[inst.origem1] + registradores[inst.origem2]; pc++; break;
            case TipoInstrucao::CMP_IMD: {
                int result = registradores[inst.destino] - inst.imediato;
                flags_z = (result == 0);
                flags_n = (result < 0);
                pc++;
                break;
            }
            case TipoInstrucao::CMP_REG: {
                int result = registradores[inst.destino] - registradores[inst.origem1];
                flags_z = (result == 0);
                flags_n = (result < 0);
                pc++;
                break;
            }
            case TipoInstrucao::BEQ:        pc += (flags_z ? inst.offset : 1); break;
            case TipoInstrucao::BNE:        pc += (!flags_z ? inst.offset : 1); break;
            default:
                std::cerr << "[erro] Instrução inválida no PC = " << pc << '\n';
                return;
        }

        imprimir_registradores(registradores, pc);

        if (modo_step) {
            std::cout << "[enter] para próximo passo, [f] para finalizar execução: ";
            std::string entrada;
            std::getline(std::cin, entrada);
            if (entrada == "f" || entrada == "F") {
                modo_step = false;
            }
        }
    }

    std::cout << "\n[Final] Registradores:\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << i << " = " << registradores[i] << '\n';
    }
}
