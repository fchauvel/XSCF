#ifndef XSCF_CLASSIFIER_H
#define XSCF_CLASSIFIER_H


#include "actions.h"


using namespace std;


class Classifier {

 public:
  Classifier(Action& action);
  ~Classifier();

  Action& action(void) const;
  
 private:
  Action& _action;

};

#endif
