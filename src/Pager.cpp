#include "Pager.h"

#include <iostream>

Pager::Pager(const Vector<String> &lines, size_t linesInPage) : linesInPage(linesInPage), shouldKeepOpen(true), currentPage(0)
{
  size_t pageIndex = 0;
  this->pages.push(Vector<String>());

  for (size_t i = 0; i < lines.getSize(); ++i)
  {
    this->pages[pageIndex].push(lines[i]);
    if (i && (i + 1) % linesInPage == 0 && i != lines.getSize() - 1)
    {
      this->pages.push(Vector<String>());
      ++pageIndex;
    }
  }

  this->printPage();

  while (this->shouldKeepOpen)
  {
    String cmd;
    std::cin >> cmd;
    this->parse(cmd);
  }
}

void Pager::parse(const String &command)
{
  if (command == String("prev"))
  {
    if (this->previousPage())
    {
      this->printPage();
    }
  }
  else if (command == String("next"))
  {
    if (this->nextPage())
    {
      this->printPage();
    }
  }
  else if (command == String("exit"))
  {
    this->close();
  }
  else
  {
    std::cerr << "Could not find the entered command. Use either prev, next or exit\n";
  }
}

bool Pager::previousPage()
{
  if (this->currentPage > 0)
  {
    --this->currentPage;
    return true;
  }
  else
  {
    std::cerr << "Already on first page\n";
    return false;
  }
}

bool Pager::nextPage()
{
  if (this->currentPage < this->pages.getSize() - 1)
  {
    ++this->currentPage;
    return true;
  }
  else
  {
    std::cerr << "Already on last page\n";
    return false;
  }
}

void Pager::close()
{
  this->shouldKeepOpen = false;
}

void Pager::printPage() const
{
  std::cout << "Page " << this->currentPage + 1 << '/' << this->pages.getSize() << ":\n";

  for (size_t i = 0; i < this->pages[this->currentPage].getSize(); ++i)
  {
    std::cout << this->pages[this->currentPage][i] << std::endl;
  }

  std::cout << "Type prev to go to previous page, next to go to next page and exit to close the pager\n";
}