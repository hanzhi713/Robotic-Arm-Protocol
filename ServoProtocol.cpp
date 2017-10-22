#include "ServoProtocol.h"
ServoProtocol::ServoProtocol() {}

//gather data from the serial
int *ServoProtocol::getData() {
  int *data = new int[7];

  // if correct header is received:
  if (Serial.read() == 0xcf && Serial.read() == 0xfc) {

    // waiting to receive all remaining data
    delay(10);
    int i = 0;
    while (Serial.available() && i < 7) {
      data[i] = Serial.read();
      i++;
    }
    if (i < 7) {
      delete[] data;
      return nullptr;
    }
  } else {
    delete[] data;
    return nullptr;
  }
  return data;
}

// interpret command from serial
int *ServoProtocol::read() {
  int *data = getData();
  if (data != nullptr) {

    // extract degrees and the checksum
    int *degrees = new int[6];
    int checkSum = data[6];
    for (int i = 0; i < 6; i++) {
      degrees[i] = data[i];
    }
    // free memory
    delete[] data;

    // check if data is consistent
    if (check(degrees, checkSum)) {
      return degrees;
    } else {
      delete[] degrees;
      return nullptr;
    }
  }
  return nullptr;
}

// check the check sum
bool ServoProtocol::check(int *data, int checkSum) {
  int s = sum(data);
  s %= 0xff;
  return s == checkSum;
}

int ServoProtocol::sum(int *arr) {
  int s = 0;
  for (int i = 0; i < 6; i++) {
    s += arr[i];
  }
  return s;
}
