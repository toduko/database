#ifndef DATABASE_CLI_H_
#define DATABASE_CLI_H_

#include "Database.h"
#include "Optional.h"
#include "CLI.h"

#include <fstream>

class DatabaseCLI final : public CLI
{
private:
  static Optional<Database> database;
  static bool hasChanges;

  static void writeTo(std::ofstream &file);
  static void read(const Vector<String> &args);
  static void save(const Vector<String> &args);
  static void print(const Vector<String> &args);
  static void insert(const Vector<String> &args);
  static void update(const Vector<String> &args);
  static void aggregate(const Vector<String> &args);
  static void innerJoin(const Vector<String> &args);
  static void deleteRows(const Vector<String> &args);
  static void countRows(const Vector<String> &args);
  static void select(const Vector<String> &args);
  static void saveAs(const Vector<String> &args);
  static void describe(const Vector<String> &args);
  static void rename(const Vector<String> &args);
  static void import(const Vector<String> &args);
  static void exportTable(const Vector<String> &args);
  static void close(const Vector<String> &args);
  static void addColumn(const Vector<String> &args);
  static void showTables(const Vector<String> &args);

  void stop() override;
  DatabaseCLI();

public:
  DatabaseCLI(const DatabaseCLI &other) = delete;
  DatabaseCLI &operator=(const DatabaseCLI &other) = delete;

  static DatabaseCLI &getInstance();
};

#endif