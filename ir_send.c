#include <wiringPi.h>
#include <iostream>
#include <string.h>
#define IRledPin 1 
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
    printf("Unesite naziv komande : ");
    scanf("%s", commandname);
    std::cout << "Ready to send IR signal" << std::endl;
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

void decodeCommand() {
    FILE *device;
    char line[100];
    char read_command[100];
    char temp[100];
    uint16_t value=0;
    int i =0;
    bool command_found = false;
    device = fopen(filename,"r");
    if (device == NULL) {
        printf("File could not be opened.");
    }
    //printf("Krecem citanje iz fajla\n");
    while (fgets(line,sizeof(line),device) != NULL) {
        //printf("Procitao sam iz fajla : %s\n",line);
        if(!strcmp(line, "}"))
        {
            command_found = false;
	    break;
        }

        if(command_found){
            sscanf(line,"%[^(]", temp);
	    //printf("command: %s\n",temp);
            if(!strcmp(temp, "pulseIR"))
            {
		sscanf(line,"pulseIR(%d);", &value);
	    //printf("value: %d\n",value);
                send_command[i][0] = value;
            }
	    else if(!strcmp(temp, "delayMicroseconds"))
            {
		sscanf(line,"delayMicroseconds(%d);", &value);
	    //printf("value: %d\n",value);
                send_command[i][1] = value;
                i++;
                num_of_puleses++;
            }
            
        }
        
        sscanf(line,"int %[^[]", read_command);
        if(!strcmp(commandname, read_command))
        {
	     command_found = true;
	     printf("USPESNO PRONASAO KOMANDU : %s\n",read_command);
	}		
	else
	     command_found = false;
    }
}
void sendCommand(){
    decodeCommand();
    
   // printf("num of pulses %d\n", num_of_puleses);
    for (int i = 0; i < num_of_puleses-1; ++i)
    {
	//printf("pulseIR(%d)\n",send_command[i][0]);
	//printf("delayMicroseconds(%d)\n",send_command[i][1]);
        pulseIR(send_command[i][0]);
        delayMicroseconds(send_command[i][1]);
    }
    pulseIR(send_command[num_of_puleses-1][0]);
    printf("IR SIGNAL SENT!\n");    
}
void loop() {
    std::cout << "Sending IR signal" << std::endl;
    sendCommand();

}



int main() {
    setup();

   // while (true) {
        loop();
    //}

    return 0;
}

