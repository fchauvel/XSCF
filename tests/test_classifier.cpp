#include "CppUTest/TestHarness.h"


#include "classifier.h"


TEST_GROUP(TestClassifier)
{
};

TEST(TestClassifier, test_invalid_context){
  vector<int> values = {25, 23, 45};
  Context context(values);
  Classifier classifier(1, 1);
  
  CHECK_THROWS(std::invalid_argument, {classifier.match(context);});
}

TEST(TestClassifier, test_match_one_value){
  vector<int> values = { 25 };
  Context context(values);
  Classifier classifier(1, 1);
  classifier[0] = Interval(20, 30);
  
  CHECK(classifier.match(context));
}

TEST(TestClassifier, test_match_three_value){
  vector<int> values = { 25, 12, 85 };
  Context context(values);
  Classifier classifier(3, 1);
  classifier[0] = Interval(20, 30);
  classifier[1] = Interval(10, 15);
  classifier[2] = Interval(80, 99);
  
  CHECK(classifier.match(context));
}

TEST(TestClassifier, test_mismatch){
  vector<int> values = { 25 };
  Context context(values);
  Classifier classifier(1, 1);
  classifier[0] = Interval(30, 40);
  
  CHECK_FALSE(classifier.match(context));
}

TEST(TestClassifier, test_mismatch_three_value){
  vector<int> values = { 25, 12, 12 };
  Context context(values);
  Classifier classifier(3, 1);
  classifier[0] = Interval(20, 30);
  classifier[1] = Interval(10, 15);
  classifier[2] = Interval(80, 99);
  
  CHECK_FALSE(classifier.match(context));   
}
