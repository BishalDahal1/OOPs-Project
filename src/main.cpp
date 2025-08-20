#include <iostream>
#include "userManager.hpp"
#include "friendSystem.hpp"
#include "chatManager.hpp"

using namespace std;

void displayMainMenu() {
    cout << "\n===== Welcome to Chat App =====\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
}

void displayUserMenu(const string& username) {
    cout << "\n==== Hello, " << username << " ====\n";
    cout << "1. View Inbox\n";
    cout << "2. Send Message\n";
    cout << "3. View Friends\n";
    cout << "4. Send Friend Request\n";
    cout << "5. View Pending Requests\n";
    cout << "6. Accept Friend Request\n";
    cout << "7. Reject Friend Request\n";
    cout << "8. Change Password\n";
    cout << "9. Change Username\n";
    cout << "10. Delete Account\n";
    cout << "11. Logout\n";
    cout << "Choice: ";
}

int main() {
    UserManager userManager;
    FriendSystem friendSystem;
    ChatManager chatManager(&userManager, &friendSystem);

    userManager.loadUsersFromFile("userdata.txt");
    friendSystem.loadFriendsFromFile("friends.txt");
    friendSystem.loadMessagesFromFile("messages.txt", userManager);

    while (true) {
        displayMainMenu();
        int mainChoice;
        cin >> mainChoice;
        cin.ignore();

        if (mainChoice == 1) {
            string uname, pass;
            cout << "Enter username: "; getline(cin, uname);
            cout << "Enter password: "; getline(cin, pass);
            if (userManager.registerUser(uname, pass)) {
                cout << "Registration successful.\n";
            } else {
                cout << "User already exists.\n";
            }
        } else if (mainChoice == 2) {
            string uname, pass;
            cout << "Enter username: "; getline(cin, uname);
            cout << "Enter password: "; getline(cin, pass);
            User* loggedInUser = userManager.login(uname, pass);
            if (!loggedInUser) {
                cout << "Login failed.\n";
                continue;
            }

            while (true) {
                displayUserMenu(loggedInUser->getUsername());
                int choice;
                cin >> choice;
                cin.ignore();

                if (choice == 1) {
                    loggedInUser->showInbox();
                } else if (choice == 2) {
                    string to, msg;
                    cout << "Send to: "; getline(cin, to);
                    cout << "Message: "; getline(cin, msg);
                    chatManager.sendMessage(loggedInUser->getUsername(), to, msg);
                } else if (choice == 3) {
                    vector<string> friends = friendSystem.getFriendList(loggedInUser->getUsername());
                    if (friends.empty()) {
                        cout << "No friends found.\n";
                    } else {
                        cout << "\nFriends:\n";
                        for (const auto& f : friends) cout << "- " << f << "\n";
                    }
                } else if (choice == 4) {
                    string to;
                    cout << "Enter username to send friend request: ";
                    getline(cin, to);
                    friendSystem.sendFriendRequest(loggedInUser->getUsername(), to);
                } else if (choice == 5) {
                    auto pending = friendSystem.getPendingRequests(loggedInUser->getUsername());
                    cout << "\nPending Friend Requests:\n";
                    for (const auto& p : pending) cout << "- " << p << "\n";
                } else if (choice == 6) {
                    string from;
                    cout << "Enter username to accept request from: ";
                    getline(cin, from);
                    friendSystem.acceptFriendRequest(loggedInUser->getUsername(), from);
                } else if (choice == 7) {
                    string from;
                    cout << "Enter username to reject request from: ";
                    getline(cin, from);
                    friendSystem.rejectFriendRequest(loggedInUser->getUsername(), from);
                } else if (choice == 8) {
                    string oldPass, newPass;
                    cout << "Old password: "; getline(cin, oldPass);
                    cout << "New password: "; getline(cin, newPass);
                    userManager.changeUserPassword(loggedInUser->getUsername(), oldPass, newPass);
                } else if (choice == 9) {
                    string oldPass, newName;
                    cout << "Password: "; getline(cin, oldPass);
                    cout << "New username: "; getline(cin, newName);
                    userManager.changeUserUsername(loggedInUser->getUsername(), oldPass, newName);
                } else if (choice == 10) {
                    string pass;
                    cout << "Confirm password to delete account: "; getline(cin, pass);
                    if (userManager.deleteUser(loggedInUser->getUsername(), pass)) {
                        break;
                    }
                } else if (choice == 11) {
                    cout << "Logged out.\n";
                    break;
                } else {
                    cout << "Invalid option.\n";
                }
            }

        } else if (mainChoice == 3) {
            userManager.saveUsersToFile("userdata.txt");
            friendSystem.saveFriendsToFile("friends.txt");
            friendSystem.saveMessagesToFile("messages.txt", userManager);
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}