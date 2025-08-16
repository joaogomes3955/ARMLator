#include "../include/simulador.hpp"
#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <iomanip>

// =====================================================================
// Flags CPSR (modelo simplificado)
static bool flagN = false; // Negative
static bool flagZ = false; // Zero
static bool flagC = false; // Carry (no SUB: C=1 se NÃO houve borrow)
static bool flagV = false; // Overflow (sinal)
// =====================================================================

// Helpers para atualizar flags -----------------------------------------
static inline void atualizaFlagsADD(int32_t a, int32_t b, int64_t res64) {
    int32_t r = static_cast<int32_t>(res64);
    flagN = (r & 0x80000000) != 0;
    flagZ = (r == 0);
    // carry em soma sem sinal (estouro de 32 bits)
    flagC = (static_cast<uint64_t>(static_cast<uint32_t>(a)) +
             static_cast<uint64_t>(static_cast<uint32_t>(b))) > 0xFFFFFFFFull;
    // overflow em soma com sinal
    flagV = ((~(a ^ b)) & (a ^ r) & 0x80000000) != 0;
}

static inline void atualizaFlagsSUB(int32_t a, int32_t b, int64_t res64) {
    int32_t r = static_cast<int32_t>(res64);
    flagN = (r & 0x80000000) != 0;
    flagZ = (r == 0);
    // em ARM, C=1 se NÃO houve borrow -> a >= b em aritmética sem sinal
    flagC = static_cast<uint32_t>(a) >= static_cast<uint32_t>(b);
    // overflow em subtração com sinal
    flagV = (((a ^ b) & (a ^ r)) & 0x80000000) != 0;
}

static inline void atualizaFlagsCMP(int32_t a, int32_t b) {
    int64_t res64 = static_cast<int64_t>(a) - static_cast<int64_t>(b);
    atualizaFlagsSUB(a, b, res64);
}

// Impressão de estado para o bridge Node fazer o parse -----------------
static inline void dump_estado(const std::array<int,16>& regs,
                               int pc,
                               const std::array<int,32>& memoria) {
    // Registradores
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << i << " = " << regs[i] << "\n";
    }
    // Flags (N Z C V) em linhas separadas
    std::cout << "N = " << (flagN ? 1 : 0) << "\n";
    std::cout << "Z = " << (flagZ ? 1 : 0) << "\n";
    std::cout << "C = " << (flagC ? 1 : 0) << "\n";
    std::cout << "V = " << (flagV ? 1 : 0) << "\n";

    // PC
    std::cout << "PC = " << pc << "\n";

    // Snapshot de memória (8 words a partir de 0x1000, apenas um exemplo visual)
    int base = 0x1000;
    for (int i = 0; i < 8 && i < static_cast<int>(memoria.size()); ++i) {
        int addr = base + i * 4;
        uint32_t val = static_cast<uint32_t>(memoria[i]);
        std::cout << "MEM 0x" << std::uppercase << std::hex << addr
                  << " = 0x" << std::setw(8) << std::setfill('0') << val
                  << std::nouppercase << std::dec << "\n";
    }

    // Separador de bloco
    std::cout << "----\n";
}

// =====================================================================

