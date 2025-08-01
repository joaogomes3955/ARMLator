#pragma once

enum class TipoInstrucao {
    NOP,
    MOV_IMD,
    MOV_REG,
    ADD_IMD,
    ADD_REG,
    CMP_IMD,
    CMP_REG,
    BEQ,
    BNE
};

struct Instrucao {
    TipoInstrucao tipo;
    int destino = 0;
    int origem1 = 0;
    int origem2 = 0;
    int imediato = 0;
    int offset = 0;
};
