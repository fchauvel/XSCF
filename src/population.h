
#ifndef XCSF_POPULATION_H
#define XCSF_POPULATION_H


#include <list>
#include <iterator>

#include "classifier.h"
#include "actions.h"


using namespace std;


class Population
{
 public:
  Population();
  ~Population();

  void add_classifier(Action& action);
  Classifier* first(void);
  
 private:
  list<Classifier*> _classifiers;
  

};

#endif
