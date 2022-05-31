#ifndef CLI_H_
#define CLI_H_

#include "Vector.h"
#include "Command.h"

class CLI
{
private:
  Vector<Command> commands;
  String name;
  bool shouldContinue;

  void printCommands() const;

  void parse(const String &command);

protected:
  virtual void printSpecialCommands() const;
  virtual void handleSpecialCommand(const String &command, const Vector<String> &args);
  virtual bool isSpecialCommand(const String &command) const;
  virtual void stop();

public:
  CLI(const String &name);
  CLI(const String &name, const Vector<Command> &commands);

  void addCommand(const Command &command);
  void addCommand(Command &&command);

  void run();
};

#endif