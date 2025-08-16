O ARMLator é um simulador educacional interativo da arquitetura ARM, desenvolvido para auxiliar no aprendizado de programação em assembly e no entendimento do funcionamento interno de um processador. O sistema permite que o usuário escreva programas em uma versão simplificada da linguagem assembly ARM, carregue esse código e acompanhe sua execução em tempo real. Durante a simulação, é possível visualizar os valores dos registradores R0 a R15, o estado das flags do registrador de status (CPSR) e o conteúdo da memória. O simulador oferece tanto a execução passo a passo, permitindo analisar cada instrução individualmente, quanto a execução contínua, que roda o programa até o fim automaticamente.

O projeto é dividido em duas partes principais. O backend, implementado em C++, é responsável por interpretar e executar as instruções assembly, atualizando registradores, memória e flags a cada operação. Esse núcleo é conectado a um servidor intermediário em Node.js, que faz a ponte entre o simulador em C++ e o frontend. O frontend, por sua vez, foi construído em React e fornece a interface gráfica, onde o usuário pode escrever o código, carregar o programa e acompanhar a execução de forma visual e interativa.

O parser do ARMLator é tolerante e aceita instruções com vírgulas, colchetes em operações de memória e comentários no estilo ;. As instruções suportadas incluem operações aritméticas, lógicas, de desvio condicional e acesso à memória, cobrindo um conjunto essencial para demonstrar a lógica de execução de programas em assembly ARM. Assim, o ARMLator não apenas executa programas simples, como somas ou multiplicações, mas também permite implementar rotinas mais complexas, como cálculo de fatorial, sequência de Fibonacci ou contagem de bits.

Dessa forma, o ARMLator cumpre seu objetivo de ser uma ferramenta didática, aproximando estudantes da arquitetura ARM e tornando tangível o funcionamento de registradores, memória e flags durante a execução de programas em baixo nível.

Como rodar o ARMLator do zero. São necessários 3 termianis
1) Compilar o backend (C++)
   a) cd back
   b) g++ -std=c++17 -O2 src/*.cpp -o armlator
3) Iniciar o servidor Node.js
   a) cd server
   b) npm install (primeira vez)
   c) npm start
4) Rodar o frontend
   a) cd frontend
   b) npm install (priemira vez)
   c) npm start
   
