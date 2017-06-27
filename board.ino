//This part of codes are provided by Pan Bowen

#define LEFT_AHEAD 10 
#define LEFT_BACK 9 
#define RIGHT_AHEAD 13
#define RIGHT_BACK 12 
#define STEER 14
int duty = 500; 
long int n;
String inString=""; 
float RunInit = 3.0; 
float RUN_LEFT = 3.0;

float RUN_RIGHT = 3.0;
 
int TURN = 0;
int RUN2=55;
int RUN1 = 50;
int CYC = 5;
float calibrateNum = 0; void stopBack();
void turnLeft(); void turnRight(); 
void goAhead(); 
void park(); 
void goBack();  
void setup()
{
	Serial.begin(9600); 
	pinMode(LEFT_AHEAD,OUTPUT); 
	pinMode(LEFT_BACK, OUTPUT); 
	pinMode(RIGHT_AHEAD, OUTPUT); 
	pinMode(RIGHT_BACK, OUTPUT); 
	digitalWrite(LEFT_AHEAD, LOW); 
	digitalWrite(LEFT_BACK, LOW); 
	digitalWrite(RIGHT_AHEAD, LOW); 
	digitalWrite(RIGHT_BACK, LOW);
	pinMode(STEER, OUTPUT); 
	digitalWrite(STEER, LOW);
	for (int i=0; i<100; i++) 
	{
	    digitalWrite(STEER,LOW); 
	    delayMicroseconds(duty); 
	    digitalWrite(STEER,HIGH); 
		delayMicroseconds(10000-duty); 
		delayMicroseconds(6550);
	}
} 
char incomingByte = ' ';

void loop()
{       n=(n+1)%200000;
	if (Serial.available() > 0) 
	{
		// read the incoming byte: 
		incomingByte = Serial.read(); 
		if (incomingByte == 'L') {
			Serial.println("TURN LEFT"); 
			turnLeft();
			Serial.read();
		} else if (incomingByte == 'R')
		{ 
			Serial.println("TURN RIGHT"); 
			turnRight();
			Serial.read();
		} else if (incomingByte == 'A')
		{ 
			Serial.println("GO AHEAD"); 
			goAhead();
			Serial.read();
		} else if (incomingByte == 'B')
		{ 
			Serial.println("GO BACK"); 
			goBack();
			Serial.read();
		} else if (incomingByte == 'P')
		{
			Serial.println("PARK");
			park();
			Serial.read();
		} else if (incomingByte == 'C')
		{ 
			calib();
		} else if (incomingByte == 'K')
		{ 
			changeP();
		} else if (incomingByte == 'S')
		{ 
			changeS();
		} else if (incomingByte == 'D')
		{ 
			changeD();
		} 
	}
if(n==25000) park();
} 
void steer(float x)
{ 
	int tmp = x*duty;
	for (int i=0; i<CYC; i++) 
	{
		digitalWrite(STEER,LOW); 
		delayMicroseconds(tmp); 
		digitalWrite(STEER,HIGH); 
		delayMicroseconds(1000-tmp); 
		delayMicroseconds(6550);
                delayMicroseconds(100);
 	}
	 digitalWrite(STEER,LOW);
} 

void goAhead()
{
	stopBack(); 
	analogWrite(LEFT_AHEAD,50);
	analogWrite(RIGHT_AHEAD,40);
	steer(1.0);
        n=0;
        
} 
void turnLeft()
{
	stopBack(); 
	analogWrite(RIGHT_AHEAD,70);
	analogWrite(LEFT_BACK,20);
	steer(1.1);
        n=23500;      
        
} 
void turnRight()
{
	stopBack(); 
	analogWrite(LEFT_AHEAD,70);
	analogWrite(RIGHT_BACK,15);
	steer(0.88);
        n=23500;
} 
void park()
{
	stopBack(); 
	digitalWrite(LEFT_AHEAD, LOW); 
	digitalWrite(RIGHT_AHEAD, LOW); 
	steer(1);
}

void goBack()
{
	// stopBack();
	digitalWrite(LEFT_AHEAD,LOW); 
	digitalWrite(RIGHT_AHEAD,LOW); 
	analogWrite(LEFT_BACK,RUN2);
        analogWrite(RIGHT_BACK,RUN1);
	steer(1);
        n=-5000;
} 

void stopBack()
{ 
	digitalWrite(LEFT_BACK, LOW); 
	digitalWrite(RIGHT_BACK, LOW);
}

int readInt()
{
	int x;
	int flag=1; 
	while(flag)
	{
		while (Serial.available() > 0) 
		{ 
			int inChar = Serial.read();
			if (isDigit(inChar)) 
			{
				// convert the incoming byte to a char 
				// and add it to the string:
				inString += (char)inChar;
			}
			// if you get a newline, print the string, 
			// then the string's value:
			if (inChar == '\n') 
			{
				x=inString.toInt();
				flag=0;
				// clear the string for new input: inString = "";
			} 
		}
	}
	return x; 
}

void calib()
{
	TURN = readInt();
	Serial.println("calibrate:");
	Serial.println(TURN,DEC); 
}
void changeP()
{
	CYC = readInt(); 
	Serial.println("DELAY(ms):"); 
	Serial.println(CYC*2,DEC);
} 

void changeS()
{
	RUN1 = readInt(); 
	Serial.println("new spead:"); 
	Serial.println(RUN1,DEC);
} 
void changeD()
{
	duty = readInt(); 
	Serial.println("new duty:"); 
	Serial.println(duty,DEC);
} 

