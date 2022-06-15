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
      this->tables.push(tableName);
    }
    else
    {
      std::cout << "Since \"" << tableName << "\" is not a valid table name it will not be read (line " << lineCount << ").\n";
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
    file << this->tables[i] << std::endl;
  }
}

void Database::printTableNames() const
{
  std::cout << this->tables << std::endl;
}