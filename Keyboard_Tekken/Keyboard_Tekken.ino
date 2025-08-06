#include <Joystick.h>

// Pin Kolom (output aktif LOW)
#define Kolom1 3
#define Kolom2 11
#define Kolom3 23
#define Kolom4 20

// Pin Baris (input pullup)
#define Baris1 6
#define Baris2 4
#define Baris3 5
#define Baris4 2
#define Baris5 21
#define Baris6 20
#define Baris7 22

// Joystick analog
#define Joy_X A0
#define Joy_Y A1

#define CENTER 512
#define THRESHOLD 200

// Objek joystick HID
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   32, 0,                  // 32 tombol, 0 axis tambahan
                   true, true, false,     // x, y axis analog
                   false, false, false,   // z, rx, ry
                   false, false,          // rudder, throttle
                   false, false, false);  // hat

// Array pin
const int kolomPins[4] = {Kolom1, Kolom2, Kolom3, Kolom4};
const int barisPins[7] = {Baris1, Baris2, Baris3, Baris4, Baris5, Baris6, Baris7};

// Pemetaan tombol ke koordinat [kolom][baris] = nomor tombol (0-based untuk Joystick)
struct Tombol {
  int kolom;
  int baris;
  int index; // nomor tombol untuk Joystick.setButton
};

Tombol tombolList[] = {
  {1, 1, 0},   // 1
  {1, 2, 1},   // 2
  {4, 1, 2},   // 3
  {1, 2, 3},   // 4
  {2, 2, 4},   // 5
  {3, 2, 5},   // 6
  {1, 3, 6},   // 7
  {2, 3, 7},   // 8
  {3, 3, 8},   // 9
  {4, 3, 9},   // 10
  {1, 4, 10},  // 11
  {2, 4, 11},  // 12
  {3, 4, 12},  // 13
  {4, 4, 13},  // 14
  {4, 5, 14},  // 15
  {3, 6, 15},  // 16
  {5, 6, 16}   // 17
};

void settingPin() {
  // Setup pin kolom sebagai output HIGH (inactive)
  for (int i = 0; i < 4; i++) {
    pinMode(kolomPins[i], OUTPUT);
    digitalWrite(kolomPins[i], HIGH);
  }

  // Setup pin baris sebagai input pull-up
  for (int i = 0; i < 7; i++) {
    pinMode(barisPins[i], INPUT_PULLUP);
  }

  // Setup analog
  pinMode(Joy_X, INPUT);
  pinMode(Joy_Y, INPUT);
}

void setup() {
  settingPin();
  Joystick.begin();
}

void loop() {
  // Scan semua tombol
  for (int c = 0; c < 4; c++) {
    digitalWrite(kolomPins[c], LOW); // aktifkan kolom

    delayMicroseconds(10); // debounce singkat

    for (int r = 0; r < 7; r++) {
      bool pressed = (digitalRead(barisPins[r]) == LOW); // LOW berarti tombol ditekan

      // Cek apakah tombol ini ada dalam daftar tombolList
      for (int i = 0; i < sizeof(tombolList) / sizeof(tombolList[0]); i++) {
        if ((tombolList[i].kolom - 1) == c && (tombolList[i].baris - 1) == r) {
          Joystick.setButton(tombolList[i].index, pressed);
        }
      }
    }

    digitalWrite(kolomPins[c], HIGH); // nonaktifkan kolom
  }

  // Baca Joystick analog
  int x = analogRead(Joy_X);
  int y = analogRead(Joy_Y);

  // Normalisasi dan kirim
  Joystick.setXAxis(x);
  Joystick.setYAxis(y);

  delay(10); // debounce global
}
