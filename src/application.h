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
    void m_run();
private:
    bool m_finished = false;
    std::unordered_map<std::string, std::function<void()>> m_supportedCommands;

    std::string m_command;
    std::vector<std::string> m_commandArguments;

    void m_fetch();
    void m_call();
    
    // Command response functions
    void m_type();
    void m_echo();
};
