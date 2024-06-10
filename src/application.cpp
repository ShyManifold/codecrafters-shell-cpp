#include "application.h"

Application::Application::Application(std::string path)
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Initializing the current working directory
    m_currentWorkingDirectory = fs::current_path();
    // Parsing the paths

    std::stringstream ss(path);

    std::string p;
#ifdef _WIN32
    while (std::getline(ss, p, ';'))
    {
        m_paths.push_back(p);
    }
#else
    while (std::getline(ss, p, ':'))
    {
        m_paths.push_back(p);
    }

#endif
    // Adding all supported commands

    m_supportedCommands["exit"] = [this]()
    { m_finished = true; };

    m_supportedCommands["echo"] = [this]()
    { m_echo(); };

    m_supportedCommands["type"] = [this]()
    { m_type(); };

    m_supportedCommands["pwd"] = [this]()
    { m_pwd(); };

    m_supportedCommands["cd"] = [this]()
    { m_cd(); };
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
    fs::path pathToExcecutable;

    if (m_supportedCommands.find(m_command) != m_supportedCommands.end())
    {
        m_supportedCommands[m_command]();
    }
    else if (m_findExecutable(m_command, pathToExcecutable))
    {
        std::string ArgV = pathToExcecutable.string();

        for (size_t i = 0; i < m_commandArguments.size(); i++)
        {
            ArgV += " " + m_commandArguments[i];
        }

        system(ArgV.c_str());
    }
    else
        std::cout << m_command << ": command not found" << std::endl;

    m_command = "";
    m_commandArguments.clear();
}

bool Application::m_findExecutable(std::string &command, fs::path &result)
{
    for (const auto &path : m_paths)
    {
        if (fs::exists(path) && fs::is_directory(path))
        {
            try
            {
                for (const auto &entry : fs::directory_iterator(path))
                {
                    if (fs::is_regular_file(entry.path()))
                        // The stem is the filename without the extension
                        if (entry.path().stem().string() == command)
                        {
                            result = entry.path();
                            return true;
                        }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    return false;
}

std::string Application::m_getEnvironmentVariable(const char *envVariable)
{
    char *value;
    size_t len;

    // Determine the required buffer size
    if (_dupenv_s(&value, &len, envVariable) != 0)
    {
        printf("Failed to retrieve environment variable\n");
        return {};
    }

    char *buffer = (char *)malloc(len * sizeof(char));
    if (buffer == NULL)
    {
        printf("Failed to allocate memory\n");
        return {};
    }

    if (_dupenv_s(&buffer, &len, envVariable) != 0)
    {
        // Handle error
        printf("Failed to retrieve environment variable\n");
        free(buffer);
        return {};
    }

    std::string result = buffer;
    free(buffer);
    return result;
}

void Application::m_type()
{
    bool found = false;

    std::string command = m_commandArguments[0];

    for (auto it = m_supportedCommands.begin(); it != m_supportedCommands.end(); ++it)
    {
        if (it->first == command)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        std::cout << command << " is a shell builtin" << std::endl;
        return;
    }
    fs::path pathToCommand;
    found = m_findExecutable(command, pathToCommand);

    if (found)
    {
        std::cout << command << " is " << pathToCommand.string() << std::endl;
        return;
    }

    std::cout << command << " not found" << std::endl;
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

void Application::m_pwd()
{
    std::cout << m_currentWorkingDirectory.string() << std::endl;
}

void Application::m_cd()
{
    fs::path newPath{};

    if (m_commandArguments.size() > 0)
    {
        newPath = m_commandArguments[0];
    };
    if (newPath == "~")
    {

#ifdef _WIN32
        fs::path newPath = m_getEnvironmentVariable("USERPROFILE");
#else
        fs::path newPath = m_getEnvironmentVariable("HOME");
#endif
        fs::current_path(newPath);
        m_currentWorkingDirectory = newPath;
        return;
    }
    else if (fs::is_directory(newPath) && fs::exists(newPath))
    {
        if (newPath.is_absolute())
        {

            fs::current_path(newPath);
            m_currentWorkingDirectory = newPath;
        }
        else
        {
            newPath = fs::canonical(newPath);
            fs::current_path(newPath);
            m_currentWorkingDirectory = newPath;
        }

        return;
    }

    std::cout << newPath.string() << ": No such file or directory" << std::endl;
}
