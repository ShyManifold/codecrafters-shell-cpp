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

// Unnecessary function due to my lack of knowledge of fs::current_path()
// Left it in here just to keep myself humble
fs::path Application::m_get_current_directory()
{
#ifdef _WIN32

    std::function<fs::path(DWORD &)> getDir = [&getDir](const DWORD &bufferSize) -> fs::path
    {
        TCHAR *buffer = new TCHAR[bufferSize]; // Dynamically allocate memory

        DWORD dwRet = GetCurrentDirectory(bufferSize, buffer);

        if (dwRet == 0)
        {
            printf("GetCurrentDirectory failed (%d)\n", GetLastError());
            delete[] buffer; // Free dynamically allocated memory
            return "";
        }
        else if (dwRet > bufferSize)
        {
            delete[] buffer;      // Free dynamically allocated memory
            return getDir(dwRet); // Call recursively with a buffer the size of the path including \0
        }
        else
        {
            tstring str(buffer);
            delete[] buffer;
            return fs::path(str);
        }
    };

    DWORD bufferSize = MAX_PATH;

    return getDir(bufferSize);

#else
    char *buffer[MAX_PATH];
    if (getcwd(buffer, size) != NULL)
    {
        std::string str(buffer);
        return fs::path(str);
    };
#endif
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

    if (fs::is_directory(newPath) && fs::exists(newPath))
    {
        fs::current_path(newPath);
        m_currentWorkingDirectory = newPath;
        return;
    }
    

    std::cout << "cd: " << newPath.string() << "No such file or directory" << std::endl;
}
