#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <string>
using namespace std;
class Validator {
private:
public:
  Validator();
  size_t obtainColumnPosition(string &schema);
};

#endif //VALIDATOR_H
