#include"friendSystem.hpp"
#include<iostream>
#include <fstream>
#include <sstream>

bool FriendSystem::sendFriendRequest(const std::string& fromUser, const std::string& toUser)
{
 if(fromUser==toUser)
 {
    std::cout<<"You can't send request to yourself. \n";
    return false;
 }
 if(friendsMap[fromUser].count(toUser))
 {
    std::cout<<fromUser <<" and " << toUser <<" are already friends with eachother\n";
    return false;
 }
 if(pendingRequests[toUser].count(fromUser))
 {
    std::cout<<"Friend request is already sent to " << toUser << ".\n";
    return false;
 }

 pendingRequests[toUser].insert(fromUser);
 std::cout<<fromUser <<" sent a request to " << toUser <<".\n";
 return true;

 
}

bool FriendSystem::acceptFriendRequest(const std::string& user, const std::string& fromUser) {
    
    if (pendingRequests[user].count(fromUser) == 0) {
        std::cout << "No pending friend request from " << fromUser << ".\n";
        return false;
    }

    friendsMap[user].insert(fromUser);
    friendsMap[fromUser].insert(user);

   
    pendingRequests[user].erase(fromUser);

    std::cout << user << " and " << fromUser << " are now friends!\n";
    return true;
}

bool FriendSystem::rejectFriendRequest(const std::string& user, const std::string& fromUser) {
    
    if (pendingRequests[user].count(fromUser) == 0) {
        std::cout << "No pending friend request from " << fromUser << ".\n";
        return false;
    }

    pendingRequests[user].erase(fromUser);

    std::cout << "Friend request from " << fromUser << " has been rejected by " << user << ".\n";
    return true;
}

bool FriendSystem::cancelRequest(const std::string& fromUser, const std::string& toUser) {
    
  if (pendingRequests[toUser].count(fromUser) == 0) {
        std::cout << "No pending friend request from " << fromUser << " to " << toUser << ".\n";
        return false;
    }

    pendingRequests[toUser].erase(fromUser);
    std::cout << fromUser << " has canceled the friend request to " << toUser << ".\n";
    return true;
}

 std::vector<std::string> FriendSystem::getPendingRequests(const std::string& username) const
 {
   std::vector<std::string> result;

   auto it=pendingRequests.find(username);
   if(it!=pendingRequests.end()){
      for(const auto& sender: it->second){
         result.push_back(sender);
      }
   }
   return result;
 }

 std::vector<std::string> FriendSystem::getFriendList(const std::string& username) const
 {
   std::vector<std::string> result;

   auto it=friendsMap.find(username);
   if(it!=friendsMap.end()){
      for(const auto& sender: it->second){
         result.push_back(sender);
      }
   }
   return result;
 }

  bool FriendSystem::isFriend(const std::string& user, const std::string& friendName) const {
    auto it = friendsMap.find(user);
    if (it != friendsMap.end()) {
        return it->second.count(friendName) > 0;
    }
    return false;
}

void FriendSystem::saveFriendsToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open friends file for saving.\n";
        return;
    }

    for (const auto& pair : friendsMap) {
        file << pair.first << ":";
        bool first = true;
        for (const auto& friendName : pair.second) {
            if (!first) file << ",";
            file << friendName;
            first = false;
        }
        file << "\n";
    }
    file.close();
    std::cout << "Friends saved to file.\n";
}

void FriendSystem::loadFriendsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No friends file found, starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username;
        if (std::getline(ss, username, ':')) {
            std::string friendsStr;
            if (std::getline(ss, friendsStr)) {
                std::stringstream friendsStream(friendsStr);
                std::string friendName;
                while (std::getline(friendsStream, friendName, ',')) {
                    friendsMap[username].insert(friendName);
                }
            }
        }
    }
    file.close();
    std::cout << "Friends loaded from file.\n";
}

void FriendSystem::saveMessagesToFile(const std::string& filename, const UserManager& userManager) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open messages file for saving.\n";
        return;
    }

    for (const auto& pair : userManager.getAllUsers()) { 
        const User& user = pair.second;
        const std::list<Message>& history = user.getHistory();

        for (const Message& msg : history) {
            file << user.getUsername() << "|" << msg.sender << "|" << msg.timestamp << "|" << msg.content << "\n";
        }
    }

    file.close();
    std::cout << "Messages saved to file.\n";
}

void FriendSystem::loadMessagesFromFile(const std::string& filename, UserManager& userManager) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No messages file found, starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string receiver, sender, timestamp, content;

        if (std::getline(ss, receiver, '|') &&
            std::getline(ss, sender, '|') &&
            std::getline(ss, timestamp, '|') &&
            std::getline(ss, content)) {

            Message msg(sender, content);
            msg.timestamp = timestamp;   
            

            User* user = userManager.getUser(receiver);
            if (user) {
                user->receiveMessage(msg);
            }
        }
    }

    file.close();
    std::cout << "Messages loaded from file.\n";
   }
