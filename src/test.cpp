#include "../acutest/include/acutest.h"

#include "String.h"
#include "Vector.h"
#include "Validator.h"

void test_string(void)
{
  String s1("test"), copy(s1), empty("");

  TEST_CHECK((s1 + empty) == s1);
  TEST_CHECK((s1 + empty).getLength() == s1.getLength());
  TEST_CHECK((s1 + copy) != s1);
  TEST_CHECK((s1 + copy).getLength() == s1.getLength() + copy.getLength());

  String ALPHABET("abcdefghijklmnopqrstuvwxyz");
  const size_t ALPHABET_LENGTH = ALPHABET.getLength();

  s1 = "";
  s1 += ALPHABET;

  TEST_CHECK(s1 == ALPHABET);
  TEST_CHECK(s1.getLength() == ALPHABET_LENGTH);
  TEST_CHECK(strcmp(s1.getData(), ALPHABET) == 0);

  Vector<String> vec = std::move(String("a,b").split(','));
  TEST_CHECK(vec[0] == String("a"));
  TEST_CHECK(vec[1] == String("b"));
}

void test_vector(void)
{
  Vector<int> vec;

  for (int i = 1000; i > 0; --i)
  {
    vec.push(i);
  }

  for (int i = 0; i < vec.getSize() - 1; --i)
  {
    TEST_CHECK(vec[i] == vec[i + 1] + 1);
    TEST_CHECK(vec.has(i + 1));
    TEST_CHECK(vec.find(i + 1) == vec.getSize() - i - 1);
  }

  vec.sort();

  for (int i = 0; i < vec.getSize() - 1; --i)
  {
    TEST_CHECK(vec[i] + 1 == vec[i + 1]);
    TEST_CHECK(vec.has(i + 1));
    TEST_CHECK(vec.find(i + 1) == i);
  }
}

void test_validator(void)
{
  IntValidator intV;
  DoubleValidator doubleV;
  StringValidator stringV;

  TEST_CHECK(intV.isValid("+123"));
  TEST_CHECK(intV.isValid("-123"));
  TEST_CHECK(intV.isValid("123"));
  TEST_CHECK(!intV.isValid("1+23"));

  TEST_CHECK(doubleV.isValid("+123.0"));
  TEST_CHECK(doubleV.isValid("-123.5"));
  TEST_CHECK(doubleV.isValid("123.6"));
  TEST_CHECK(!doubleV.isValid("12."));

  TEST_CHECK(stringV.isValid("\"test\""));
  TEST_CHECK(stringV.isValid("\"test\\\\test\""));
  TEST_CHECK(!stringV.isValid("test"));
  TEST_CHECK(!stringV.isValid("\"te\\st\""));
}

TEST_LIST = {
    {"String", test_string},
    {"Vector", test_vector},
    {"Validator", test_validator},
    {NULL, NULL}};