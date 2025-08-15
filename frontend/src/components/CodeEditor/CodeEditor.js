import './CodeEditor.css';

function CodeEditor({ code, setCode }) {
  return (
    <div className="code-editor">
      <p className='title'>Código em Assembly</p>
      <textarea
        value={code}
        onChange={(e) => setCode(e.target.value)}
        placeholder="Digite ou cole seu código Assembly aqui..."
      />
    </div>
  );
}

export default CodeEditor;
