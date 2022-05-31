#ifndef STRING_H_
#define STRING_H_

#include "Vector.h"

#include <cstddef>
#include <iostream>
#include <fstream>

class String
{
private:
  size_t length;
  char *data;

  void copy(const char *data);
  void move(String &&other);
  void free();

public:
  String(char character);
  String(const char *data = "");
  String(const String &other);
  String(String &&other);
  String &operator=(const String &other);
  String &operator=(String &&other);
  ~String();

  String toLowercase() const;
  String toUppercase() const;

  String removeFirst() const;
  String removeLast() const;

  Vector<String> split(char splitSymbol) const;
  size_t count(char symbol) const;

  bool isEmpty() const;

  bool includes(const String &substring) const;

  char &operator[](size_t index);
  const char &operator[](size_t index) const;

  operator const char *() const;
  String &concat(const char *extra);

  const char *getData() const;
  size_t getLength() const;

  String &operator+=(const String &other);

  static bool isDigit(char c);
  static unsigned toDigit(char c);
  static char toChar(unsigned digit);
};

String operator+(const String &string1, const String &string2);
bool operator==(const String &string1, const String &string2);
bool operator!=(const String &string1, const String &string2);

std::istream &operator>>(std::istream &is, String &string);
std::ostream &operator<<(std::ostream &os, const String &string);

#endif