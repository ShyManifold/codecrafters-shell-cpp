#include "application.h"

Application::Application::Application()
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Adding all supported commands
    
    m_supportedCommands["exit"] = [this]()
    { m_finished = true; };

    m_supportedCommands["echo"] = [this]()
    { echo(); };
}

void Application::run()
{
    while (!m_finished)
    {
        fetch();
        call();
    }
}

void Application::fetch()
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

void Application::call()
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

void Application::echo()
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