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

void Database::update(size_t column, const String &tableName)
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  String search, replace;
  std::cout << "Enter value to replace: ";
  std::cin >> search;
  std::cout << "Enter new value: ";
  std::cin >> replace;

  size_t numUpdated = this->tables[index].update(column, search, replace);
  if (numUpdated)
  {
    std::cout << "Successfully updated " << numUpdated << (numUpdated == 1 ? " row\n" : " rows\n");
  }
  else
  {
    throw "No rows affected";
  }
}

void Database::insert(const String &tableName)
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  this->tables[index].insert();
}

void Database::innerJoin(const String &table1Name, size_t column1, const String &table2Name, size_t column2)
{
  int index1 = this->findTable(table1Name);

  if (index1 < 0)
  {
    throw "Table 1 not found";
  }

  int index2 = this->findTable(table2Name);

  if (index2 < 0)
  {
    throw "Table 2 not found";
  }

  Table result = this->tables[index1].innerJoin(column1, this->tables[index2], column2);
  std::cout << "Successfully created table " << result.getName() << "\n";
  this->tables.push(result);
}

const String &Database::getName() const
{
  return this->name;
}

void Database::select(size_t column, const String &value, const String &tableName) const
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  this->tables[index].select(column, value);
}

void Database::describeTable(const String &tableName) const
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  this->tables[index].describe();
}

void Database::printTable(const String &tableName) const
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  this->tables[index].print();
}

void Database::importTable(const String &tableName)
{
  if (this->findTable(tableName) >= 0)
  {
    throw "Table already exists in database";
  }

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

void Database::exportTable(const String &tableName, const String &file) const
{
  int index = this->findTable(tableName);

  if (index == -1)
  {
    throw "Could not find a table with this name";
  }

  this->tables[index].writeTo(file);
}

void Database::renameTable(const String &oldName, const String &newName)
{
  int index = this->findTable(oldName);

  if (index == -1)
  {
    throw "Could not find table to rename";
  }

  if (this->findTable(newName) != -1)
  {
    throw "New name must be unique";
  }

  this->tables[index].setName(newName);
}

void Database::countRows(size_t column, const String &value, const String &tableName) const
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  size_t rows = this->tables[index].countRows(column, value);
  std::cout << "Total rows containing value: " << rows << "\n";
}

void Database::deleteRows(size_t column, const String &value, const String &tableName)
{
  int index = this->findTable(tableName);

  if (index < 0)
  {
    throw "Table not found";
  }

  size_t numDeleted = this->tables[index].deleteRows(column, value);

  if (numDeleted)
  {
    std::cout << "Successfully deleted " << numDeleted << (numDeleted == 1 ? " row\n" : " rows\n");
  }
  else
  {
    throw "No rows deleted";
  }
}

void Database::addColumn(const String &tableName, const String &columnType)
{
  int index = this->findTable(tableName);

  if (index == -1)
  {
    throw "Could not find a table with this name";
  }

  String colType(std::move(columnType.toUppercase()));

  if (colType == String("INT") || colType == String("DOUBLE") || colType == String("STRING"))
  {
    this->tables[index].addColumn((DataType)colType[0]);
  }
  else
  {
    throw "Invalid type for new column";
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