void executar_programa(const std::vector<Instrucao>& instrucoes) {
    std::array<int, 16> registradores{}; // R0..R15
    std::array<int, 32> memoria{};       // memória simplificada (indexada em words)
    int pc = 0;
    bool modo_step = true;

    // zera flags no início
    flagN = flagZ = flagC = flagV = false;

    while (pc < static_cast<int>(instrucoes.size())) {
        const auto& inst = instrucoes[pc];

        switch (inst.tipo) {
            case TipoInstrucao::NOP:
                pc++;
                break;

            // MOV
            case TipoInstrucao::MOV_IMD:
                registradores[inst.destino] = inst.imediato;
                pc++;
                break;
            case TipoInstrucao::MOV_REG:
                registradores[inst.destino] = registradores[inst.origem1];
                pc++;
                break;

            // ADD
            case TipoInstrucao::ADD_IMD: {
                int32_t a = registradores[inst.origem1];
                int32_t b = inst.imediato;
                int64_t r64 = static_cast<int64_t>(a) + static_cast<int64_t>(b);
                registradores[inst.destino] = static_cast<int32_t>(r64);
                atualizaFlagsADD(a, b, r64);
                pc++;
                break;
            }
            case TipoInstrucao::ADD_REG: {
                int32_t a = registradores[inst.origem1];
                int32_t b = registradores[inst.origem2];
                int64_t r64 = static_cast<int64_t>(a) + static_cast<int64_t>(b);
                registradores[inst.destino] = static_cast<int32_t>(r64);
                atualizaFlagsADD(a, b, r64);
                pc++;
                break;
            }

            // SUB
            case TipoInstrucao::SUB_IMD: {
                int32_t a = registradores[inst.origem1];
                int32_t b = inst.imediato;
                int64_t r64 = static_cast<int64_t>(a) - static_cast<int64_t>(b);
                registradores[inst.destino] = static_cast<int32_t>(r64);
                atualizaFlagsSUB(a, b, r64);
                pc++;
                break;
            }
            case TipoInstrucao::SUB_REG: {
                int32_t a = registradores[inst.origem1];
                int32_t b = registradores[inst.origem2];
                int64_t r64 = static_cast<int64_t>(a) - static_cast<int64_t>(b);
                registradores[inst.destino] = static_cast<int32_t>(r64);
                atualizaFlagsSUB(a, b, r64);
                pc++;
                break;
            }

            // MUL / DIV (não atualizam flags neste modelo, só resultado)
            case TipoInstrucao::MUL:
                registradores[inst.destino] = registradores[inst.origem1] * registradores[inst.origem2];
                pc++;
                break;

            case TipoInstrucao::DIV: {
                if (registradores[inst.origem2] != 0) {
                    registradores[inst.destino] = registradores[inst.origem1] / registradores[inst.origem2];
                } else {
                    std::cerr << "[erro] Divisão por zero no PC = " << pc << '\n';
                    return;
                }
                pc++;
                break;
            }

            // Lógicas (mantemos flags como estão; se quiser, pode atualizar N/Z do resultado)
            case TipoInstrucao::AND:
                registradores[inst.destino] = registradores[inst.origem1] & registradores[inst.origem2];
                pc++;
                break;

            case TipoInstrucao::ORR:
                registradores[inst.destino] = registradores[inst.origem1] | registradores[inst.origem2];
                pc++;
                break;

            case TipoInstrucao::EOR:
                registradores[inst.destino] = registradores[inst.origem1] ^ registradores[inst.origem2];
                pc++;
                break;

            // Comparação (atualiza flags, não escreve registradores)
            case TipoInstrucao::CMP_IMD: {
                int32_t a = registradores[inst.destino];
                int32_t b = inst.imediato;
                atualizaFlagsCMP(a, b);
                pc++;
                break;
            }
            case TipoInstrucao::CMP_REG: {
                int32_t a = registradores[inst.destino];
                int32_t b = registradores[inst.origem1];
                atualizaFlagsCMP(a, b);
                pc++;
                break;
            }

            // Branches (offset relativo ao PC; unidade = instruções)
            case TipoInstrucao::BEQ:
                pc += (flagZ ? inst.offset : 1);
                break;

            case TipoInstrucao::BNE:
                pc += (!flagZ ? inst.offset : 1);
                break;

            case TipoInstrucao::B:
                pc += inst.offset;
                break;

            // Shifts
            case TipoInstrucao::LSL:
                registradores[inst.destino] = registradores[inst.origem1] << inst.imediato;
                pc++;
                break;

            case TipoInstrucao::LSR:
                registradores[inst.destino] = static_cast<unsigned int>(registradores[inst.origem1]) >> inst.imediato;
                pc++;
                break;

            // Load / Store (endereçamento direto base+offset na memória word-indexed)
            case TipoInstrucao::LDR: {
                int idx = registradores[inst.origem1] + inst.imediato;
                if (idx >= 0 && idx < static_cast<int>(memoria.size()))
                    registradores[inst.destino] = memoria[idx];
                else
                    std::cerr << "[aviso] LDR fora da memória (idx=" << idx << ")\n";
                pc++;
                break;
            }
            case TipoInstrucao::STR: {
                int idx = registradores[inst.origem1] + inst.imediato;
                if (idx >= 0 && idx < static_cast<int>(memoria.size()))
                    memoria[idx] = registradores[inst.destino];
                else
                    std::cerr << "[aviso] STR fora da memória (idx=" << idx << ")\n";
                pc++;
                break;
            }

            default:
                std::cerr << "[erro] Instrução inválida no PC = " << pc << '\n';
                return;
        }

        // Imprime o estado após executar a instrução
        dump_estado(registradores, pc, memoria);

        // Modo step interativo (ENTER=passo, 'f'=finaliza step)
        if (modo_step) {
            std::cout << "[enter] para próximo passo, [f] para finalizar execução: ";
            std::string entrada;
            std::getline(std::cin, entrada);
            if (entrada == "f" || entrada == "F") {
                modo_step = false;
            }
        }
    }

    // Dump final (opcional)
    dump_estado(registradores, pc, memoria);
}
