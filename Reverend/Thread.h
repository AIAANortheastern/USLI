typedef void (* GenericCallback)(unsigned long);

class Thread {
  private:
  unsigned long last_called;
  unsigned long timeout;
  GenericCallback callback;
  
  public:
  Thread(GenericCallback function, unsigned long interval) {
    last_called = 0;
    timeout = interval;
    callback = function;
  }
  
  bool update(unsigned long time) {
    unsigned long diff = time - last_called;
    if (diff >= timeout) {
      last_called = time;
      callback(diff);
      return true;
    }
    return false;
  }
};
