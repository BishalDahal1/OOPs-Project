// message.hpp
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

using namespace std;

class Message {
public:
    string sender;
    string content;
    string timestamp;
     std::time_t rawTime;

    Message(string s, string c);
};
  
#endif
