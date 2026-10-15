// AD8232 -> A0, LO+ -> 10, LO- -> 11, buzzer -> 8
// AD8232 runs on 3.3V not 5V

int buzzer = 8;
int flip = -1;   // change to 1 if the spikes point down

float base = 0;
bool started = false;
float peak = 10;
unsigned long lastBeat = 0;
bool above = false;
unsigned long lastSample = 0;

int lastVal = 0;
float jumpy = 0;
unsigned long offUntil = 0;

int a[5];
float b[4];
float c[5];
int ai = 0, bi = 0, ci = 0;

float x1 = 0, x2 = 0;

unsigned long gaps[4];
int gi = 0;
int gapCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // 250 readings a second
  if (micros() - lastSample < 4000) return;
  lastSample = micros();

  int val = analogRead(A0);

  jumpy = jumpy * 0.98 + abs(val - lastVal) * 0.02;
  lastVal = val;

  // pad off or too noisy
  if (digitalRead(10) == 1 || digitalRead(11) == 1 || jumpy > 30) {
    offUntil = millis() + 1000;
  }
  if (millis() < offUntil) {
    Serial.println("L");
    gapCount = 0;
    return;
  }

  // smoothing
  a[ai] = val;  ai = (ai + 1) % 5;
  float s1 = (a[0] + a[1] + a[2] + a[3] + a[4]) / 5.0;
  b[bi] = s1;   bi = (bi + 1) % 4;
  float s2 = (b[0] + b[1] + b[2] + b[3]) / 4.0;
  c[ci] = s2;   ci = (ci + 1) % 5;
  float smooth = (c[0] + c[1] + c[2] + c[3] + c[4]) / 5.0;

  // remove drift
  if (!started) { base = smooth; started = true; }
  base = base + (smooth - base) * 0.02;
  float x = (smooth - base) * flip;

  float steep = abs(x - x2);
  x2 = x1;
  x1 = x;

  peak = peak * 0.998;
  if (steep > peak) peak = steep;
  if (peak < 10) peak = 10;

  if (!above && steep > peak * 0.5 && millis() - lastBeat > 330) {
    above = true;
    unsigned long gap = millis() - lastBeat;
    lastBeat = millis();

    tone(buzzer, 1000, 60);

    if (gap < 2000) {
      gaps[gi] = gap;
      gi = (gi + 1) % 4;
      if (gapCount < 4) gapCount++;

      unsigned long total = 0;
      for (int i = 0; i < gapCount; i++) total += gaps[i];
      int bpm = 60000 / (total / gapCount);
      Serial.print("B,");
      Serial.println(bpm);
    }
  }
  if (steep < peak * 0.2) above = false;

  Serial.println((int)x);
}
