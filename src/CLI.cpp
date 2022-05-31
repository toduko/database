#include "CLI.h"

#include <iostream>

CLI::CLI(const String &name) : name(name) {}

CLI::CLI(const String &name, const Vector<Command> &commands)
    : name(name), commands(commands), shouldContinue(true) {}

void CLI::printCommands() const
{
  std::cout << "Available commands:\n";
  std::cout << "help - prints this menu\n";
  for (size_t i = 0; i < this->commands.getSize(); ++i)
  {
    std::cout << commands[i].getName() << " - " << commands[i].getDescription() << std::endl;
  }
}

void CLI::parse(const String &command)
{

  Vector<String> args(std::move(command.split(' ')));
  String cmd = args[0];
  args.remove(0);

  if (cmd == String("help"))
  {
    this->printCommands();
  }
  else
  {
    bool foundCommand = false;

    for (size_t i = 0; i < this->commands.getSize(); ++i)
    {
      if (cmd == this->commands[i].getName())
      {
        this->commands[i].execute(args);
        foundCommand = true;
        break;
      }
    }

    if (!foundCommand)
    {
      throw "Command not found. Type help to get list of commands";
    }
  }
}

void CLI::addCommand(const Command &command)
{
  if (command.getName() == String("help"))
  {
    throw "There already exists a command with the same name";
  }
  for (size_t i = 0; i < this->commands.getSize(); ++i)
  {
    if (this->commands[i].getName() == command.getName())
    {
      throw "There already exists a command with the same name";
    }
  }

  this->commands.push(command);
}

void CLI::addCommand(Command &&command)
{
  if (command.getName() == String("help"))
  {
    throw "There already exists a command with the same name";
  }
  for (size_t i = 0; i < this->commands.getSize(); ++i)
  {
    if (this->commands[i].getName() == command.getName())
    {
      throw "There already exists a command with the same name";
    }
  }

  this->commands.push(std::move(command));
}

void CLI::run()
{
  while (this->shouldContinue)
  {
    try
    {
      String cmd;
      std::cout << this->name << " > ";
      std::cin >> cmd;
      if (cmd != String(""))
      {
        this->parse(cmd);
      }
    }
    catch (const char *msg)
    {
      std::cerr << msg << std::endl;
    }
  }
}