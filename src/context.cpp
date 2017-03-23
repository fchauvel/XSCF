
#include "context.h"

Value::Value(int value):
  Value(static_cast<unsigned>(value))
{}

Value::Value(unsigned int value)
{
  if (value > MAXIMUM) throw std::invalid_argument("A value cannot exceed the maximum");
  _value = value;
}

Value::~Value()
{}


void
Value::operator = (const Value& other)
{
  _value = other._value;
}

bool
Value::operator == (const Value& other) const
{
  return _value == other._value;
}


bool
Value::operator != (const Value& other) const
{
  return !(*this == other);
}


bool
Value::operator > (const Value& other) const
{
  return _value > other._value;
}


bool
Value::operator >= (const Value& other) const
{
  return _value > other._value or _value == other._value;
}


bool
Value::operator < (const Value& other) const
{
  return !(_value >= other._value);
}

bool
Value::operator <= (const Value& other) const
{
  return _value < other._value or _value == other._value;
}

unsigned int
Value::to_unsigned_int(void) const
{
  return _value;
}


Context::Context(unsigned int dimension_count):
  _values(dimension_count, 0)
{}


Context::Context(vector<int>& values):
  _values(values.size(), 0)
{
  for (unsigned int index=0 ; index<values.size() ; index++)
    {
      _values[index] = values[index];
    }
}

unsigned int
Context::size(void) const
{
  return _values.size();
}


const Value&
Context::operator [] (unsigned int index) const
{
  return _values[index];
}
