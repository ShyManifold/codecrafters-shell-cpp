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

    fs::path m_currentWorkingDirectory;

    std::string m_command;
    std::vector<std::string> m_commandArguments;

    // Utility functions
    void m_fetch();
    void m_call();
    bool m_findExecutable(std::string &command, fs::path &result);
    std::string m_getEnvironmentVariable(const char *envVariable);

    // Command response functions
    void m_type();
    void m_echo();
    void m_pwd();
    void m_cd();
};
