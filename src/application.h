#pragma once

#include "stdafx.h"

class Application
{
public:
    Application(std::string path = ".");
    ~Application(){};
    void m_run();

private:
    bool m_finished = false;
    std::vector<std::filesystem::path> m_paths;
    std::unordered_map<std::string, std::function<void()>> m_supportedCommands;

    std::string m_command;
    std::vector<std::string> m_commandArguments;
    
    //Utility functions
    void m_fetch();
    void m_call();

    // Command response functions
    void m_type();
    void m_echo();
};
