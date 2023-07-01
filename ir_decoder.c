#include <wiringPi.h>
#include <iostream>

#define IRpin 4

#define MAXPULSE 65000
#define RESOLUTION 20

uint16_t pulses[100][2];
uint8_t currentpulse = 0;
char filename[100];
char commandname[100];
int flag=0;
void printPulses() {
    std::cout << "\nReceived: \nOFF \tON" << std::endl;
    
    for (uint8_t i = 0; i < currentpulse; i++) {
        std::cout << pulses[i][0] * RESOLUTION << " usec, " << pulses[i][1] * RESOLUTION << " usec" << std::endl;
    }

    std::cout << "int IRsignal[] = {" << std::endl;
    std::cout << "// ON, OFF " << std::endl;

    for (uint8_t i = 0; i < currentpulse - 1; i++) {
        std::cout << "pulseIR(" << pulses[i][1] * RESOLUTION << ");" << std::endl;
        std::cout << "delayMicroseconds(" << pulses[i + 1][0] * RESOLUTION << ");" << std::endl;
    }

    std::cout << "pulseIR(" << pulses[currentpulse - 1][1] * RESOLUTION << ");" << std::endl;
}
void setup() {
    wiringPiSetup();
	printf("Unesite ime uredjaja : ");
	scanf("%s", filename);
	printf("\nUnesite naziv komande : ");
	scanf("%s", commandname);
	printf("\n");
    std::cout << "Ready to decode IR!" << std::endl;
}


void printToFile(){
	FILE *file;
	file = fopen(filename, "a+");
    if (file == NULL){
        printf("Error opening the file.\n");
  //      return 1;
    }

	fprintf(file, "int %s[] = {\n",commandname);
    for (uint8_t i = 0; i < currentpulse - 1; i++) {
		fprintf(file, "pulseIR(%d);\n",pulses[i][1] * RESOLUTION);
		fprintf(file, "delayMicroseconds(%d);\n",pulses[i + 1][0] * RESOLUTION);
      //  *file << "pulseIR(" << pulses[i][1] * RESOLUTION << ");" << std::
    //    *file << "delayMicroseconds(" << pulses[i + 1][0] * RESOLUTION << ");" << std::endl;
    }
	
	fprintf(file, "pulseIR(%d);\n", pulses[currentpulse - 1][1] * RESOLUTION );
	fprintf(file,"}\n");
  //  *file << "pulseIR(" << pulses[currentpulse - 1][1] * RESOLUTION << ");" << std::endl;
    fclose(file);
}

void loop() {
    uint16_t highpulse, lowpulse;
    highpulse = lowpulse = 0;
    
    while (digitalRead(IRpin)) {
        highpulse++;
        delayMicroseconds(RESOLUTION);

        if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
			printToFile();
            printPulses();
            currentpulse = 0;
	    flag = 1;
            return;
        }
    }

    pulses[currentpulse][0] = highpulse;

    while (!digitalRead(IRpin)) {
        lowpulse++;
        delayMicroseconds(RESOLUTION);

        if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
	printToFile();
            printPulses();
            currentpulse = 0;
	    flag =1;
            return;
        }
    }

    pulses[currentpulse][1] = lowpulse;

    currentpulse++;
}


int main() {
    setup();

    while (flag == 0) {
        loop();
    }

    return 0;
}

