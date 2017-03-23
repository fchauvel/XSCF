
#ifndef XCSF_CONTEXT_H
#define XCSF_CONTEXT_H


#include <vector>
#include <stdexcept>


using namespace std;


class Value
{
 public:
  Value(int value);
  Value(unsigned int value);
  ~Value();

  void operator = (const Value& other);
  
  bool operator == (const Value& other) const;
  bool operator != (const Value& other) const;
  bool operator > (const Value& other) const;
  bool operator >= (const Value& other) const;
  bool operator < (const Value& other) const;
  bool operator <= (const Value& other) const;

  unsigned int to_unsigned_int(void) const;
  
  static const unsigned int MAXIMUM = 100;
  
 private:
  unsigned int _value;
  
};


class Context
{
 public:
  Context(unsigned int dimension_count);
  Context(vector<int>& values);

  unsigned int size(void) const;
  const Value& operator [] (unsigned int index) const;
  
 private:
  vector<Value> _values;
  
};

#endif
