struct Machine {
  // Contains error message if there is error.
  string err;

  int ip = 0;
  deque<Opcode> codes;

  deque<P> valuestack;

  Opcode fetch() {
    return codes[ip];
  }

  void step() {
    Opcode opcode = fetch();
    if (opcode.type == "str") {
      valuestack.push_back(P(new Data(opcode.sval)));
    } else {
      err = string("Unrecognized opcode: ") + opcode.type;
    }
  }

  void load(const string& text) {
    istringstream oss(text);
    string type;
    oss >> type;
    while (!oss.eof()) {
      if (type == "str") {
        string value;
        int count;
        oss >> count;
        for (int i = 0; i < count; i++) {
          int point;
          oss >> point;
          value.push_back(point);
        }
        codes.push_back(Opcode::str(value));
      } else if (type == "num") {
        double value;
        oss >> value;
        codes.push_back(Opcode::num(value));
      } else if (type == "begin") {
      } else {
        err = string("Unrecognized opcode: ") + type;
        return;
      }
      oss >> type;
    }
  }
};
