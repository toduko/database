#include "String.h"

#include <utility>
#include <cstring>

String::String(const char *data)
{
  this->copy(data);
}

String::String(char character)
{
  char data[2] = {character, 0};
  this->copy(data);
}

String::String(const String &other)
{
  this->copy(other.data);
}

String::String(String &&other)
{
  this->move(std::move(other));
}

String &String::operator=(const String &other)
{
  if (this != &other)
  {
    this->free();
    this->copy(other.data);
  }

  return *this;
}

String &String::operator=(String &&other)
{
  if (this != &other)
  {
    this->move(std::move(other));
  }

  return *this;
}

char &String::operator[](size_t index)
{
  if (index >= this->length)
  {
    throw "Invalid index";
  }
  return this->data[index];
}

const char &String::operator[](size_t index) const
{
  if (index >= this->length)
  {
    throw "Invalid index";
  }
  return this->data[index];
}

String::operator const char *() const
{
  return this->data;
}

String::~String()
{
  this->free();
}

void String::free()
{
  delete[] this->data;
}

void String::copy(const char *data)
{
  if (data == nullptr)
  {
    throw "Cannot copy nullptr to string";
  }

  this->length = strlen(data);
  this->data = new char[this->length + 1];
  strcpy(this->data, data);
}

void String::move(String &&other)
{
  this->data = other.data;
  this->length = other.length;
  other.data = nullptr;
}

String &String::concat(const char *extra)
{
  if (extra == nullptr)
  {
    throw "Cannot copy nullptr to string";
  }

  size_t newLength = this->length + strlen(extra);
  char *newData = new char[newLength + 1];

  strcpy(newData, this->data);
  strcat(newData, extra);

  this->free();
  this->length = newLength;
  this->data = newData;

  return *this;
}

const char *String::getData() const
{
  return this->data;
}

size_t String::getLength() const
{
  return this->length;
}

bool String::isEmpty() const
{
  return this->length == 0;
}

bool String::isOnlyLetters() const
{
  for (size_t i = 0; i < this->length; ++i)
  {
    if (!String::isLetter(this->data[i]))
    {
      return false;
    }
  }

  return true;
}

bool String::isEscapableCharacter(char c)
{
  return c == '\\' || c == '"';
}

bool String::includes(const String &substring) const
{
  return strstr(this->data, substring.data) != nullptr;
}

String String::toLowercase() const
{
  String result(*this);

  for (size_t i = 0; i < result.length; ++i)
  {
    if (result[i] >= 'A' && result[i] <= 'Z')
    {
      result[i] |= 32;
    }
  }

  return result;
}

String String::toUppercase() const
{
  String result(*this);

  for (size_t i = 0; i < result.length; ++i)
  {
    if (result[i] >= 'a' && result[i] <= 'z')
    {
      result[i] &= ~32;
    }
  }

  return result;
}

String String::removeFirst() const
{
  if (this->length == 0)
  {
    throw "No elements to remove";
  }
  return String(this->data + 1);
}

String String::removeLast() const
{
  if (this->length == 0)
  {
    throw "No elements to remove";
  }

  char *newData = new char[this->length];
  strncpy(newData, this->data, this->length);
  newData[this->length - 1] = 0;

  String result(newData);
  delete[] newData;

  return result;
}

String &String::operator+=(const String &other)
{
  return this->concat(other.data);
}

String operator+(const String &string1, const String &string2)
{
  String copy(string1);

  return (copy += string2);
}

bool operator==(const String &string1, const String &string2)
{
  return strcmp(string1.getData(), string2.getData()) == 0;
}

bool operator!=(const String &string1, const String &string2)
{
  return !(string1 == string2);
}

Vector<String> String::split(char splitSymbol) const
{
  Vector<String> result;
  String s("");
  bool hasUnpushedData = false;

  for (size_t i = 0; i < this->length; ++i)
  {
    if (this->data[i] == splitSymbol)
    {
      result.push(s);
      s = String("");
      hasUnpushedData = false;
    }
    else
    {
      s += this->data[i];
      hasUnpushedData = true;
    }
  }

  if (hasUnpushedData)
  {
    result.push(s);
  }

  return result;
}

size_t String::count(char symbol) const
{
  size_t count = 0;

  for (size_t i = 0; i < this->length; ++i)
  {
    count += this->data[i] == symbol;
  }

  return count;
}

bool String::isLetter(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool String::isDigit(char c)
{
  return c >= '0' && c <= '9';
}

unsigned String::toDigit(char c)
{
  if (!String::isDigit(c))
  {
    throw "Character is not a digit";
  }
  return c - '0';
}

char String::toChar(unsigned digit)
{
  if (digit >= 10)
  {
    throw "Number is not a digit";
  }
  return '0' + digit;
}

std::istream &operator>>(std::istream &is, String &string)
{
  const size_t MAX_INPUT_LENGTH = 1024;
  char input[MAX_INPUT_LENGTH + 1];

  is.getline(input, MAX_INPUT_LENGTH);
  string = input;

  return is;
}

std::ostream &operator<<(std::ostream &os, const String &string)
{
  os << string.getData();

  return os;
}