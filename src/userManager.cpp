#include "userManager.hpp"
#include <iostream> 
#include <fstream>
#include <sstream>

bool UserManager::registerUser(const std::string& username, const std::string& password) {
    if (userExists(username)) {
        return false;
    }
    users.emplace(username, User(username, password));
    std::cout << "User '" << username << "' registered successfully.\n";
    return true;
}

User* UserManager::login(const std::string& username, const std::string& password) {
    auto now = std::chrono::steady_clock::now();

    if (loginAttempts[username] >= 3) {
        auto lockedAt = lockoutTime[username];
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - lockedAt);

        if (elapsed.count() < 30) {
            std::cout << "Account locked. Try again in " << (30 - elapsed.count()) << " minutes.\n";
            return nullptr;
        } else {
            loginAttempts[username] = 0;
            lockoutTime.erase(username);
            std::cout << "Lock expired. You may try again.\n";
        }
    }

    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "Login failed: user '" << username << "' does not exist.\n";
        return nullptr;
    }

    if (it->second.checkPassword(password)) {
        loginAttempts[username] = 0;
        lockoutTime.erase(username);
        std::cout << "User '" << username << "' logged in successfully.\n";
        return &it->second;
    } else {
        loginAttempts[username]++;
        if (loginAttempts[username] == 3) {
            lockoutTime[username] = now;
            std::cout << "Account locked due to 3 failed attempts. Try again in 30 minutes.\n";
        } else {
            std::cout << "Incorrect password. Attempts left: " << (3 - loginAttempts[username]) << "\n";
        }
        return nullptr;
    }
}

bool UserManager::userExists(const std::string& username) const {
    return users.find(username) != users.end();
}

void UserManager::saveUsersToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file for saving.\n";
        return;
    }

    for (const auto& pair : users) {
        const std::string& username = pair.first;
        const std::string& password = pair.second.getPassword(); 
        file << username << "," << password << "\n";
    }

    file.close();
    std::cout << "Users saved to file: " << filename << "\n";
}

void UserManager::loadUsersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No existing user file found. Starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password;

        if (std::getline(ss, username, ',') && std::getline(ss, password)) {
            users.emplace(username, User(username, password));
        }
    }

    file.close();
    std::cout << "Users loaded from file: " << filename << "\n";
}

bool UserManager::changeUserPassword(const std::string& username, const std::string& oldPass, const std::string& newPass) {
    auto now = std::chrono::steady_clock::now();

   
    if (loginAttempts[username] >= 3) {
        auto lockedAt = lockoutTime[username];
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - lockedAt);
        if (elapsed.count() < 30) {
            std::cout << "Too many wrong attempts. Try password change again in " << (30 - elapsed.count()) << " minutes.\n";
            return false;
        } else {
            loginAttempts[username] = 0;
            lockoutTime.erase(username);
            std::cout << "Lock expired. You can try again.\n";
        }
    }

    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "User not found.\n";
        return false;
    }

    if (it->second.changePassword(oldPass, newPass)) {
        loginAttempts[username] = 0;
        lockoutTime.erase(username);
        std::cout << "Password changed successfully.\n";
        return true;
    } else {
        loginAttempts[username]++;
        if (loginAttempts[username] == 3) {
            lockoutTime[username] = now;
            std::cout << "Too many wrong attempts. Account locked for 30 minutes.\n";
        } else {
            std::cout << "Wrong old password. Attempts left: " << (3 - loginAttempts[username]) << "\n";
        }
        return false;
    }
}

bool UserManager::deleteUser(const std::string& username,const std::string& password)
{
     if(!userExists(username))
     {
         std::cout<<"User " <<username <<" " << "doesn't exist";
         return false;

     }
       auto now = std::chrono::steady_clock::now();

   
    if (loginAttempts[username] >= 3) {
        auto lockedAt = lockoutTime[username];
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - lockedAt);
        if (elapsed.count() < 30) {
            std::cout << "Too many wrong attempts. Try password change again in " << (30 - elapsed.count()) << " minutes.\n";
            return false;
        } else {
            loginAttempts[username] = 0;
            lockoutTime.erase(username);
            std::cout << "Lock expired. You can try again.\n";
        }
    }
     auto it=users.find(username);
     if(it->second.checkPassword(password))
     {
        // for(auto&[name,user] : users)
        // {
        //     if(name!=username){
        //         user.removeFriend(username);

        // }
        users.erase(username);
        loginAttempts.erase(username);
        lockoutTime.erase(username);

        std::cout << "User '" << username << "' has been deleted successfully.\n";
        return true;
    } else {
        loginAttempts[username]++;
        if (loginAttempts[username] == 3) {
            lockoutTime[username] = now;
            std::cout << "Account locked due to 3 failed attempts. Try again in 30 minutes.\n";
        } else {
            std::cout << "Incorrect password. Attempts left: " << (3 - loginAttempts[username]) << "\n";
        }
        return false;
    }
     } 

bool UserManager::changeUserUsername(const std::string& username, const std::string& password, const std::string& newUsername) {
    auto now = std::chrono::steady_clock::now();

    if (loginAttempts[username] >= 3) {
        auto lockedAt = lockoutTime[username];
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - lockedAt);
        if (elapsed.count() < 30) {
            std::cout << "Too many wrong attempts. Try username change again in " << (30 - elapsed.count()) << " minutes.\n";
            return false;
        } else {
            loginAttempts[username] = 0;
            lockoutTime.erase(username);
            std::cout << "Lock expired. You can try again.\n";
        }
    }

    auto it = users.find(username);
    if (it == users.end()) {
        std::cout << "User not found.\n";
        return false;
    }

    if (!it->second.checkPassword(password)) {
        loginAttempts[username]++;
        if (loginAttempts[username] == 3) {
            lockoutTime[username] = now;
            std::cout << "Too many wrong attempts. Account locked for 30 minutes.\n";
        } else {
            std::cout << "Wrong password. Attempts left: " << (3 - loginAttempts[username]) << "\n";
        }
        return false;
    }

    if (userExists(newUsername)) {
        std::cout << "Username '" << newUsername << "' already exists.\n";
        return false;
    }

    
    users.emplace(newUsername, it->second);  
    users.erase(username);

  
    loginAttempts[newUsername] = loginAttempts[username];
    loginAttempts.erase(username);

    if (lockoutTime.count(username)) {
        lockoutTime[newUsername] = lockoutTime[username];
        lockoutTime.erase(username);
    }

    std::cout << "Username changed successfully from '" << username << "' to '" << newUsername << "'.\n";
    return true;
}

User* UserManager::getUser(const std::string& username) {
    if (!userExists(username)) return nullptr;
    return &users[username];
}

const std::unordered_map<std::string, User>& UserManager::getAllUsers() const {
    return users;
}



