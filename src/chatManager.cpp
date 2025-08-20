#include "chatManager.hpp"
#include <iostream>

ChatManager::ChatManager(UserManager* um, FriendSystem* fs) {
    userManager = um;
    friendSystem = fs;
}

void ChatManager::sendMessage(const std::string& sender, const std::string& receiver, const std::string& content) {
    if (sender == receiver) {
        std::cout << "You cannot message yourself.\n";
        return;
    }

    if (!userManager->userExists(receiver)) {
        std::cout << "User '" << receiver << "' does not exist.\n";
        return;
    }

    if (!friendSystem->isFriend(sender, receiver)) {
        std::cout << "You can only message your friends.\n";
        return;
    }

    if (content.empty()) {
        std::cout << "Cannot send an empty message.\n";
        return;
    }

    Message msg(sender, content);
    User* recipient = userManager->getUser(receiver);
    recipient->receiveMessage(msg);

    std::cout << "Message sent to " << receiver << ".\n";
}

void ChatManager::showInbox(const std::string& username) {
    if (!userManager->userExists(username)) {
        std::cout << "User does not exist.\n";
        return;
    }

    User* user = userManager->getUser(username);
    user->showInbox();  
}