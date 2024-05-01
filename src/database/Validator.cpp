#include "database/Validator.h"

Validator::Validator() {
}

int Validator::obtainColumnPosition(string &schema, string &columnName) {
  size_t pos = schema.find(columnName);
  if (pos == string::npos) {
    return -1;
  }
  int cont = 0;
  for (size_t i = 0; i < pos; i++) {
    if (schema[i] == '#') {
      cont++;
    }
  }
  return cont / 2;
}
