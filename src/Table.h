#ifndef TABLE_H_
#define TABLE_H_

#include "Validator.h"
#include "Optional.h"
#include "String.h"
#include "Vector.h"

class Table
{
private:
  const static String FILE_EXTENSION;
  const static size_t LINES_IN_PAGE;
  String name;
  Vector<DataType> columnTypes;
  Vector<Vector<String>> data;

  Table(const String &name, const Vector<DataType> &columnTypes);

public:
  bool addColumn(DataType columnType);
  bool addRow(const Vector<String> &row);

  size_t deleteRows(size_t column, const String &value);

  void select(size_t column, const String &value) const;

  void describe() const;
  void print() const;

  void write() const;
  void writeTo(const String &filename) const;

  void setName(const String &name);

  const String &getName() const;

  static bool isValidName(const String &tableName);
  static Optional<Table> createTable(const String &filename);
};

#endif