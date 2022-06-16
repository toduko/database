#include "Database.h"

#include <iostream>
#include <fstream>

Database::Database(const String &databaseFilePath) : name(databaseFilePath)
{
  std::ifstream file(databaseFilePath, std::ios::app);

  if (!file.is_open())
  {
    return;
  }

  file.seekg(0);
  size_t lineCount = 1;
  while (!file.eof())
  {
    String tableName;
    file >> tableName;

    this->importTable(tableName);
  }

  ++lineCount;
}

int Database::findTable(const String &tableName) const
{
  for (size_t i = 0; i < this->tables.getSize(); ++i)
  {
    if (this->tables[i].getName() == tableName)
    {
      return i;
    }
  }

  return -1;
}

const String &Database::getName() const
{
  return this->name;
}

void Database::importTable(const String &tableName)
{
  if (this->findTable(tableName) >= 0)
  {
    std::cerr << "Table \"" << tableName << "\" already exists in database\n";
  }
  if (tableName.isOnlyLetters() && !tableName.isEmpty())
  {
    Optional<Table> table(std::move(Table::createTable(tableName)));

    if (!table.isNull())
    {
      this->tables.push(table.getData());
    }
    else
    {
      std::cerr << "Error creating table \"" << tableName << "\"\n";
    }
  }
  else
  {
    std::cerr << "\"" << tableName << "\" is not a valid table name and it will not be read\n";
  }
}

void Database::writeTo(std::ofstream &file) const
{
  file.seekp(0);

  for (size_t i = 0; i < this->tables.getSize(); ++i)
  {
    file << this->tables[i].getName();
    if (i < this->tables.getSize() - 1)
    {
      file << std::endl;
    }

    this->tables[i].write();
  }
}

void Database::printTableNames() const
{
  if (!this->tables.getSize())
  {
    std::cout << "No tables\n";
    return;
  }

  std::cout << "Current tables:\n";
  for (size_t i = 0; i < this->tables.getSize(); ++i)
  {
    std::cout << this->tables[i].getName() << " ";
  }
  std::cout << "\n";
}