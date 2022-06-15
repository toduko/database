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
      std::cerr << "Since \"" << tableName
                << "\" is not a valid table name it will not be read (line "
                << lineCount << " - " << databaseFilePath << ")\n";
    }

    ++lineCount;
  }
}

const String &Database::getName() const
{
  return this->name;
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