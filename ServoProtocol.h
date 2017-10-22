#include <Arduino.h>
class ServoProtocol {
  public:
    ServoProtocol();
    int *read();

  private:
    bool check(int *data, int checkSum);
    int *getData();
    int sum(int* array);
};
