

#include "population.h"


Population::Population():
  _classifiers()
{}


Population::~Population()
{
  for(list<Classifier*>::const_iterator each = _classifiers.begin();
      each != _classifiers.end();
      ++each) {
    delete *each;
  }
}


void
Population::add_classifier(Action& action)
{
  Classifier* classifier = new Classifier(1, 1);
  _classifiers.push_back(classifier);
}


Classifier*
Population::first(void)
{
  if (_classifiers.empty()) throw "Error, the population is empty!";
  return _classifiers.front();
}
