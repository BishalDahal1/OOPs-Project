#ifndef CHATMANAGER_HPP
#define CHATMANAGER_HPP

#include "userManager.hpp"
#include "friendSystem.hpp" 
#include "message.hpp"

class ChatManager {
private:
    UserManager* userManager;
    FriendSystem* friendSystem;

public:
    ChatManager(UserManager* um, FriendSystem* fs);

    void sendMessage(const std::string& sender, const std::string& receiver, const std::string& content);
    void showInbox(const std::string& username);
};

#endif