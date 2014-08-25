#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "GlobalData.h"
#include "ConfigurationReader.h"
#include "FileNameHandler.h"

namespace po = boost::program_options;
namespace sy = boost::system;

void setupAndParseArguments(int argc, char* argv[]);
void printUsage();
void printVersion();

std::string configPath;
std::vector<std::string> paths;

po::options_description generic("Generic options");
po::options_description optional("Optional options");
po::options_description hidden("Hidden options");

po::options_description visible("Allowed options");

po::variables_map vm;

int main(int argc, char* argv[])
{
    setupAndParseArguments(argc, argv);

    if (vm.count("version"))
    {
        printVersion();
        return 0;
    }

    if (vm.count("help"))
    {
        printUsage();
        return 1;
    }

    if (vm.count("verbose"))
        VERBOSE = true;

    if (vm.count("config"))
        std::cout << "Using configuration " << configPath << std::endl;

    if (vm.count("test"))
    {
        std::cout << "Test argument supplied. Will not move files." << std::endl;
        std::cout << "Hmpf. You're the boss." << std::endl;
        TEST = true;
    }

    if (paths.empty())
    {
        std::cout << "No path to a file or directory given." << std::endl;
        std::cout << "What should I process if you don't give me anything?" << std::endl;
        return 1;
    }

    ConfigurationReader reader(configPath);

    switch (reader.state)
    {
    case ConfigurationState::FileNotReadable:
    {
        std::cout << "Couldn't read configuration file " + configPath + "." << std::endl;
        std::cout << "Don't tell me you borked up the permissions again." << std::endl;
        return 1;
    }
    case ConfigurationState::FileNotActuallyAFile:
    {
        std::cout << "Configuration file " + configPath + " is not actually a file." << std::endl;
        std::cout << "Why are you trying to trick me?" << std::endl;
        return 1;
    }
    }

    FileNameHandler handler(&paths, reader.config);

    switch (handler.state)
    {
    case RegexState::NoFiles:
    {
        std::cout << "Couldn't find any input files." << std::endl;
        std::cout << "No input == no output. Simple as that." << std::endl;
        return 1;
    }
    case RegexState::Errors:
    {
        std::cout << "Couldn't parse configured regexes." << std::endl;
        std::cout << "Master hasn't granted me to show what went wrong yet." << std::endl;
        return 1;
    }
    case RegexState::NoRegex:
    {
        std::cout << "No regexes defined." << std::endl;
        std::cout << "This is the main feature. How in the world did you forget it?" << std::endl;
    }
    }

    handler.process();
    std::vector<FileRename>::iterator i = handler.fileRenames.begin();

    do
    {
        if (!fs::exists(i->destDirectory))
        {
            std::cout << "Destination directory does not exist. Creating... " << std::endl;
            fs::create_directories(i->destDirectory);
        }

        fs::path newFile = fs::path(i->destDirectory) / i->destFileName;
        std::cout << "Moving file " << i->srcFile << " to " << newFile << std::endl;

        if (TEST)
        {
            std::cout << "Just kidding. I'm not moving anything in this mode." << std::endl;
            continue;
        }

        fs::rename(i->srcFile, newFile);
    } while (++i != handler.fileRenames.end());
}

void setupAndParseArguments(int argc, char* argv[])
{
    generic.add_options()
        ("version", "Print current version")
        ("help,h", "Print this");

    optional.add_options()
        ("test,t", "Do not physically move files")
        ("verbose,v", "Provide verbose output")
        ("config,c", po::value<std::string>(&configPath)->default_value("config.json"), "Specify configuration file");

    hidden.add_options()
        ("input,i", po::value<std::vector<std::string>>(&paths), "Path to files or directories");

    po::options_description cmdline;
    cmdline.add(generic).add(optional).add(hidden);
    visible.add(generic).add(optional);

    po::positional_options_description p;
    p.add("input", -1);
    po::store(po::command_line_parser(argc, argv).options(cmdline).positional(p).run(), vm);
    po::notify(vm);
}

void printUsage()
{
    std::cout << visible << std::endl;
}

void printVersion()
{
    std::cout << "KongouFileMover v1.0.0.0" << std::endl;
    std::cout << "   @BinaryTENSHi 2014   " << std::endl << std::endl;
    std::cout << "Thanks for using KFM. <3" << std::endl;
}