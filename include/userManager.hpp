#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <string>
#include <unordered_map>
#include<map>
#include<chrono>

#include "user.hpp"

class UserManager {
private:
    std::unordered_map<std::string, User> users;
    
    std::map<std::string, int> loginAttempts;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> lockoutTime;

public:

    bool registerUser(const std::string& username, const std::string& password);

    User* login(const std::string& username, const std::string& password);

    bool userExists(const std::string& username) const;

    void saveUsersToFile(const std::string& filename);

    void loadUsersFromFile(const std::string& filename);
 
    bool changeUserPassword(const std::string& username, const std::string& oldPass, const std::string& newPass);

    bool deleteUser(const std::string& username,const std::string& password);

    bool changeUserUsername(const std::string& username, const std::string& password, const std::string& newUsername);

    User* getUser(const std::string& username);

    const std::unordered_map<std::string, User>& getAllUsers() const;

};

#endif