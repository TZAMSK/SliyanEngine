#pragma once

#include <string>

class Message
{
  public:
    virtual ~Message() = default;

    Message(const std::string &message);

  public:
    const std::string &getMessage() const;
    void addMessage(std::string new_message);
    void clearMessages();

  private:
    std::string message;
};
