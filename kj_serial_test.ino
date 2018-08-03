#include <SoftwareSerial.h>
#define STX 0x02
#define ETX 0x03
#define SPACE 0x20
#define ENQ 0x05
#define ACK 0x06
#define EOT 0x04

SoftwareSerial mySerial(6, 7); //232_TX,232_RX

void setup() {
	Serial.begin(19200);
	// Serial3.begin(9600);
	mySerial.begin(19200);
	delay(1000);

	// String base_command = "04DI  ";
	// sendMessageBytes(base_command);
	// String base_command = "0EPH  CH001P2000";
	// sendMessageBytes(base_command);
}

void loop() {
	//message to change pressure (for specific channel)
	//format: stx 0EPH__CHXXXPXXXX?? etx
	//Channel: 0-399
	//20.0[kPa]-800.0[kPa]
    //Set channel 1 to 400.0[kPa]
	
	for(int i=0;i<4;i++){
		int pressure = (i+1)*1000;
		String pressure_string = String(pressure);
		String base_command = "0EPH  CH001P";
		String p = base_command+pressure_string;
		sendMessageBytes(p);
		delay(10);
	}
	
	// delay(1000);
}

void sendMessageBytes(String command_and_data){
	int string_length = command_and_data.length();
	char buf[string_length+1]; //length+1 (null terminator \0=0x00 in ASCII)
	command_and_data.toCharArray(buf,string_length+1);

	byte result = 0x00;
	for (int i=0;i<string_length;i++){
		result = result - buf[i];
	}
	// byte result = 0x00 - 0x30 - 0x34 - 0x44 - 0x49 - 0x20 - 0x20; //-305->two's complement of 305->0b011001111->0xCF
	byte ubyte = ((result & 0b11110000) >> 4) & 0b11111111;
	byte lbyte = result & 0b00001111;

	char ubyteChar[2];
	char lbyteChar[2];
	sprintf(ubyteChar,"%x",ubyte);
	sprintf(lbyteChar,"%x",lbyte);

	mySerial.write(ENQ);
	delayMicroseconds(100);
    mySerial.write(STX); 
    mySerial.write(buf,string_length); //do not send null terminator
  	mySerial.print(ubyteChar[0]);
  	mySerial.print(lbyteChar[0]);
    mySerial.write(ETX); 
	delayMicroseconds(100);
	mySerial.write(EOT);
}
