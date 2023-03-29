/*
  4164-4256 DRAM TESTER
  https://andy.od.ua
  2018
  Version 1.0
*/

#define ADDRES_A0 A0 //chip 4164 address A0-A7
#define ADDRES_A1 A1
#define ADDRES_A2 A2
#define ADDRES_A3 A3
#define ADDRES_A4 A4
#define ADDRES_A5 A5
#define ADDRES_A6 10
#define ADDRES_A7 11
#define ADDRES_A8 12 //4256

#define RAS         4 // chip RAS
#define CAS         5 // chip CAS
#define D           6 // chip data IN
#define Q           8 // chip data OUT
#define WRITE       9 // chip write
#define STATUS_LED  7
#define STATUS_ERROR_LED 3

#define ROW_RANGE 512 //row range 255 - 4164  512 - 4256
#define COL_RANGE 512

void setup() {
  Serial.begin(9600);
  pinMode(D, OUTPUT);
  pinMode(Q, INPUT);
  pinMode(ADDRES_A0, OUTPUT);
  pinMode(ADDRES_A1, OUTPUT);
  pinMode(ADDRES_A2, OUTPUT);
  pinMode(ADDRES_A3, OUTPUT);
  pinMode(ADDRES_A4, OUTPUT);
  pinMode(ADDRES_A5, OUTPUT);
  pinMode(ADDRES_A6, OUTPUT);
  pinMode(ADDRES_A7, OUTPUT);
  pinMode(ADDRES_A8, OUTPUT);
  pinMode(RAS, OUTPUT);
  pinMode(CAS, OUTPUT);
  pinMode(WRITE, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(STATUS_ERROR_LED, OUTPUT);
}

void loop() {
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(STATUS_ERROR_LED, HIGH);
  Serial.println("RAM TESTER V.1.0");
  Serial.println("Start RAM test....");
  fillOne();
  fillZero();
  Serial.println("RAM test passed!");
  while (1) {
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);
    delay(500);
  }
}

void fillOne() {
  Serial.println("Starting test... fill one");
  for (int row = 0; row <= ROW_RANGE; row++) {
    writeBits(row, 1);
    int BitNumber = readBits(row);
    if (BitNumber != (ROW_RANGE+1)) {
      digitalWrite(STATUS_ERROR_LED, LOW);
      Serial.println("ERROR: row " + String(row) + " number of bits was: " + String(BitNumber)) + ", but should be 255.";
      while (1);
    }
  }
}
void fillZero() {
  Serial.println("Starting test... fill zero");
  for (int row = 0; row <= ROW_RANGE; row++) {
    writeBits(row, 0);
    int BitNumber = readBits(row);
    if (BitNumber > 0) {
      digitalWrite(STATUS_ERROR_LED, LOW);
      Serial.println("ERROR: row " + String(row) + " number of bits was: " + String(BitNumber)) + ", but should be 0.";
      while (1);
    }
  }
}

void setAddress(int address) {
  digitalWrite(ADDRES_A0, bitRead(address, 0));
  digitalWrite(ADDRES_A1, bitRead(address, 1));
  digitalWrite(ADDRES_A2, bitRead(address, 2));
  digitalWrite(ADDRES_A3, bitRead(address, 3));
  digitalWrite(ADDRES_A4, bitRead(address, 4));
  digitalWrite(ADDRES_A5, bitRead(address, 5));
  digitalWrite(ADDRES_A6, bitRead(address, 6));
  digitalWrite(ADDRES_A7, bitRead(address, 7));
  if(ROW_RANGE == 512){
    digitalWrite(ADDRES_A8, bitRead(address, 8));
  }
}

int writeBits(int row, int bits) {
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  for (int i = 0; i <= COL_RANGE; i++) {
    setAddress(row);
    digitalWrite(RAS, LOW);
    digitalWrite(WRITE, LOW);
    digitalWrite(D, bits);
    setAddress(i);
    digitalWrite(CAS, LOW);
    digitalWrite(RAS, HIGH);
    digitalWrite(CAS, HIGH);
  }
}

int readBits(int row) {
  int BitNumber = 0;
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  digitalWrite(WRITE, HIGH);
  for (int i = 0; i <= COL_RANGE; i++) {
    setAddress(row);
    digitalWrite(RAS, LOW);
    setAddress(i);
    digitalWrite(CAS, LOW);
    BitNumber += digitalRead(Q);
    digitalWrite(RAS, HIGH);
    digitalWrite(CAS, HIGH);
  }
  return BitNumber;
}
