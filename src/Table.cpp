#include "Table.h"

#include <fstream>

const String Table::FILE_EXTENSION(".table");

Table::Table(const String &name, const Vector<DataType> &columnTypes)
    : name(name), columnTypes(columnTypes)
{
  for (size_t i = 0; i < columnTypes.getSize(); ++i)
  {
    this->data.push(Vector<String>());
  }
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

void Table::write() const
{
  std::ofstream file(this->name + Table::FILE_EXTENSION, std::ios::trunc);

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

const String &Table::getName() const
{
  return this->name;
}

Optional<Table> Table::createTable(const String &filename)
{
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