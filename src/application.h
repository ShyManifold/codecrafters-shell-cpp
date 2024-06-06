#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <functional>
#include <vector>

class Application
{
public:
    Application();
    ~Application(){};
    void run();
    void fetch();
    void call();

private:
    bool m_finished = false;
    std::unordered_map<std::string, std::function<void()>> m_supportedCommands;

    std::string m_command;
    std::vector<std::string> m_commandArguments;

    // Command response functions
    void echo();
};
