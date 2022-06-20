#ifndef DATABASE_H_
#define DATABASE_H_

#include "Vector.h"
#include "String.h"
#include "Table.h"

class Database
{
private:
  Vector<Table> tables;
  String name;

public:
  Database(const String &databaseFilePath);

  const String &getName() const;

  int findTable(const String &tableName) const;
  void select(size_t column, const String &value, const String &tableName) const;
  void describeTable(const String &tableName) const;
  void printTable(const String &tableName) const;
  void importTable(const String &tableName);
  void exportTable(const String &tableName, const String &file) const;
  void renameTable(const String &oldName, const String &newName);
  void addColumn(const String& tableName, const String& columnType);
  void writeTo(std::ofstream &file) const;
  void printTableNames() const;
};

#endif