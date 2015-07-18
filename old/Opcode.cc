/*
type is one of:
  str
  num
  begin
  end
  name
  assign
  attr
  mark
*/
struct Opcode {
  const string type;
  const string sval;
  const double nval = 0;
  const int ival = 0;

  static Opcode str(const string& value) {
    return Opcode("str", value);
  }
  static Opcode num(const double value) {
    return Opcode("num", value);
  }
  static Opcode begin() {
    return Opcode("begin");
  }
  static Opcode end() {
    return Opcode("end");
  }
  static Opcode name(const string& value) {
    return Opcode("name", value);
  }
  static Opcode assign(const string& value) {
    return Opcode("assign", value);
  }
  static Opcode attr(const string& value) {
    return Opcode("attr", value);
  }
  static Opcode mark() {
    return Opcode("mark");
  }

private:
  Opcode(const string& type): type(type) {}
  Opcode(const string& type, const string& value): type(type), sval(value) {}
  Opcode(const string& type, double value): type(type), nval(value) {}
  Opcode(const string& type, int value): type(type), nval(value) {}
};
