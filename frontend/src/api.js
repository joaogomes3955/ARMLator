const API = "http://localhost:3001";

export async function loadProgram(programText) {
  const r = await fetch(`${API}/load`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ programText })
  });
  return r.json();
}

export async function step() {
  const r = await fetch(`${API}/step`, { method: "POST" });
  return r.json();
}

export async function run(steps = 50) {
  const r = await fetch(`${API}/run`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ steps })
  });
  return r.json();
}

export async function resetSim() {
  const r = await fetch(`${API}/reset`, { method: "POST" });
  return r.json();
}
