#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <queue>
#include <list>
#include <unordered_set>
#include "message.hpp"

class User {
private:
    std::string username;
    std::string password;
    std::unordered_set<std::string> friends;
    std::queue<Message> inbox;
    std::list<Message> history;

public:
    User() = default;                     
    User(std::string uname, std::string pass);

    std::string getUsername() const;
    bool checkPassword(const std::string& pass) const;

    void addFriend(const std::string& friendName);
    bool isFriend(const std::string& friendName) const;

    void receiveMessage(const Message& msg);
    void showInbox() const;

    int getInboxSize() const;
    void clearInbox();

    std::string getPassword() const;

    bool changePassword(const std::string oldPassword ,const std::string newPassword);

    bool changeUsername(const std::string& pass, const std::string& newUsername);

    const std::list<Message>& getHistory() const;
};

#endif