#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void displayMainMenu() {
    cout << "\n===== Chat Menu =====\n";
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


string sendCommand(int sock, const string& cmd) {
    send(sock, cmd.c_str(), cmd.size(), 0);
    char buffer[2048] = {0};
    int bytes = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes > 0) return string(buffer, bytes);
    return "No response from server";
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { cerr << "Socket creation failed\n"; return 1; }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed\n"; return 1;
    }

    cout << "Connected to server!\n";

    string currentUser;
    bool running = true;

    while (running) {
        if (currentUser.empty()) {
            displayMainMenu();
            int choice; cin >> choice; cin.ignore();
            string cmd;

            if (choice == 1) {
                string u, p;
                cout << "Enter username: "; getline(cin, u);
                cout << "Enter password: "; getline(cin, p);
                cmd = "REGISTER|" + u + "|" + p;
                cout << "[Server]: " << sendCommand(sock, cmd) << endl;
            }
            else if (choice == 2) {
                string u, p;
                cout << "Enter username: "; getline(cin, u);
                cout << "Enter password: "; getline(cin, p);
                cmd = "LOGIN|" + u + "|" + p;
                string res = sendCommand(sock, cmd);
                cout << "[Server]: " << res << endl;
                if (res.find("SUCCESS") != string::npos) currentUser = u;
            }
            else if (choice == 3) {
                running = false;
                cout << "Exiting...\n";
            }
            else cout << "Invalid option.\n";
        }
        else {
            displayUserMenu(currentUser);
            int choice; cin >> choice; cin.ignore();
            string cmd;

            switch (choice) {
                case 1: { 
                     std::string cmd = "SHOW_INBOX|" + currentUser;
                     std::string res = sendCommand(sock, cmd);
                     std::cout << res << std::endl;
                }
                case 2: {
                    string to, msg;
                    cout << "Send to: "; getline(cin, to);
                    cout << "Message: "; getline(cin, msg);
                    cmd = "SEND_MSG|" + currentUser + "|" + to + "|" + msg;
                    break;
                }
                case 3: cmd = "VIEW_FRIENDS|" + currentUser; break;
                case 4: {
                    string to; 
                    cout << "Enter username to send friend request: "; getline(cin, to);
                    cmd = "FRIEND_REQUEST|" + currentUser + "|" + to;
                    break;
                }
                case 5: cmd = "VIEW_PENDING|" + currentUser; break;
                case 6: {
                    string from; 
                    cout << "Enter username to accept request from: "; getline(cin, from);
                    cmd = "FRIEND_REQUEST_ACCEPT|" + currentUser + "|" + from;
                    break;
                }
                case 7: {
                    string from; 
                    cout << "Enter username to reject request from: "; getline(cin, from);
                    cmd = "FRIEND_REQUEST_REJECT|" + currentUser + "|" + from;
                    break;
                }
                case 8: {
                    string oldP, newP;
                    cout << "Enter old password: "; getline(cin, oldP);
                    cout << "Enter new password: "; getline(cin, newP);
                    cmd = "CHANGE_PASSWORD|" + currentUser + "|" + oldP + "|" + newP;
                    break;
                }
                case 9: {
                    string newU; 
                    cout << "Enter new username: "; getline(cin, newU);
                    cmd = "CHANGE_USERNAME|" + currentUser + "|" + currentUser + "|" + newU;
                    currentUser = newU;
                    break;
                }
                case 10: {
                    string pass;
                    cout << "Enter password to delete account: "; getline(cin, pass);
                    cmd = "DELETE_USER|" + currentUser + "|" + pass;
                    currentUser.clear();
                    break;
                }
                case 11: {
                    cmd = "LOGOUT|" + currentUser;
                    currentUser.clear();
                    break;
                }
                default: cout << "Invalid option.\n"; continue;
            }

            string res = sendCommand(sock, cmd);
            cout << "[Server]: " << res << endl;
        }
    }

    close(sock);
    return 0;
}