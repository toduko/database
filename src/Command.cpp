#include "Command.h"

Command::Command(const String &name, const String &description, void (*function)(const Vector<String>& args))
    : name(name), description(description), function(function) {}

const String &Command::getName() const
{
  return this->name;
}

const String &Command::getDescription() const
{
  return this->description;
}

void Command::execute(const Vector<String>& args) const
{
  this->function(args);
}