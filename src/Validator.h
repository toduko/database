#ifndef VALIDATOR_H_
#define VALIDATOR_H_
#include "String.h"

class Validator
{
public:
  virtual bool isValid(const String &data) const = 0;
  virtual ~Validator() = default;
};

class IntValidator final : public Validator
{
public:
  bool isValid(const String &data) const override;
};

class DoubleValidator final : public Validator
{
public:
  bool isValid(const String &data) const override;
};

class StringValidator final : public Validator
{
public:
  bool isValid(const String &data) const override;
};

#endif