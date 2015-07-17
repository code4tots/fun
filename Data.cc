/*
type is one of:
  str
  num
  list
  dict
  lambda
    lambda uses ival to point into opcode array, and
    dval which is scope.
*/
struct Data;

struct P {
  Data *data;
  P() : data(nullptr) {}
  P(Data *data) : data(data) {}
  Data *operator->() {
    return data;
  }
  const Data *operator->() const {
    return data;
  }
};

struct Data {
  string type;
  string sval;
  double nval;
  int ival;
  deque<P> lval;
  map<P, P> dval;
  Data() : type("none") {}
  Data(const string& value) : type("string"), sval(value) {}

  bool eq(const P b) const {
    return type == b->type && sval == b->sval;
  }

  bool lt(const P b) const {
    return type < b->type || sval < b->sval;
  }

};

bool operator==(const P a, const P b) {
  return a->eq(b);
}

bool operator<(const P a, const P b) {
  return a->lt(b);
}
