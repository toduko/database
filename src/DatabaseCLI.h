#ifndef DATABASE_CLI_H_
#define DATABASE_CLI_H_

#include "CLI.h"

#include <fstream>

class DatabaseCLI final : public CLI
{
private:
  static Vector<char> bytes;
  static String filename;
  static bool isFileOpen;

  static void writeTo(std::ofstream &file);
  static void read(const Vector<String> &args);
  static void save(const Vector<String> &args);
  static void saveAs(const Vector<String> &args);
  static void close(const Vector<String> &args);

  void stop() override;
  DatabaseCLI();

public:
  static DatabaseCLI &getInstance();
};

#endif