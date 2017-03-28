

#include "context.h"

#include <iostream>


Value::Value(int value):
  Value(static_cast<unsigned>(value))
{}

Value::Value(unsigned int value)
{
  if (value > MAXIMUM)
    throw std::invalid_argument("A value cannot exceed the maximum");
  
  _value = value;
}

Value::Value(const Value& prototype)
{
  _value = prototype._value;
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


std::ostream&
operator << (std::ostream& out, const Value& value) {
  out << value._value;
  return out;
}


Vector::Vector(unsigned int dimension_count):
  _values(dimension_count, 0)
{}


Vector::Vector(vector<int>& values):
  _values(values.size(), 0)
{
  for (unsigned int index=0 ; index<values.size() ; index++)
    {
      _values[index] = values[index];
    }
}


Vector::Vector(initializer_list<int> values):
  _values(values.size(), 0)
{
  for (initializer_list<int>::iterator each = values.begin() ;
       each != values.end();
       ++each)
    {
      unsigned int index = each - values.begin();
      _values[index] = *each;
    }
}


Vector::Vector(const Vector& prototype)
  :_values(prototype._values)
{}
  

Vector::~Vector()
{}


void
Vector::operator = (const Vector& other)
{
  _values = other._values;
}  


unsigned int
Vector::size(void) const
{
  return _values.size();
}


bool
Vector::operator == (const Vector& other) const
{
  if (_values.size() != other._values.size()) return false;
  
  for(unsigned int index=0 ; index<_values.size() ; index++) {
    if (_values[index] != other._values[index]) return false;
  }
  
  return true;
}


const Value&
Vector::operator [] (unsigned int index) const
{
  return _values[index];
}


bool
Vector::operator < (const Vector& other) const
{
  for (unsigned int index=0 ; index<_values.size() ; ++index) {
    if (_values[index] == other._values[index]) continue;
    return _values[index] < other._values[index];
  }
  return false;
}


std::ostream&
operator << (ostream& out, const Vector& vector) {
  out << "[";
  for(unsigned int index=0 ; index<vector._values.size()-1 ; ++index) {
    out << vector._values[index] << ", ";
  }
  out << vector._values.back() <<  "]" << std::endl;
  return out;
}
