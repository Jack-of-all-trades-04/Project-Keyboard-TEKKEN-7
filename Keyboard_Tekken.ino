#include <Joystick.h>

// Tombol arah
#define Button_U 2
#define Button_F 3
#define Button_D 4
#define Button_B 5

// Tombol aksi
#define Button_1 6
#define Button_2 7
#define Button_3 8
#define Button_4 9

// Tombol Mode
#define OnOff 10
#define OnOffJoystick 11

// Joystick analog
#define Joy_X A0
#define Joy_Y A1

// Ambang batas analog
#define CENTER 512
#define THRESHOLD 200

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   32, 0,                  
                   false, false, false,    
                   false, false, false,
                   false, false,
                   false, false, false);


bool button_status[8] = {0};

void settingPin() {
  pinMode(Button_U, INPUT_PULLUP);
  pinMode(Button_F, INPUT_PULLUP);
  pinMode(Button_D, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(Button_2, INPUT_PULLUP);
  pinMode(Button_3, INPUT_PULLUP);
  pinMode(Button_4, INPUT_PULLUP);
  pinMode(OnOff, INPUT_PULLUP);
  pinMode(OnOffJoystick, INPUT_PULLUP);
  pinMode(Joy_X, INPUT);
  pinMode(Joy_Y, INPUT);
}

void setup() {
  settingPin();
  Joystick.begin();
}

void loop() {
  bool aktif = digitalRead(OnOff);
  bool joy = digitalRead(OnOffJoystick);
  bool arah[4] = {false, false, false, false}; // UP, RIGHT, DOWN, LEFT
  
  if (aktif == LOW) {
    // Input dari Tombol 
    if (joy == HIGH) {
    arah[0] = digitalRead(Button_U);
    arah[1] = digitalRead(Button_F);
    arah[2] = digitalRead(Button_D);
    arah[3] = digitalRead(Button_B);
    }
    
    else if (joy == LOW) {
      // Input dari Joystick Analog
      int x = analogRead(Joy_X);
      int y = analogRead(Joy_Y);

      if (y < CENTER - THRESHOLD) arah[0] = true; // UP
      if (y > CENTER + THRESHOLD) arah[2] = true; // DOWN
      if (x > CENTER + THRESHOLD) arah[1] = true; // RIGHT
      if (x < CENTER - THRESHOLD) arah[3] = true; // LEFT
    }

    // Kirim ke Joystick (tombol 0-3)
    for (int i = 0; i < 4; i++) {
      if (button_status[i] != arah[i]) {
        button_status[i] = arah[i];
        Joystick.setButton(i, arah[i]);
      }
    }

    // Tombol Aksi (4–7)
    for (int i = 0; i < 4; i++) {
      int pin = i + 6; // Button_1 = pin 6
      bool pressed = digitalRead(pin);
      if (button_status[i + 4] != pressed) {
        button_status[i + 4] = pressed;
        Joystick.setButton(i + 4, pressed);
      }
    }

  } else {
    // OFF: Matikan semua tombol jika sebelumnya menyala
    for (int i = 0; i < 8; i++) {
      if (button_status[i]) {
        button_status[i] = false;
        Joystick.setButton(i, 0);
      }
    }
  }
  delay(5);
}
