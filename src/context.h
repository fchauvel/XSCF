
#ifndef XCSF_CONTEXT_H
#define XCSF_CONTEXT_H


#include <vector>
#include <iostream>
#include <stdexcept>


using namespace std;


class Value
{
 public:
  Value(int value);
  Value(unsigned int value);
  Value(const Value& prototype);
  ~Value();

  Value& operator = (const Value& other);
  
  bool operator == (const Value& other) const;
  bool operator != (const Value& other) const;
  bool operator > (const Value& other) const;
  bool operator >= (const Value& other) const;
  bool operator < (const Value& other) const;
  bool operator <= (const Value& other) const;

  unsigned int unsigned_int(void) const;
  
  static const unsigned int MAXIMUM = 100;
  
 private:
  friend ostream& operator << (ostream& out, const Value& value);
  
  unsigned int _value;
  
};


class Vector
{
 public:
  Vector(unsigned int dimension_count);
  Vector(initializer_list<int> values);
  Vector(const vector<int>& values);
  Vector(const vector<unsigned int>& values);
  Vector(const Vector& other);
  ~Vector();

  void operator = (const Vector& other);
  bool operator == (const Vector& other) const;
  bool operator < (const Vector& other) const;
  
  unsigned int size(void) const;
  const Value& operator [] (unsigned int index) const;


  static Vector parse(const string& text);
  
 private:
  friend ostream& operator << (ostream& out, const Vector& vector);
  
  vector<Value> _values;
  
};






#endif
