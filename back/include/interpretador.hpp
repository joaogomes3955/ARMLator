#pragma once
#include "instrucao.hpp"
#include <vector>
#include <string>

std::vector<Instrucao> interpretar_arquivo(const std::string& caminho);
