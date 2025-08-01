# ARMLator

**ARMLator** é um simulador educacional da arquitetura ARM desenvolvido em C++.  
O projeto visa facilitar o aprendizado de assembly ARM, registradores e controle de fluxo através de uma simulação interativa em terminal.

---

## ✅ Funcionalidades já implementadas

- [x] Simulador de registradores R0 a R15
- [x] Execução de programas a partir de arquivos `.txt`
- [x] Instruções suportadas: `MOV`, `ADD`, `CMP`, `BEQ`, `BNE`, `NOP`
- [x] Suporte a literais (`#`) e registradores mistos
- [x] Impressão detalhada do estado dos registradores a cada instrução
- [x] Modo interativo: executar passo a passo ou pular para o final
- [x] Leitura de comentários com `;`
- [x] Código modularizado em arquivos `.hpp` e `.cpp`

---

## 🔧 Funcionalidades em desenvolvimento

- [ ] Suporte à instrução `SUB`
- [ ] Suporte à instrução `STR` e acesso indireto com `[Rn, #offset]`
- [ ] Saltos condicionais adicionais (`BGT`, `BLT`, `BGE`, `BLE`)
- [ ] Carregamento e escrita em memória simulada
- [ ] Representação gráfica via interface web (futuro com WebAssembly)
- [ ] Testes unitários e casos de teste automáticos
- [ ] Interpretador robusto com validação e mensagens de erro

---

## 📦 Como rodar o ARMLator localmente

g++ -std=c++17 -o armlator src/*.cpp -o armlator
./armlator.exe

