# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -g -I"C:\SFML\include" -L"C:\SFML\lib"

# Bibliotecas do SFML
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Arquivos de origem
SRCS = main.cpp base.cpp projectile.cpp enemy.cpp hero.cpp

# Arquivos objeto
OBJS = $(SRCS:.cpp=.o)

# Executável
TARGET = BaseDefender

# Regra padrão
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Regras de compilação
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	del /f main.o base.o projectile.o enemy.o hero.o BaseDefender.exe
