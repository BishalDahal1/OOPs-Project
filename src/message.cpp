// message.cpp
#include "message.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>


Message::Message(string s, string c) {
    sender = s;
    content = c;
    rawTime = std::time(nullptr);  

    std::tm* now_tm = std::localtime(&rawTime);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    timestamp = oss.str();
}

