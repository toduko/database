#include "Validator.h"

bool IntValidator::isValid(const String &data) const
{
  if (!data.getLength())
  {
    return false;
  }

  String unsignedVersion(data);
  if (data[0] == '-' || data[0] == '+')
  {
    if ((unsignedVersion = std::move(unsignedVersion.removeFirst())) == String(""))
    {
      return false;
    }
  }

  for (size_t i = 0; i < unsignedVersion.getLength(); ++i)
  {
    if (!String::isDigit(unsignedVersion[i]))
    {
      return false;
    }
  }

  return true;
}

bool DoubleValidator::isValid(const String &data) const
{
  if (!data.getLength())
  {
    return false;
  }

  String unsignedVersion(data);
  if (data[0] == '-' || data[0] == '+')
  {
    if ((unsignedVersion = std::move(unsignedVersion.removeFirst())) == String(""))
    {
      return false;
    }
  }

  Vector<String> numbers = std::move(data.split('.'));

  if (numbers.getSize() != 2)
  {
    return false;
  }

  IntValidator validator;

  return validator.isValid(numbers[0]) && validator.isValid(numbers[1]);
}

bool StringValidator::isValid(const String &data) const
{
  if (data.getLength() < 2)
  {
    return false;
  }

  if (data[0] != '"' || data[data.getLength() - 1] != '"')
  {
    return false;
  }

  String trimmed = std::move(data.removeFirst().removeLast());
  
  for (size_t i = 0; i < trimmed.getLength(); ++i)
  {
    if (trimmed[i] == '\\')
    {
      if ((i >= trimmed.getLength() - 1) || !String::isEscapableCharacter(trimmed[i + 1]))
      {
        return false;
      }
      ++i;
    }
  }

  return true;
}