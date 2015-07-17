
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
    if (opcode.type == "string") {
      valuestack.push_back(P(new Data(opcode.sval)));
    } else {
      err = string("Unrecognized opcode: ") + opcode.type;
    }
  }

};
