import './App.css';
import { useState } from 'react';
import Header from './components/Header/Header';
import CodeEditor from './components/CodeEditor/CodeEditor';
import SidePanel from './components/SidePanel/SidePanel';
import ExecutionControls from './components/ExecutionControls/ExecutionsControls';
import Registrador from './classes/Registrador';
import Memoria from './classes/Memoria'
import CPSR from './classes/CPSR';

function App() {
  const [registradores, setRegistradores] = useState({
    registrador1: new Registrador("R0", 10, ""),
    registrador2: new Registrador("R1", 20, ""),
    registrador3: new Registrador("R2", 30, ""),
    registrador4: new Registrador("R3", 40, ""),
    registrador5: new Registrador("R4", 50, ""),
    registrador6: new Registrador("R5", 60, ""),
    registrador7: new Registrador("R6", 70, ""),
    registrador8: new Registrador("R7", 80, ""),
    registrador9: new Registrador("R8", 90, ""),
    registrador10: new Registrador("R9", 100, ""),
    registrador11: new Registrador("R10", 110, ""),
    registrador12: new Registrador("R11", 120, ""),
    registrador13: new Registrador("R12", 130, ""),
    registrador14: new Registrador("R13", 140, "SP (Stack Pointer)"),
    registrador15: new Registrador("R14", 150, "LR (Link Register)"),
    registrador16: new Registrador("R15", 160, "PC (Program Counter)"),
  });

  const [memoria, setmemoria] = useState({
    memoria1: new Memoria("0x 1000", 10),
    memoria2: new Memoria("0x 1004", 20),
    memoria3: new Memoria("0x 1008", 30),
    memoria4: new Memoria("0x 100C", 40),
    memoria5: new Memoria("0x 1010", 50),
    memoria6: new Memoria("0x 1014", 60),
    memoria7: new Memoria("0x 1018", 70),
    memoria8: new Memoria("0x 101C", 80)
  });

  const [cpsr, setcpsr] = useState(new CPSR(12, "0", "0", "0", "0"));

  return (
    <div className='app-content'>
      <Header />
      <div className="main-content">
        <div className='editor'>
          <CodeEditor />
          <ExecutionControls />
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
