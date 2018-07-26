#define STX 0x02
#define ETX 0x03
#define SPACE 0x20

void setup() {
	Serial3.begin(9600);
	delay(1000);
}

void loop() {
	//message to change pressure (for specific channel)
	//format: stx 0EPH__CHXXXPXXXX?? etx
	//Channel: 0-399
	//20.0[kPa]-800.0[kPa]
    //Set channel 100 to 400.0[kPa]

	for(int i=0;i<4;i++){
		int pressure = (i+1)*100;
		String pressure_string = String(pressure);
		String base_command = "0EPH  CH100P";
		String p = base_command+pressure_string;
		// String p = "4000";  		
		// String p = "04DI  ";  //Test command
		sendMessageBytes(p);
		delay(1000);
	}
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
	byte ubyte = (result & 0b11110000) >> 4;
	byte lbyte = result & 0b00001111;

	//Here we send a message
	//STX
    Serial3.write(STX); 
	//Payload
    Serial3.write(buf,string_length); //do not send null terminator
    //Checksum
	Serial3.write(ubyte);
	Serial3.write(lbyte);
	//ETX
    Serial3.write(ETX); 

}

