// frontend/src/App.js
import "./App.css";
import { useState } from "react";

// Componentes
import Header from "./components/Header/Header";
import CodeEditor from "./components/CodeEditor/CodeEditor";
import SidePanel from "./components/SidePanel/SidePanel";
import ExecutionControls from "./components/ExecutionControls/ExecutionsControls";

// Modelos
import Registrador from "./classes/Registrador";
import Memoria from "./classes/Memoria";
import CPSR from "./classes/CPSR";

// API (servidor ponte em http://localhost:3001)
import { loadProgram, step, run as runSteps, resetSim } from "./api";

function App() {
  // === Editor ===
  const [code, setCode] = useState(
`MOV R0 #0
MOV R1 #5
ADD R0 R0 #1
CMP R0 R1
BNE -2
NOP`
  );

  // === Estados da UI ===
  const [registradores, setRegistradores] = useState(() => {
    const obj = {};
    for (let i = 0; i < 16; i++) {
      const nome = `R${i}`;
      const desc =
        i === 13 ? "SP (Stack Pointer)" :
        i === 14 ? "LR (Link Register)" :
        i === 15 ? "PC (Program Counter)" : "";
      obj[`registrador${i + 1}`] = new Registrador(nome, 0, desc);
    }
    return obj;
  });

  const [memoria, setMemoria] = useState({
    memoria1: new Memoria("0x1000", "0x00000000"),
    memoria2: new Memoria("0x1004", "0x00000000"),
    memoria3: new Memoria("0x1008", "0x00000000"),
    memoria4: new Memoria("0x100C", "0x00000000"),
    memoria5: new Memoria("0x1010", "0x00000000"),
    memoria6: new Memoria("0x1014", "0x00000000"),
    memoria7: new Memoria("0x1018", "0x00000000"),
    memoria8: new Memoria("0x101C", "0x00000000"),
  });

  const [cpsr, setCpsr] = useState(new CPSR(0, 0, 0, 0, 0));

  // === Mappers (dump JSON -> modelos) ===
  function mapRegs(regsArray) {
    const obj = {};
    for (let i = 0; i < 16; i++) {
      const nome = `R${i}`;
      const desc =
        i === 13 ? "SP (Stack Pointer)" :
        i === 14 ? "LR (Link Register)" :
        i === 15 ? "PC (Program Counter)" : "";
      obj[`registrador${i + 1}`] = new Registrador(
        nome,
        Number(regsArray?.[i] ?? 0),
        desc
      );
    }
    return obj;
  }

  function mapCpsr(flags) {
    const N = Number(flags?.N ?? 0);
    const Z = Number(flags?.Z ?? 0);
    const C = Number(flags?.C ?? 0);
    const V = Number(flags?.V ?? 0);
    // CPSR(value, z, c, v, n)
    return new CPSR(0, Z, C, V, N);
  }

  function mapMem(memList) {
    const obj = {};
    const list = Array.isArray(memList) ? memList : [];
    for (let i = 0; i < Math.min(8, list.length); i++) {
      obj[`memoria${i + 1}`] = new Memoria(list[i].addr, list[i].value);
    }
    return obj;
  }

  // === Handlers ===
  async function onStart() {
    const r = await loadProgram(code);
    if (!r?.ok) { alert(r?.error || "Falha ao carregar"); return; }
    // dá 1 passo para sincronizar com o primeiro dump do simulador
    const s = await step();
    if (s?.ok) {
      setRegistradores(mapRegs(s.state?.regs));
      setCpsr(mapCpsr(s.state?.flags));
      setMemoria(prev => ({ ...prev, ...mapMem(s.state?.memory) }));
    }
  }

  async function onStep() {
    const s = await step();
    if (s?.ok) {
      setRegistradores(mapRegs(s.state?.regs));
      setCpsr(mapCpsr(s.state?.flags));
      setMemoria(prev => ({ ...prev, ...mapMem(s.state?.memory) }));
    }
  }

  async function onRun() {
    // executa em lotes até não avançar mais (ou alcançar o limite)
    let lastPc = -1;
    for (let i = 0; i < 50; i++) {          // até 50 lotes
      const r = await runSteps(200);        // cada lote ~200 passos
      if (!r?.ok) break;

      const st = r.state;
      setRegistradores(mapRegs(st?.regs));
      setCpsr(mapCpsr(st?.flags));
      setMemoria(prev => ({ ...prev, ...mapMem(st?.memory) }));

      const pc = st?.pc ?? null;
      if (pc === null || pc === lastPc) break; // fim (pc não mudou)
      lastPc = pc;
    }
  }

  async function onReset() {
    await resetSim();
  }

  // === Render ===
  return (
    <div className="app-content">
      <Header />
      <div className="main-content">
        <div className="editor">
          <CodeEditor code={code} setCode={setCode} />
          <ExecutionControls
            onStart={onStart}
            onStep={onStep}
            onRun={onRun}
            onReset={onReset}
          />
        </div>
        <SidePanel
          registers={registradores}
          cpsr={cpsr}
          memory={memoria}
        />
      </div>
    </div>
  );
}

export default App;
