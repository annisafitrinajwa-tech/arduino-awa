#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define BUTTON_START 9
#define BUTTON_RESET 8
#define POT A2

int leds[] = {LED1, LED2, LED3, LED4};
int indexLED = 0;
bool running = false;

void setup() {
  for (int i = 0; i < 4; i++) pinMode(leds[i], OUTPUT);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);

 Serial.begin(9600);
 Serial.println("Menjejak Cahaya: Rangkaian LED Berurutan Sederhana");
  Serial.println("Tekan tombol START untuk memulai LED berjalan.");
  Serial.println("------------------------------------------");
}

void loop() {
  int potValue = analogRead(POT);
  int delayTime = map(potValue, 0, 1023, 200, 1000);

  // Tombol Start/Stop
  if (digitalRead(BUTTON_START) == LOW) {
    running = !running;
    Serial.print("STATUS: ");
    if (running) Serial.println("RUNNING");
    else Serial.println("PAUSE");
    delay(300); // debounce
  }

  // Tombol Reset
  if (digitalRead(BUTTON_RESET) == LOW) {
    indexLED = 0;
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
    Serial.println("RESET DITEKAN - Semua LED dimatikan & kembali ke LED pertama.");
    delay(300); // debounce
  }

  // Jalankan LED jika running
  if (running) {
    Serial.print("LED Menyala: LED ");
    Serial.println(indexLED + 1);

    Serial.print("Kecepatan (ms): ");
    Serial.println(delayTime);

    digitalWrite(leds[indexLED], HIGH);
    delay(delayTime);
    digitalWrite(leds[indexLED], LOW);

    indexLED++;
    if (indexLED > 3) indexLED = 0;
  }
}

