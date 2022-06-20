#include "Table.h"
#include "Pager.h"

#include <fstream>

const String Table::FILE_EXTENSION(".table");
const size_t Table::LINES_IN_PAGE = 5;

Table::Table(const String &name, const Vector<DataType> &columnTypes)
    : name(name), columnTypes(columnTypes)
{
  for (size_t i = 0; i < columnTypes.getSize(); ++i)
  {
    this->data.push(Vector<String>());
  }
}

bool Table::addColumn(DataType columnType)
{
  if (columnType != DataType::INT && columnType != DataType::DOUBLE && columnType != DataType::STRING)
  {
    return false;
  }

  this->columnTypes.push(columnType);
  this->data.push(Vector<String>());

  if (this->columnTypes.getSize() > 1)
  {
    for (size_t i = 0; i < this->data[0].getSize(); ++i)
    {
      this->data[this->columnTypes.getSize() - 1].push(String("NULL"));
    }
  }

  return true;
}

bool Table::addRow(const Vector<String> &row)
{
  if (row.getSize() != this->columnTypes.getSize())
  {
    return false;
  }

  for (size_t i = 0; i < row.getSize(); ++i)
  {
    if (row[i] != String("NULL") && !ValidationManager::isValid(row[i], this->columnTypes[i]))
    {
      return false;
    }
  }

  for (size_t i = 0; i < row.getSize(); ++i)
  {
    this->data[i].push(row[i]);
  }

  return true;
}

Table Table::innerJoin(size_t column, const Table &other, size_t otherColumn) const
{
  if (column-- > this->columnTypes.getSize() || otherColumn-- > other.columnTypes.getSize())
  {
    throw "Column number too large";
  }

  if (this->columnTypes[column] != other.columnTypes[otherColumn])
  {
    throw "Column types do not match";
  }

  Vector<DataType> columnTypes(this->columnTypes);

  for (size_t i = 0; i < other.columnTypes.getSize(); ++i)
  {
    if (i != otherColumn)
    {
      columnTypes.push(other.columnTypes[i]);
    }
  }

  Table result(String("InnerJoin") + this->name + other.name, columnTypes);

  for (size_t i = 0; i < this->data[column].getSize(); ++i)
  {
    for (size_t j = 0; j < other.data[otherColumn].getSize(); ++j)
    {
      if (this->data[column][i] == other.data[otherColumn][j])
      {
        Vector<String> row;

        for (size_t k = 0; k < this->columnTypes.getSize(); ++k)
        {
          row.push(this->data[k][i]);
        }

        for (size_t k = 0; k < other.columnTypes.getSize(); ++k)
        {
          if (k != otherColumn)
          {
            row.push(other.data[k][i]);
          }
        }

        result.addRow(row);
      }
    }
  }

  return result;
}

size_t Table::update(size_t column, const String &search, const String &replace)
{
  if (column-- > this->columnTypes.getSize())
  {
    throw "Column number too large";
  }

  if (!ValidationManager::isValid(replace, this->columnTypes[column]))
  {
    throw "Invalid new data";
  }

  size_t numUpdated = 0;

  for (size_t i = 0; i < this->data[column].getSize(); ++i)
  {
    if (this->data[column][i] == search)
    {
      this->data[column][i] = replace;
      ++numUpdated;
    }
  }

  return numUpdated;
}

size_t Table::deleteRows(size_t column, const String &value)
{
  if (column-- > this->columnTypes.getSize())
  {
    throw "Column number too large";
  }

  size_t numDeleted = 0;

  for (size_t i = 0; i < this->data[column].getSize(); ++i)
  {
    if (this->data[column][i] == value)
    {
      for (size_t j = 0; j < this->columnTypes.getSize(); ++j)
      {
        this->data[j].remove(i);
      }
      ++numDeleted;
      --i;
    }
  }

  return numDeleted;
}

size_t Table::countRows(size_t column, const String &value) const
{
  if (column-- > this->columnTypes.getSize())
  {
    throw "Column number too large";
  }

  size_t rows = 0;

  for (size_t i = 0; i < this->data[column].getSize(); ++i)
  {
    if (this->data[column][i] == value)
    {
      ++rows;
    }
  }

  return rows;
}

void Table::select(size_t column, const String &value) const
{
  if (column-- > this->columnTypes.getSize())
  {
    throw "Column number too large";
  }

  Vector<String> lines;
  for (size_t i = 0; i < this->data[column].getSize(); ++i)
  {
    if (this->data[column][i] == value)
    {
      String line("");

      for (size_t j = 0; j < this->columnTypes.getSize(); ++j)
      {
        line += this->data[j][i];
        line += String(" ");
      }

      lines.push(line);
    }
  }

  Pager(lines, Table::LINES_IN_PAGE);
}

