#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27   // alamat I2C (ubah jika scanner berbeda)

LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);
Servo myServo;

// Pin
const int ledOpen = 2;        // LED hijau (OPEN)
const int ledClose = 3;       // LED merah (CLOSED)
const int buttonOpen = 4;     // Tombol buka tirai -> ke GND
const int buttonClose = 5;    // Tombol tutup tirai -> ke GND
const int servoPin = 9;       // Servo

// Debounce
unsigned long lastDebounceOpen = 0;
unsigned long lastDebounceClose = 0;
const unsigned long debounceDelay = 80;

bool lastOpenReading = HIGH;
bool lastCloseReading = HIGH;

void setup() {
  Serial.begin(9600);     // ➜ AKTIFKAN SERIAL MONITOR
  Serial.println("System Starting...");
  Serial.println("Smart Curtain Ready!");

  // Tombol
  pinMode(buttonOpen, INPUT_PULLUP);
  pinMode(buttonClose, INPUT_PULLUP);

  // LED
  pinMode(ledOpen, OUTPUT);
  pinMode(ledClose, OUTPUT);

  // Servo
  myServo.attach(servoPin);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Curtain");
  lcd.setCursor(0, 1);
  lcd.print("Status: CLOSED");

  // Posisi awal tertutup
  myServo.write(0);
  digitalWrite(ledOpen, LOW);
  digitalWrite(ledClose, HIGH);

  Serial.println("Initial State: CLOSED");
}

void loop() {
  bool openReading = digitalRead(buttonOpen);
  bool closeReading = digitalRead(buttonClose);

  // =================== HANDLE OPEN ===================
  if (openReading != lastOpenReading) {
    lastDebounceOpen = millis();
  }

  if ((millis() - lastDebounceOpen) > debounceDelay) {
    if (openReading == LOW) {
      Serial.println("Button OPEN pressed");
      openCurtain();
    }
  }

  lastOpenReading = openReading;

  // =================== HANDLE CLOSE ===================
  if (closeReading != lastCloseReading) {
    lastDebounceClose = millis();
  }

  if ((millis() - lastDebounceClose) > debounceDelay) {
    if (closeReading == LOW) {
      Serial.println("Button CLOSE pressed");
      closeCurtain();
    }
  }

  lastCloseReading = closeReading;
}

// ====================== FUNGSI BUKA =====================
void openCurtain() {
  Serial.println("Opening Curtain...");
  myServo.write(90);

  digitalWrite(ledOpen, HIGH);
  digitalWrite(ledClose, LOW);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Curtain Status:");
  lcd.setCursor(0, 1); lcd.print("OPEN");

  Serial.println("Status: OPEN");
  delay(200);
}

// ====================== FUNGSI TUTUP ====================
void closeCurtain() {
  Serial.println("Closing Curtain...");
  myServo.write(0);

  digitalWrite(ledOpen, LOW);
  digitalWrite(ledClose, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Curtain Status:");
  lcd.setCursor(0, 1); lcd.print("CLOSED");

  Serial.println("Status: CLOSED");
  delay(200);
}
