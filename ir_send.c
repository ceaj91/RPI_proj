#include <wiringPi.h>
#include <iostream>

#define IRledPin 13

void setup() {
    std::cout << "Ready to send IR signal" << std::endl;
    wiringPiSetup();
    pinMode(IRledPin, OUTPUT);
    digitalWrite(IRledPin, LOW);
    delay(1000); // Delay for stability
}

void loop() {
    std::cout << "Sending IR signal" << std::endl;
    sendNikonCode();
    delay(60 * 1000); // wait one minute (60 seconds * 1000 milliseconds)
}

void pulseIR(long microsecs) {
    while (microsecs > 0) {
        digitalWrite(IRledPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(IRledPin, LOW);
        delayMicroseconds(10);
        microsecs -= 26;
    }
}

void sendNikonCode() {
    pulseIR(2080);
    delay(27);
    pulseIR(440);
    delayMicroseconds(1500);
    pulseIR(460);
    delayMicroseconds(3440);
    pulseIR(480);

    delay(65); // wait 65 milliseconds before sending it again

    pulseIR(2000);
    delay(27);
    pulseIR(440);
    delayMicroseconds(1500);
    pulseIR(460);
    delayMicroseconds(3440);
    pulseIR(480);
}

int main() {
    setup();

    while (true) {
        loop();
    }

    return 0;
}

