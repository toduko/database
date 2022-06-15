#include "DatabaseCLI.h"

Optional<Database> DatabaseCLI::database;

void DatabaseCLI::writeTo(std::ofstream &file)
{
  DatabaseCLI::database.getData().writeTo(file);
}

DatabaseCLI::DatabaseCLI() : CLI("database")
{
  this->addCommand(std::move(Command("open", "opens specified file. Usage: open <file>", &DatabaseCLI::read)));
  this->addCommand(std::move(Command("close", "closes currently opened file", &DatabaseCLI::close)));
  this->addCommand(std::move(Command("save", "saves the currently open file", &DatabaseCLI::save)));
  this->addCommand(std::move(Command("saveas", "saves the currently open file to specified file. Usage: saveas <file>", &DatabaseCLI::saveAs)));
}

void DatabaseCLI::read(const Vector<String> &args)
{
  if (args.getSize() == 0)
  {
    throw "Must specify filename";
  }

  DatabaseCLI::database = Database(args[0]);
  std::cout << "Successfully opened " << args[0] << std::endl;
}

void DatabaseCLI::close(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No file is open";
  }

  std::cout << "Successfully closed " << DatabaseCLI::database.getData().getName() << std::endl;
}

void DatabaseCLI::stop()
{
  if (!DatabaseCLI::database.isNull())
  {
    throw "You have an open file with unsaved changes, please select close or save first";
  }
  this->shouldContinue = false;
}

void DatabaseCLI::save(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No file is open";
  }

  std::ofstream file(DatabaseCLI::database.getData().getName(), std::ios::binary);
  if (!file.is_open())
  {
    throw "Could not open file";
  }

  DatabaseCLI::writeTo(file);
  std::cout << "Successfully written to " << DatabaseCLI::database.getData().getName() << std::endl;
  DatabaseCLI::database.clear();
}

void DatabaseCLI::saveAs(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No file is open";
  }

  if (args.getSize() == 0)
  {
    throw "Must specify filename";
  }

  std::ofstream file(args[0], std::ios::binary);
  if (!file.is_open())
  {
    throw "Could not open file";
  }

  DatabaseCLI::writeTo(file);
  std::cout << "Successfully written to " << args[0] << std::endl;
  DatabaseCLI::database.clear();
}

DatabaseCLI &DatabaseCLI::getInstance()
{
  static DatabaseCLI cli;
  return cli;
}