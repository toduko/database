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
  void update(size_t column, const String &tableName);
  void insert(const String &tableName);
  void innerJoin(const String &table1Name, size_t column1, const String &table2Name, size_t column2);
  void aggregate(const String &tableName, size_t searchColumn, const String &value, size_t targetColumn, const String &operation) const;
  void select(size_t column, const String &value, const String &tableName) const;
  void describeTable(const String &tableName) const;
  void printTable(const String &tableName) const;
  void importTable(const String &tableName);
  void exportTable(const String &tableName, const String &file) const;
  void renameTable(const String &oldName, const String &newName);
  void countRows(size_t column, const String &value, const String &tableName) const;
  void deleteRows(size_t column, const String &value, const String &tableName);
  void addColumn(const String &tableName, const String &columnType);
  void writeTo(std::ofstream &file) const;
  void printTableNames() const;
};

#endif