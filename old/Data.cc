/*
type is one of:
  none
  str
  num
  list
  dict
  lambda
    lambda uses ival to point into opcode array, and
    dval which is scope.
*/
struct Data {
  string type;
  string sval;
  double nval = 0;
  int ival = 0;
  deque<P> lval;
  map<P, P> dval;
  Data(): type("none") {}
  Data(const string& val): type("str"), sval(val) {}
  Data(double val): type("num"), nval(val) {}
  Data(deque<P> val): type("list"), lval(val) {}
  Data(map<P, P> val): type("dict"), dval(val) {}
  Data(int ip, map<P, P> scope): type("lambda"), dval(scope) {}

  bool eq(const P b) const {
    return
        type == b->type &&
        sval == b->sval &&
        nval == b->nval &&
        ival == b->ival &&
        dval == b->dval;
  }

  bool lt(const P b) const {
    return
        type < b->type ||
        sval < b->sval ||
        nval < b->nval ||
        ival < b->ival ||
        dval < b->dval;
  }
};

bool operator==(const P a, const P b) {
  return a->eq(b);
}

bool operator<(const P a, const P b) {
  return a->lt(b);
}
