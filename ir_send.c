#include <wiringPi.h>
#include <iostream>

#define IRledPin 13
char filename[100];
char commandname[100];
uint16_t send_command[1000][2];
uint16_t num_of_puleses=0;
void setup() {

    wiringPiSetup();
    pinMode(IRledPin, OUTPUT);
    digitalWrite(IRledPin, LOW);
    delay(1000); // Delay for stability
    printf("Unesite ime uredjaja : ");
    scanf("%s", filename);
    printf("Unesite naziv komande : ")
    scanf("%s", commandname);
    std::cout << "Ready to send IR signal" << std::endl;
}
void decodeCommand() {
    FILE *device;
    char line[100];
    char read_command[100];
    char temp[100];
    int value;
    int i =0;
    bool command_found = false;
    device = fopen(filename,"r");
    if (filePointer == NULL) {
        printf("File could not be opened.");
    }

    while (fscanf(filePointer, "%s", line) != EOF) {
        
        if(!strcmp(commandname, '}'))
        {
            command_found = false;
        }

        if(command_found){
            sscanf(line,"%s(%d);", temp,value);
            if(!strcmp(temp, "pulseIR"))
            {
                send_command[i][0] = value;
            }
            elsif(!strcmp(temp, "delayMicroseconds"))
            {
                send_command[i][1] = value;
                i++;
                num_of_puleses++;
            }
            
        }
        
        sscanf(line,"int %s[] = {", read_command);
        if(!strcmp(commandname, read_command))
            command_found = true;                
    }
}
void sendCommand(){
    decodeCommand();
    for (int i = 0; i < num_of_puleses-1; ++i)
    {
        pulseIR(send_command[i][0]);
        delayMicroseconds(send_command[i][1]);
    }
    pulseIR(send_command[num_of_puleses-1][0]);
        
}
void loop() {
    std::cout << "Sending IR signal" << std::endl;
    sendCommand();

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



int main() {
    setup();

   // while (true) {
        loop();
    //}

    return 0;
}

