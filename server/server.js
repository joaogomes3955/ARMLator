import express from "express";
import cors from "cors";
import { spawn } from "child_process";
import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Caminhos
const BACKEND_DIR = path.resolve(__dirname, "../back");
const EXEC_PATH = path.resolve(
  BACKEND_DIR,
  process.platform === "win32" ? "armlator.exe" : "armlator"
);

const app = express();
app.use(cors());
app.use(express.json({ limit: "2mb" }));

let child = null;
let stdoutBuf = "";

// REGEX
const reRegLine = /R(\d+)\s*=\s*(-?\d+)/g;
const rePC = /PC\s*=\s*(-?\d+)/;
const reN = /N\s*[:=]\s*(\d+)/;
const reZ = /Z\s*[:=]\s*(\d+)/;
const reC = /C\s*[:=]\s*(\d+)/;
const reV = /V\s*[:=]\s*(\d+)/;
const reMem = /MEM\s*0x([0-9a-fA-F]+)\s*=\s*0x([0-9a-fA-F]+)/g;

function parseState(text) {
  // pega só o último bloco (após o último "----")
  const parts = text.split("----");
  const last = parts[parts.length - 2] || text; // -2 pois o split deixa string vazia no fim

  const regs = new Array(16).fill(0);
  let m;
  while ((m = reRegLine.exec(last)) !== null) {
    const idx = parseInt(m[1], 10);
    const val = parseInt(m[2], 10);
    if (idx >= 0 && idx < 16) regs[idx] = val;
  }

  const pc = (last.match(rePC)?.[1]) ? parseInt(last.match(rePC)[1], 10) : null;
  const flags = {
    N: (last.match(reN)?.[1]) ? parseInt(last.match(reN)[1], 10) : 0,
    Z: (last.match(reZ)?.[1]) ? parseInt(last.match(reZ)[1], 10) : 0,
    C: (last.match(reC)?.[1]) ? parseInt(last.match(reC)[1], 10) : 0,
    V: (last.match(reV)?.[1]) ? parseInt(last.match(reV)[1], 10) : 0,
  };

  const memory = [];
  while ((m = reMem.exec(last)) !== null) {
    const addrHex = m[1].toUpperCase();
    const valHex = m[2].toUpperCase();
    memory.push({ addr: `0x${addrHex}`, value: `0x${valHex}` });
  }

  return { regs, pc, flags, memory };
}

function ensureBinaryExists() {
  if (!fs.existsSync(EXEC_PATH)) {
    throw new Error(`Binário não encontrado: ${EXEC_PATH}`);
  }
}

function startChild() {
  ensureBinaryExists();
  child = spawn(EXEC_PATH, [], { cwd: BACKEND_DIR });
  stdoutBuf = "";
  child.stdout.setEncoding("utf8");
  child.stdout.on("data", chunk => { stdoutBuf += chunk; });
  child.stderr.on("data", chunk => { console.error("[SIM ERR]", chunk.toString()); });
  child.on("exit", code => { console.log("[SIM EXIT]", code); child = null; });
}

app.post("/load", (req, res) => {
  const { programText } = req.body;
  try {
    fs.writeFileSync(path.join(BACKEND_DIR, "program.txt"), programText ?? "", "utf8");
    if (child) { child.kill("SIGKILL"); child = null; }
    startChild();
    res.json({ ok: true });
  } catch (e) {
    res.status(500).json({ ok: false, error: e.message });
  }
});

app.post("/step", (_req, res) => {
  try {
    if (!child) startChild();
    child.stdin.write(process.platform === "win32" ? "\r\n" : "\n");
    setTimeout(() => {
      const state = parseState(stdoutBuf);
      res.json({ ok: true, state });
    }, 180); // extra margem pro Windows
  } catch (e) {
    res.status(500).json({ ok: false, error: e.message });
  }
});

app.post("/run", (req, res) => {
  const { steps = 50 } = req.body || {};
  if (!child) startChild();
  for (let i = 0; i < steps; i++) child.stdin.write(process.platform === "win32" ? "\r\n" : "\n");
  setTimeout(() => {
    const state = parseState(stdoutBuf);
    res.json({ ok: true, state });
  }, Math.min(1200, steps * 20));
});

app.post("/reset", (_req, res) => {
  try {
    if (child) { child.kill("SIGKILL"); child = null; }
    startChild();
    res.json({ ok: true });
  } catch (e) {
    res.status(500).json({ ok: false, error: e.message });
  }
});

app.get("/state", (_req, res) => {
  const state = parseState(stdoutBuf);
  res.json({ ok: true, state });
});

const PORT = 3001;
app.listen(PORT, () => {
  console.log(`ARMLator bridge on http://localhost:${PORT}`);
  console.log(`[BACKEND_DIR] ${BACKEND_DIR}`);
  console.log(`[EXEC_PATH] ${EXEC_PATH}`);
});
