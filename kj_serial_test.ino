#define STX 0x02
#define ETX 0x03
#define SPACE 0x20

void setup() {
	Serial3.begin(9600);
	delay(1000);
}

void loop() {
	createMessageBytes();
	delay(1000);
}

void createMessageBytes(){
	//message to change pressure (for specific channel)
	//format: stx 0EPH__CHXXXPXXXX?? etx
	//Channel: 0-399
	//20.0[kPa]-800.0[kPa]

	// String p = "0EPH  CH100P4000";  //Set channel 100 to 400.0[kPa]
	String p = "04DI  ";  //Test command
	int string_length = p.length();
	char buf[string_length+1]; //length+1 (null terminator \0=0x00 in ASCII)
	p.toCharArray(buf,string_length+1);

	computeCheckSum(buf,string_length);

	// Serial3.write(buf,string_length); //do not send null terminator
	delay(1000);
}

void computeCheckSum(char *buf, int string_length){
	byte result = 0x00;
	for (int i=0;i<string_length;i++){
		result = result - buf[i];
	}
	// byte result = 0x00 - 0x30 - 0x34 - 0x44 - 0x49 - 0x20 - 0x20; //-305->two's complement of 305->0b011001111->0xCF
	byte ubyte = (result & 0b11110000) >> 4;
	byte lbyte = result & 0b00001111;
	Serial3.write(ubyte);
	Serial3.write(lbyte);

}











