CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lcryptopp -lpthread

SRCS = main.cpp Logger.cpp ServerConfig.cpp AuthDatabase.cpp Authenticator.cpp VectorProcessor.cpp Server.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
# Модульные тесты
test_main.o: test_main.cpp
	g++ -std=c++17 -Wall -Wextra -c test_main.cpp -o test_main.o

test: test_main.o Logger.o ServerConfig.o AuthDatabase.o Authenticator.o VectorProcessor.o
	g++ test_main.o Logger.o ServerConfig.o AuthDatabase.o Authenticator.o VectorProcessor.o -o test_server -lcryptopp -lpthread
	./test_server