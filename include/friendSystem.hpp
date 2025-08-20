#ifndef FRIENDSYSTEM_HPP
#define FRIENDSYSTEM_HPP

#include<string>
#include"userManager.hpp"
#include"user.hpp"
#include<unordered_map>
#include<unordered_set>

class FriendSystem
{
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> friendsMap;


    std::unordered_map<std::string, std::unordered_set<std::string>> pendingRequests;

public:

    bool sendFriendRequest(const std::string& fromUser, const std::string& toUser);


    bool acceptFriendRequest(const std::string& user, const std::string& fromUser);


    bool rejectFriendRequest(const std::string& user, const std::string& fromUser);

    bool cancelRequest(const std::string& fromUser, const std::string& toUser);


    std::vector<std::string> getFriendList(const std::string& username) const;

    std::vector<std::string> getPendingRequests(const std::string& username) const;

    bool isFriend(const std::string& user, const std::string& friendName) const;

    void saveFriendsToFile(const std::string& filename);

    void loadFriendsFromFile(const std::string& filename);

    void saveMessagesToFile(const std::string& filename, const UserManager& userManager);
    void loadMessagesFromFile(const std::string& filename, UserManager& userManager);
};


#endif