void Table::describe() const
{
  std::cout << "The column types are in the following order: ";

  for (size_t i = 0; i < this->columnTypes.getSize(); ++i)
  {
    if (this->columnTypes[i] == DataType::INT)
    {
      std::cout << "Int ";
    }
    else if (this->columnTypes[i] == DataType::DOUBLE)
    {
      std::cout << "Double ";
    }
    else if (this->columnTypes[i] == DataType::STRING)
    {
      std::cout << "String ";
    }
  }

  std::cout << std::endl;
}

void Table::print() const
{
  Vector<String> lines;

  for (size_t i = 0; i < this->data[0].getSize(); ++i)
  {
    String line("");

    for (size_t j = 0; j < this->data.getSize(); ++j)
    {
      line += this->data[j][i];
      line += String(" ");
    }

    lines.push(line);
  }

  Pager(lines, Table::LINES_IN_PAGE);
}

bool Table::isValidName(const String &tableName)
{
  return tableName.isOnlyLetters() && !tableName.isEmpty();
}

void Table::write() const
{
  this->writeTo(this->name);
}

void Table::writeTo(const String &filename) const
{
  if (!Table::isValidName(filename))
  {
    throw "Invalid table name";
  }

  std::ofstream file(filename + Table::FILE_EXTENSION, std::ios::trunc);

  if (!file.is_open())
  {
    throw "Could not open file";
  }

  file.seekp(0);

  String types("");

  for (size_t i = 0; i < this->columnTypes.getSize(); ++i)
  {
    types += this->columnTypes[i];
  }

  file << types;

  if (this->data.getSize() == 0)
  {
    return;
  }

  for (size_t i = 0; i < this->data[0].getSize(); ++i)
  {
    for (size_t j = 0; j < this->data.getSize(); ++j)
    {
      file << "\n"
           << this->data[j][i];
    }
  }

  file.close();
}

void Table::insert()
{
  Vector<String> row;

  for (size_t i = 0; i < this->columnTypes.getSize(); ++i)
  {
    String entry;
    std::cout << "Column " << i + 1 << " data: ";
    std::cin >> entry;
    row.push(entry);
  }

  if (!this->addRow(row))
  {
    throw "Invalid data in row";
  }
}

void Table::setName(const String &name)
{
  if (!Table::isValidName(name))
  {
    throw "Invalid new name for table";
  }

  this->name = name;
}

const String &Table::getName() const
{
  return this->name;
}

Optional<Table> Table::createTable(const String &filename)
{
  if (!Table::isValidName(filename))
  {
    throw "Invalid table name";
  }

  Optional<Table> result;

  std::ifstream file(filename + Table::FILE_EXTENSION, std::ios::app);

  file.seekg(0);

  if (file.is_open())
  {
    size_t lineCount = 1;
    bool hasInvalidData = false;
    Vector<DataType> columnTypes;
    Vector<String> row;
    Optional<Table> temp;

    while (!file.eof() && !hasInvalidData)
    {
      String line;
      file >> line;

      if (lineCount == 1)
      {
        if (!line.getLength())
        {
          hasInvalidData = true;
          std::cerr << "Tables must have types defined in the first line of their file\n";
        }

        for (size_t i = 0; i < line.getLength(); ++i)
        {
          if (line[i] == DataType::INT || line[i] == DataType::DOUBLE || line[i] == DataType::STRING)
          {
            columnTypes.push(DataType(line[i]));
          }
          else
          {
            hasInvalidData = true;
            std::cerr << "Invalid types for table columns (line 1 - " << filename << ")\n";
            break;
          }
        }

        if (!hasInvalidData)
        {
          temp = std::move(Table(filename, columnTypes));
        }
      }
      else
      {
        if (row.getSize() < columnTypes.getSize())
        {
          row.push(line);

          if (row.getSize() == columnTypes.getSize())
          {
            if (!temp.getData().addRow(row))
            {
              hasInvalidData = true;
              std::cerr << "Invalid data for row (lines "
                        << lineCount - columnTypes.getSize() + 1
                        << " to " << lineCount << " - " << filename << ")\n";
            }

            row.clear();
          }
        }
      }

      ++lineCount;
    }

    if (row.getSize() > 0 && row.getSize() < columnTypes.getSize())
    {
      hasInvalidData = true;
      std::cerr << "Not enough columns for last row\n";
    }

    if (!hasInvalidData)
    {
      result = std::move(temp);
    }
  }

  file.close();

  return result;
}