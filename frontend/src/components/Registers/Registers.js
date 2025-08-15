import './Registers.css';

function toHex32(num) {
  const hex = num.toString(16).toUpperCase();
  const padded = hex.padStart(8, '0');
  return `0x${padded}`;
}


function Registers({ registers }) {
  return (
    <div className='register-div'>
      <p className='title'>Registradores</p>
      <div className="registers">
        {Object.entries(registers).map(([key, register]) => (
          <div key={key} className="register">
            <div className='span-div'>
              <span className='span-register'>{register.nome}</span>
              <span className='span-description'>{register.description}</span>
            </div>
            <span className='span-value'>{toHex32(register.valor)}</span>
          </div>
        ))}
      </div>
    </div>
  );
}

export default Registers;
