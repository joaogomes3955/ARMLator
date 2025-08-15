import './SidePanel.css'
import CPSRComponent from '../CPSR/cpsr';
import Memory from '../Memory/Memory';
import Registers from '../Registers/Registers'

function SidePanel({ registers, cpsr, memory }) {
  return (
    <div className="side-panel">
      <Registers registers={registers} />
      <CPSRComponent cpsr={cpsr} />
      <Memory memory={memory} />
    </div>
  );
}

export default SidePanel;
