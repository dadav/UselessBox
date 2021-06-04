#include <Servo.h>

// PINS
const int switchPIN = 7;
const int handPIN = 6;
const int doorPIN = 5;

// STATES
int handOFF = 0;
int handON = 160;
int doorCLOSE = 0;
int doorOPEN = 60;
int OPEN = 0;

// Misc
int selectedMove = 0;

// Servos
Servo doorServo;
Servo handServo;

// Helper functions

/*
Attaches to the servo, turns it, waits some time and detaches.
We do this to stop the current draw
*/
void turnDoorServo(int val, int wait) {
  doorServo.attach(doorPIN);
  doorServo.write(val);
  delay(wait);
  doorServo.detach();
}

/*
Attaches to the servo, turns it, waits some time and detaches.
We do this to stop the current draw
*/
void turnHandServo(int val, int wait) {
  handServo.attach(handPIN);
  handServo.write(val);
  delay(wait);
  handServo.detach();
}

void resetServos() {
  turnHandServo(handOFF, 100);
  turnDoorServo(doorCLOSE, 100);
}

void setup() {
  Serial.begin(9600);
  pinMode(switchPIN, INPUT_PULLUP);
  resetServos();
  delay(1000);
}

/*
Simple, plain open, turn off & close
*/
void simpleClose() {
    turnDoorServo(doorOPEN, 1000);
    turnHandServo(handON, 2000);
    turnHandServo(handOFF, 450);
    turnDoorServo(doorCLOSE, 500);
}

/*
Opens the door, turns the hand half way, make a small pause and then continue.
*/
void simpleCloseSlow() {
    turnDoorServo(doorOPEN, 1000);
    turnHandServo(handON / 2, 2000);
    simpleClose();
}

/*
Opens the door repeatedly and fakes the trigger
*/
void increasingFakeTouch() {
  int turns = 5;
  for(int i = 0; i < turns; i++) {
    turnDoorServo(doorOPEN, 1000);
    turnHandServo(handON / (turns - i), 300 + i * 100);
    turnHandServo(handOFF, 450);
    turnDoorServo(doorCLOSE, 500);
  }
  simpleClose();
}

/*
Opens the door multiple times without turning the hand at all
 */
void crazyDoor() {
    for (int i = 0; i < 3; i++) {
      turnDoorServo(doorOPEN / 4, 300);
      turnDoorServo(doorCLOSE, 300);
    }
    simpleClose();
}

/*
Does a simple close, really slow
 */
void slow() {
  // open door
  for (int pos = doorCLOSE; pos < doorOPEN; pos = pos + 5) {
    turnDoorServo(pos, 200);
  }
  turnDoorServo(doorOPEN, 250);
  
  // turn finger
  for (int pos = handOFF; pos < handON - 30; pos = pos + 5) {
    turnHandServo(pos, 100);
  }
  turnHandServo(handON, 250);
  
  // turn finger off
  for (int pos = handON; pos > handOFF; pos = pos - 5) {
    turnHandServo(pos, 200);
  }
  
  // close door
  for (int pos = doorOPEN; pos > doorCLOSE; pos = pos - 5) {
    turnDoorServo(pos, 200);
  }
}

/*
Turns the hand out & in multiple times
*/
void cantDecide() {
  turnDoorServo(doorOPEN, 1000);
  turnHandServo(handON / 2, 500);
  for (int i = 0; i < 5; i++) {
    turnHandServo(handON / 4, 200);
    turnHandServo(handON / 2, 200);
  }
  simpleClose();
}

void loop() {
  int sensorVal = digitalRead(switchPIN);
  if (sensorVal == OPEN) {
    if (selectedMove > 5) { 
      selectedMove = 0;
    }

    if (selectedMove == 0) {
      simpleClose();
    } else if (selectedMove == 1) { 
      simpleCloseSlow(); 
    } else if (selectedMove == 2) { 
      crazyDoor(); 
    } else if (selectedMove == 3) { 
      slow(); 
    } else if (selectedMove == 4) { 
      increasingFakeTouch(); 
    } else if (selectedMove == 5) { 
      cantDecide(); 
    }
    
    selectedMove += 1;
  }
}
