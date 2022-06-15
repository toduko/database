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

  static void writeTo(std::ofstream &file);
  static void read(const Vector<String> &args);
  static void save(const Vector<String> &args);
  static void saveAs(const Vector<String> &args);
  static void close(const Vector<String> &args);

  void stop() override;
  DatabaseCLI();

public:
  DatabaseCLI(const DatabaseCLI &other) = delete;
  DatabaseCLI &operator=(const DatabaseCLI &other) = delete;
  
  static DatabaseCLI &getInstance();
};

#endif