🔥 Flammable Gas Detector

A Flammable Gas Detection System built using an MQ-2 Gas Sensor and Arduino to detect hazardous gases such as LPG, methane, propane, hydrogen, alcohol vapors, and smoke. The system alerts users through a buzzer and LED indicator when dangerous gas concentrations are detected.

This project is useful for home safety, kitchens, laboratories, and industrial environments to prevent accidents caused by gas leakage.

📌 Project Overview

Gas leakage is one of the most dangerous hazards in homes and industries. If combustible gas accumulates in a closed environment, it can cause fires or explosions when exposed to ignition sources.

The MQ-2 gas sensor detects various gases including LPG, methane, propane, hydrogen, alcohol vapor, and smoke. It works by detecting changes in electrical resistance when these gases are present in the air.

When the detected gas concentration crosses a threshold, the system activates an alarm using a buzzer and LED indicator.

🎯 Features

Detects LPG, Methane, Propane, Hydrogen, Smoke

Real-time gas monitoring

Automatic alarm (buzzer + LED)

Low cost and easy to build

Suitable for home and industrial safety systems

🧰 Components Required
Component	Quantity
Arduino UNO / Nano	1
MQ-2 Gas Sensor	1
Buzzer	1
LED	1
220Ω Resistor	1
Breadboard	1
Jumper Wires	Several
USB Cable	1

The MQ-2 sensor operates on 5V power and can detect gases between roughly 200–10000 ppm concentration.

⚡ Circuit Diagram

(Add your circuit image in the repo and link it here)

MQ2 Sensor       Arduino
---------        -------
VCC  ----------> 5V
GND  ----------> GND
AO   ----------> A0

LED  ----------> Pin 13 (via resistor)
Buzzer --------> Pin 8
Connection Explanation
MQ-2 Pin	Arduino Pin
VCC	5V
GND	GND
AO (Analog Output)	A0

The sensor outputs a voltage proportional to the gas concentration; higher gas levels produce higher output voltage.

🧠 Working Principle

The MQ-2 sensor continuously monitors the air for combustible gases.

The sensor outputs an analog voltage signal depending on gas concentration.

Arduino reads this value through the A0 analog pin.

If the value exceeds a preset threshold:

The buzzer turns ON

The LED lights up

This alerts users about a potential gas leak.

The MQ-2 sensor works based on changes in resistance of a sensing material when exposed to gases.

💻 Arduino Code
int gasSensor = A0;
int buzzer = 8;
int led = 13;

int threshold = 400;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  int gasValue = analogRead(gasSensor);
  Serial.println(gasValue);

  if (gasValue > threshold) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }

  delay(500);
}
📊 Project Structure
Flamable-Gas-Detector
│
├── code
│   └── gas_detector.ino
│
├── images
│   ├── circuit_diagram.png
│   └── prototype.jpg
│
├── README.md
🚀 Installation & Usage
1️⃣ Clone the repository
git clone https://github.com/hsay123/Flamable-Gas-Detector.git
2️⃣ Open the project

Open the .ino file using Arduino IDE.

3️⃣ Upload the code

Select your Arduino board and upload the program.

4️⃣ Test the system

Expose the sensor to gas (for example lighter gas without flame).
The buzzer and LED will activate when gas is detected.

📸 Demo / Prototype

Add screenshots here:

/images/prototype.jpg
/images/circuit_diagram.png
🔮 Future Improvements

IoT gas monitoring using ESP8266 / ESP32

Mobile notification system

LCD display for gas concentration

GSM module for SMS alerts

Smart home integration

📚 Applications

Home kitchen gas leak detection

Industrial safety systems

Laboratory gas monitoring

Smart home automation

Fire prevention systems
