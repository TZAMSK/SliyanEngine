#include "console/Message.hpp"

#include <string>

Message::Message(const std::string &message) : message(message)
{
}

const std::string &Message::getMessage() const
{
    return message;
}

void Message::addMessage(std::string new_message)
{
    message += new_message;
}

void Message::clearMessages()
{
    message = "";
}
