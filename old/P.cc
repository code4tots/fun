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
