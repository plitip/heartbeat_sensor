# ecg monitor

heart monitor i made with an AD8232 and an arduino uno. it shows the heartbeat on a green screen like the ones in hospitals and beeps on every beat. if a pad comes off it goes flat and does the long beep.

video: demo.mov

## parts
- AD8232 ecg sensor + the 3 pads
- arduino uno r3
- passive buzzer
- jumper wires

## wiring
| AD8232 | arduino |
|---|---|
| GND | GND |
| 3.3V | 3.3V |
| OUTPUT | A0 |
| LO- | 11 |
| LO+ | 10 |

buzzer + goes to pin 8, - goes to GND

## how to run
1. upload ecg_monitor.ino to the arduino
2. close the arduino ide
3. open ecg_monitor.html in chrome and press connect

pads go red on right chest, yellow on left chest, green on lower right belly. sit still or it gets noisy.

not a real medical device obviously
