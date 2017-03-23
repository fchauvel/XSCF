#ifndef XSCF_CLASSIFIER_H
#define XSCF_CLASSIFIER_H


#include <vector>

#include "context.h"
#include "actions.h"


using namespace std;


class Interval
{
 public:
  Interval(Value lower, Value upper);
  ~Interval();

  bool contains(const Value value) const;
  
 private:
  Value _lower_bound;
  Value _upper_bound;

};


class Classifier {

 public:
  Classifier(unsigned int dimension_count, unsigned int output_size);
  ~Classifier();

  Interval& operator [](unsigned int index);
  bool match(const Context& context) const;
  const vector<Value>& outputs(void) const;
  
 private:
  vector<Interval> _intervals;
  vector<Value> _outputs;

};

#endif
