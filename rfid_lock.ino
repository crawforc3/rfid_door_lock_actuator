#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Card UID: 80 55 71 46
// Card UID: 07 93 89 AB
String read_rfid;
String ok_rfid_1="80557146";
String ok_rfid_2="79389ab";
int lock_pin = 7;
int servo_pin = 8;
int pos = 0;

Servo myservo;                     // Create servo object to control a servo
void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	pinMode(lock_pin, OUTPUT);      // Sets the lock_LED

	myservo.attach(servo_pin);      // attaches the servo on pin 8 to the servo object
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid = read_rfid + String(buffer[i], HEX);
    }
}

void open_lock() {
    digitalWrite(lock_pin, HIGH);
    for (pos = 20; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15ms for the servo to reach the position
    }
    
    delay(2000);
    
    for (pos = 170; pos >= 20; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15ms for the servo to reach the position
    }

    digitalWrite(lock_pin, LOW);
}


void loop() {
	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
    
    // Get the card serial
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(read_rfid);
    
    if (read_rfid==ok_rfid_1) {
        //ok, open the door.
        Serial.println(ok_rfid_1);
        open_lock();
    }
    
    if (read_rfid==ok_rfid_2) {
        //also ok
        Serial.println(ok_rfid_2);
        open_lock();
    }
    
}








