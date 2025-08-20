#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "userManager.hpp"
#include "friendSystem.hpp"
#include "chatManager.hpp"

#define PORT 12345
#define MAX_CLIENTS 10

std::mutex mtx;
UserManager userManager;
FriendSystem friendSystem;
ChatManager chatManager(&userManager, &friendSystem);
std::map<int, std::string> clients;

void handleClient(int clientSock) {
    char buffer[2048];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSock, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) break;

        std::string msg(buffer);
        std::string response;

      
        std::vector<std::string> tokens;
        size_t pos = 0;
        while ((pos = msg.find('|')) != std::string::npos) {
            tokens.push_back(msg.substr(0, pos));
            msg.erase(0, pos + 1);
        }
        tokens.push_back(msg);

        mtx.lock();
        if (tokens[0] == "REGISTER") {
            bool ok = userManager.registerUser(tokens[1], tokens[2]);
            response = ok ? "SUCCESS|Registered" : "ERROR|Username exists";
        }
        else if (tokens[0] == "LOGIN") {
            User* u = userManager.login(tokens[1], tokens[2]);
            if (u) clients[clientSock] = tokens[1];
            response = u ? "SUCCESS|Logged in" : "ERROR|Invalid credentials";
        }
        else if (tokens[0] == "LOGOUT") {
            clients.erase(clientSock);
            response = "SUCCESS|Logged out";
        }
        else if (tokens[0] == "SEND_MSG") {
            chatManager.sendMessage(tokens[1], tokens[2], tokens[3]);
            response = "SUCCESS|Message sent";
        }
        else if (tokens[0] == "FRIEND_REQUEST") {
            friendSystem.sendFriendRequest(tokens[1], tokens[2]);
            response = "SUCCESS|Friend request sent";
        }
        else if (tokens[0] == "FRIEND_REQUEST_ACCEPT") {
            friendSystem.acceptFriendRequest(tokens[1], tokens[2]);
            response = "SUCCESS|Friend request accepted";
        }
        else if (tokens[0] == "FRIEND_REQUEST_REJECT") {
            friendSystem.rejectFriendRequest(tokens[1], tokens[2]);
            response = "SUCCESS|Friend request rejected";
        }
        else if (tokens[0] == "SHOW_INBOX") {
            std::ostringstream oss;
            std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf()); 
            chatManager.showInbox(tokens[1]);
            std::cout.rdbuf(oldCout);
            response = oss.str();
            if (response.empty()) response = "Inbox is empty.";
        }
        else if (tokens[0] == "CHANGE_PASSWORD") {
            bool ok = userManager.changeUserPassword(tokens[1], tokens[2], tokens[3]);
            response = ok ? "SUCCESS|Password changed" : "ERROR|Failed to change password";
        }
        else if (tokens[0] == "CHANGE_USERNAME") {
            bool ok = userManager.changeUserUsername(tokens[1], tokens[2], tokens[3]);
            response = ok ? "SUCCESS|Username changed" : "ERROR|Failed to change username";
        }
        else if (tokens[0] == "DELETE_USER") {
            bool ok = userManager.deleteUser(tokens[1], tokens[2]);
            response = ok ? "SUCCESS|User deleted" : "ERROR|Failed to delete user";
        }
        else if (tokens[0] == "VIEW_FRIENDS") {
            auto friends = friendSystem.getFriendList(tokens[1]);
            response = "Friends:";
            for (auto& f : friends) response += " " + f;
        }
        else if (tokens[0] == "VIEW_PENDING") {
            auto pending = friendSystem.getPendingRequests(tokens[1]);
            response = "Pending:";
            for (auto& p : pending) response += " " + p;
        }
        else {
            response = "ERROR|Unknown command";
        }
        mtx.unlock();

        send(clientSock, response.c_str(), response.size(), 0);
    }

    clients.erase(clientSock);
    close(clientSock);
}

int main() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) { perror("socket failed"); return 1; }

    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (::bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed"); return 1;
    }

    if (listen(serverSock, MAX_CLIENTS) < 0) {
        perror("listen failed"); return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSock < 0) { perror("accept failed"); continue; }

        std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        std::thread(handleClient, clientSock).detach();
    }

    close(serverSock);
    return 0;
}