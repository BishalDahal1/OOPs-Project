//Real-time Chat Application (with Multi-Threading & Networking) 

#include "user.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>


User::User(std::string uname, std::string pass) {
    username = uname;
    password = pass;

}

std::string User::getUsername() const {
    return username;
}

bool User::checkPassword(const std::string& pass) const {
    return pass == password;
}

void User::addFriend(const std::string& friendName) {
    friends.insert(friendName);
}

bool User::isFriend(const std::string& friendName) const {
    return friends.find(friendName) != friends.end();
}

void User::receiveMessage(const Message& msg) {
    inbox.push(msg);
    history.push_back(msg);
}

void User::showInbox() const {
    std::queue<Message> copy = inbox;

    while (!copy.empty()) {
        const Message& msg = copy.front();

        std::cout << "From: " << msg.sender << "\n";
        std::cout << "Content: " << msg.content << "\n";

     
        std::time_t now = std::time(nullptr);
        double secondsAgo = std::difftime(now, msg.rawTime);
        int minutesAgo = static_cast<int>(secondsAgo / 60);

        std::cout << "Sent " << minutesAgo << " minute(s) ago\n";
        std::cout << "Exact Time: " << msg.timestamp << "\n\n";

        copy.pop();
    }
}

int User::getInboxSize() const {
    return inbox.size();
}

void User::clearInbox() {
     while(!inbox.empty())
     {
      inbox.pop();
     }
}

std::string User::getPassword() const{
     return password;
}

bool User::changePassword(const std::string oldPassword ,const std::string newPassword)
{
    if(oldPassword==password)
    {
        password=newPassword;
        return true;
    }
    else{
        std::cout << "Incorrect old password. Cannot change.\n";
        return false;
    }
}

bool User::changeUsername( const std::string& pass, const std::string& newUsername)
{
    if(password==pass)
    {
        username=newUsername;
        return true;
        
    }
    else{
        std::cout << "Incorrect old password. Cannot change.\n";

        return false;
    }
}

const std::list<Message>& User::getHistory() const {
    return history;
}