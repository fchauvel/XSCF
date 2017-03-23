#include "CppUTest/TestHarness.h"


#include "context.h"


TEST_GROUP(TestValue)
{};


TEST(TestValue, test_equals_itself) {
  Value value(50);
  CHECK(value == value);
};

TEST(TestValue, test_equals_unsigned_int) {
  Value value(50);
  CHECK(value == 50U);
};

TEST(TestValue, test_equals_int) {
  Value value(50);
  CHECK(value == 50);
};

TEST(TestValue, test_differs) {
  Value v1(50);
  Value v2(40);
  CHECK(v1 != v2);
};

TEST(TestValue, test_greater_than) {
  Value v1(50);
  Value v2(40);
  CHECK(v1 > v2);
};

TEST(TestValue, test_greater_or_equals_than) {
  Value v1(50);
  Value v2(50);
  CHECK(v1 >= v2);
};

TEST(TestValue, test_less_than) {
  Value v1(40);
  Value v2(50);
  CHECK(v1 < v2);
};

TEST(TestValue, test_less_or_equal_than) {
  Value v1(50);
  Value v2(50);
  CHECK(v1 <= v2);
};


TEST(TestValue, test_invalid_values)
{
   CHECK_THROWS(std::invalid_argument, {Value value(150);});
}


