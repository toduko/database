#ifndef COMMAND_H_
#define COMMAND_H_

#include "String.h"

class Command
{
private:
  String name;
  String description;
  void (*function)();

public:
  Command(const String &name, const String &description, void (*function)());

  const String &getName() const;
  const String &getDescription() const;
  void execute() const;
};

#endif