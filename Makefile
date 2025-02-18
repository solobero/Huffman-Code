# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Archivos fuente y objeto
SRC = huffman.cpp main.cpp file_io.cpp
OBJ = huffman.o main.o file_io.o
TARGET = huffman_programa

# Regla principal
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Regla para compilar los archivos fuente a objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de archivos objeto y binarios
clean:
	rm -f $(OBJ) $(TARGET)