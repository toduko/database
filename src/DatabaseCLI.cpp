#include "DatabaseCLI.h"

Optional<Database> DatabaseCLI::database;
bool DatabaseCLI::hasChanges = false;

void DatabaseCLI::writeTo(std::ofstream &file)
{
  DatabaseCLI::database.getData().writeTo(file);
}

DatabaseCLI::DatabaseCLI() : CLI("database")
{
  this->addCommand(std::move(Command("open", "opens specified database file. Usage: open <file>", &DatabaseCLI::read)));
  this->addCommand(std::move(Command("close", "closes currently opened database", &DatabaseCLI::close)));
  this->addCommand(std::move(Command("save", "saves the currently open database", &DatabaseCLI::save)));
  this->addCommand(std::move(Command("print", "prints all rows in table. Usage: print <table name>", &DatabaseCLI::print)));
  this->addCommand(std::move(Command("saveas", "saves the currently open database to specified file. Usage: saveas <file>", &DatabaseCLI::saveAs)));
  this->addCommand(std::move(Command("rename", "renames a table. Usage: rename <old name> <new name>", &DatabaseCLI::rename)));
  this->addCommand(std::move(Command("import", "imports a table to the database from file. Usage: import <table name>", &DatabaseCLI::import)));
  this->addCommand(std::move(Command("export", "saves a table to a table file. Usage: export <table name> <file>", &DatabaseCLI::exportTable)));
  this->addCommand(std::move(Command("showtables", "prints all of the tables in the currently opened database", &DatabaseCLI::showTables)));
}

void DatabaseCLI::read(const Vector<String> &args)
{
  if (args.getSize() == 0)
  {
    throw "Must specify database";
  }

  if (!DatabaseCLI::database.isNull())
  {
    throw "Another database is already open";
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

  DatabaseCLI::hasChanges = false;
  std::cout << "Successfully closed " << DatabaseCLI::database.getData().getName() << std::endl;
  DatabaseCLI::database.clear();
}

void DatabaseCLI::stop()
{
  if (DatabaseCLI::hasChanges)
  {
    throw "You have an open database with unsaved changes, please select close or save first";
  }
  this->shouldContinue = false;
}

void DatabaseCLI::print(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  if (args.getSize() == 0)
  {
    throw "Must specify table name";
  }

  DatabaseCLI::database.getData().printTable(args[0]);
}

void DatabaseCLI::save(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  std::ofstream file(DatabaseCLI::database.getData().getName(), std::ios::binary);
  if (!file.is_open())
  {
    throw "Could not open file";
  }

  DatabaseCLI::writeTo(file);
  DatabaseCLI::hasChanges = false;
  std::cout << "Successfully written to " << DatabaseCLI::database.getData().getName() << std::endl;
}

void DatabaseCLI::saveAs(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
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
  DatabaseCLI::hasChanges = false;
  std::cout << "Successfully written to " << args[0] << std::endl;
}

void DatabaseCLI::rename(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  if (args.getSize() < 2)
  {
    throw "Must specify both old table name and new table name";
  }

  DatabaseCLI::database.getData().renameTable(args[0], args[1]);
  DatabaseCLI::hasChanges = true;
  std::cout << "Successfully renamed table \"" << args[0] << "\" to \"" << args[1] << "\"\n";
}

void DatabaseCLI::import(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  if (args.getSize() == 0)
  {
    throw "Must specify table name";
  }

  DatabaseCLI::database.getData().importTable(args[0]);
  DatabaseCLI::hasChanges = true;
  std::cout << "Successfully opened table \"" << args[0] << "\"\n";
}

void DatabaseCLI::exportTable(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  if (args.getSize() < 2)
  {
    throw "Must specify both table name and file name";
  }

  DatabaseCLI::database.getData().exportTable(args[0], args[1]);
  std::cout << "Successfully saved table \"" << args[0] << "\" to \"" << args[1] << "\"\n";
}

void DatabaseCLI::showTables(const Vector<String> &args)
{
  if (DatabaseCLI::database.isNull())
  {
    throw "No database is open";
  }

  DatabaseCLI::database.getData().printTableNames();
}

DatabaseCLI &DatabaseCLI::getInstance()
{
  static DatabaseCLI cli;
  return cli;
}