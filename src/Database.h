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

  void writeTo(std::ofstream &file) const;
  void printTableNames() const;
};

#endif