#ifndef PAGER_H_
#define PAGER_H_

#include "Vector.h"
#include "String.h"

class Pager
{
private:
  bool shouldKeepOpen;
  size_t currentPage;
  size_t linesInPage;
  Vector<Vector<String>> pages;

  void parse(const String &command);

  void printPage() const;

  bool previousPage();
  bool nextPage();

  void close();

public:
  Pager(const Vector<String> &lines, size_t linesInPage);
};

#endif