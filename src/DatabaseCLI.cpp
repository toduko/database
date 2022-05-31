#include "DatabaseCLI.h"

Vector<char> DatabaseCLI::bytes;
String DatabaseCLI::filename;
bool DatabaseCLI::isFileOpen = false;

void DatabaseCLI::writeTo(std::ofstream &file)
{
  file.seekp(0);
  for (size_t i = 0; i < DatabaseCLI::bytes.getSize(); ++i)
  {
    file.write(&DatabaseCLI::bytes[i], sizeof(char));
  }
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

  std::ifstream file(args[0], std::ios::binary | std::ios::app);
  if (!file.is_open())
  {
    throw "Could not open file";
  }

  DatabaseCLI::isFileOpen = true;
  file.seekg(0);

  while (1)
  {
    char c;
    file.read(&c, sizeof(c));
    if (file.eof())
    {
      break;
    }
    DatabaseCLI::bytes.push(c);
  }

  DatabaseCLI::filename = args[0];
  std::cout << "Successfully opened " << args[0] << std::endl;
}

void DatabaseCLI::close(const Vector<String> &args)
{
  if (!DatabaseCLI::isFileOpen)
  {
    throw "No file is open";
  }

  DatabaseCLI::isFileOpen = false;
  DatabaseCLI::bytes.clear();
  std::cout << "Successfully closed " << DatabaseCLI::filename << std::endl;
}

void DatabaseCLI::stop()
{
  if (DatabaseCLI::isFileOpen)
  {
    throw "You have an open file with unsaved changes, please select close or save first";
  }
  this->shouldContinue = false;
}

void DatabaseCLI::save(const Vector<String> &args)
{
  if (!DatabaseCLI::isFileOpen)
  {
    throw "No file is open";
  }

  std::ofstream file(DatabaseCLI::filename, std::ios::binary);
  if (!file.is_open())
  {
    throw "Could not open file";
  }

  DatabaseCLI::writeTo(file);
  std::cout << "Successfully written to " << DatabaseCLI::filename << std::endl;
  DatabaseCLI::isFileOpen = false;
  DatabaseCLI::bytes.clear();
}

void DatabaseCLI::saveAs(const Vector<String> &args)
{
  if (!DatabaseCLI::isFileOpen)
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
  DatabaseCLI::isFileOpen = false;
  DatabaseCLI::bytes.clear();
}

DatabaseCLI &DatabaseCLI::getInstance()
{
  static DatabaseCLI cli;
  return cli;
}