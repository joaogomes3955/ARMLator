import './Memory.css'

function toHex32(num) {
  const hex = num.toString(16).toUpperCase();
  const padded = hex.padStart(8, '0');
  return `0x${padded}`;
}


function Memory({ memory }) {
  return (
    <div className="memory">
      <p className='title'>Memória</p>
      <div className='table'>
        <div className='table-title-div'>
          <span>Endereço</span>
          <span>Valor</span>
        </div>
        {Object.entries(memory).map(([key, item]) => (
          <div key={key} className="table-content-div">
            <span className='address-value'>{item.endereco}</span>
            <span className='number-value'>{toHex32(item.valor)}</span>
          </div>
        ))}
      </div>
    </div>
  );
}

export default Memory;
