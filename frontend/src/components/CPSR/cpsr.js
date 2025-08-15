import './cpsr.css';

function toHex32(num) {
  const hex = num.toString(16).toUpperCase();
  const padded = hex.padStart(8, '0');
  return `0x${padded}`;
}


function CPSRComponent({ cpsr }) {
  return (
    <div className="cpsr">
      <p className='title'>CPSR</p>
      <div className='itens-div'>
        <div className='item-div'>
            <p className='label'>Valor</p>
            <div className='gap'></div>
            <p className='values'>{toHex32(cpsr.value)}</p>
        </div>
        <div className='item-div'>
          <p className='label'>Flags:</p>
          <div className='gap'></div>
          <div className='flags-div'>
            <p className='values'>N:{cpsr.n}</p>
            <p className='values'>Z:{cpsr.z}</p>
            <p className='values'>V:{cpsr.v}</p>
            <p className='values'>C:{cpsr.c}</p> 
          </div>
        </div>
      </div>
    </div>
  );
}

export default CPSRComponent;