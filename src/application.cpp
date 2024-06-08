#include "application.h"

Application::Application::Application(std::string path)
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Parsing the paths

    std::stringstream ss(path);

    std::string p;

    while (std::getline(ss, p, ':'))
        m_paths.push_back(p);

    // Adding all supported commands

    m_supportedCommands["exit"] = [this]()
    { m_finished = true; };

    m_supportedCommands["echo"] = [this]()
    { m_echo(); };

    m_supportedCommands["type"] = [this]()
    { m_type(); };
}

void Application::m_run()
{
    while (!m_finished)
    {
        m_fetch();
        m_call();
    }
}

void Application::m_fetch()
{
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    std::stringstream ss;
    ss << input;

    std::getline(ss, m_command, ' ');
    std::string argument;

    while (std::getline(ss, argument, ' '))
    {
        m_commandArguments.push_back(argument);
    }
}

void Application::m_call()
{
    if (m_supportedCommands.find(m_command) != m_supportedCommands.end())
    {
        m_supportedCommands[m_command]();
    }
    else
        std::cout << m_command << ": command not found" << std::endl;

    m_command = "";
    m_commandArguments.clear();
}

void Application::m_type()
{
    bool found = false;

    for (auto it = m_supportedCommands.begin(); it != m_supportedCommands.end(); ++it)
    {
        if (it->first == m_commandArguments[0])
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        std::cout << m_commandArguments[0] << " is a shell builtin" << std::endl;
        return;
    }

    std::filesystem::path filepath;

    for (size_t i = 0; i < m_paths.size(); i++)
    {
        filepath = m_paths[i];
        filepath += m_commandArguments[0];
        if (std::filesystem::exists(filepath))
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        std::cout << m_commandArguments[0] << " is " << filepath << std::endl;
        return;
    }

    std::cout << m_commandArguments[0] << " not found" << std::endl;
}

void Application::m_echo()
{
    std::string response = "";

    size_t amountOfArguments = m_commandArguments.size();

    if (m_commandArguments.size() > 0)
    {
        if (m_commandArguments.size() == 1)
        {
            std::cout << m_commandArguments[0] << std::endl;
        }
        else
        {
            response += m_commandArguments[0];
            for (size_t i = 1; i < amountOfArguments; i++)
            {
                response += " " + m_commandArguments[i];
            }
            std::cout << response << std::endl;
        }
    }
}