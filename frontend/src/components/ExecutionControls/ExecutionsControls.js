import './ExecutionsControls.css';

function ExecutionControls({ onStart, onStep, onReset }) {
  return (
    <div className='execution-controls'>
      <p className='title'>Controles de execução</p>
      <div className="buttons-div">
        <button onClick={onStart} className='buttons'>Iniciar</button>
        <button onClick={onStep} className='buttons'>Passo a Passo</button>
        <button onClick={onReset} className='reset-button'>Resetar</button>
      </div>
    </div>
  );
}

export default ExecutionControls;