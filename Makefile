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