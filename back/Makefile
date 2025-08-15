# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g -Iinclude

# Diretórios
SRC_DIR = src
BUILD_DIR = build

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Executável
TARGET = app

# Regra principal: gerar executável
all: $(TARGET)

# Linka os objetos para gerar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compila .cpp em .o, cria build se não existir
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar objetos e executável
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
