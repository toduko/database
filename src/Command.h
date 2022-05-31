#ifndef COMMAND_H_
#define COMMAND_H_

#include "Vector.h"
#include "String.h"

class Command
{
private:
  String name;
  String description;
  void (*function)(const Vector<String>& args);

public:
  Command(const String &name, const String &description, void (*function)(const Vector<String>& args));

  const String &getName() const;
  const String &getDescription() const;
  void execute(const Vector<String>& args) const;
};

#endif