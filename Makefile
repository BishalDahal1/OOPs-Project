CXX = g++
CXXFLAGS = -std=c++17 -pthread -Iinclude

SRC_DIR = src
BUILD_DIR = build

SERVER_SRC = $(SRC_DIR)/server.cpp $(SRC_DIR)/user.cpp $(SRC_DIR)/userManager.cpp \
             $(SRC_DIR)/friendSystem.cpp $(SRC_DIR)/chatManager.cpp $(SRC_DIR)/message.cpp
CLIENT_SRC = $(SRC_DIR)/client.cpp $(SRC_DIR)/user.cpp $(SRC_DIR)/userManager.cpp \
             $(SRC_DIR)/friendSystem.cpp $(SRC_DIR)/chatManager.cpp $(SRC_DIR)/message.cpp

SERVER_BIN = $(BUILD_DIR)/chatServer
CLIENT_BIN = $(BUILD_DIR)/chatClient

all: server client

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

clean:
	rm -f $(BUILD_DIR)/*