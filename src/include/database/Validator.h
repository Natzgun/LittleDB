#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <string>
using namespace std;
class Validator {
private:
public:
  Validator();
  int obtainColumnPosition(string &schema, string &columnName);
};

#endif //VALIDATOR_H
