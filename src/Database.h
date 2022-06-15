#ifndef DATABASE_H_
#define DATABASE_H_

#include "Vector.h"
#include "String.h"

class Database
{
private:
  Vector<String> tables;
  String name;

public:
  Database(const String &databaseFilePath);

  const String &getName() const;

  void writeTo(std::ofstream &file) const;
  void printTableNames() const;
};

#endif