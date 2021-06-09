#include <Servo.h>

// PINS
const int switchPIN = 7;
const int handPIN = 6;
const int doorPIN = 5;

// STATES
int handOFF = 0;
int handON = 160;
int doorCLOSE = 0;
int doorOPEN = 70;
int OPEN = 0;
int rndVal = 0;
int lastVal = -1;

// Times
int startOffsetTime = 500;
int fullHandRotationTime = 1500;
int fullDoorRotationTime = 800;

// Servos
Servo doorServo;
Servo handServo;

// Helper functions

/*
Attaches to the servo, turns it, waits some time and detaches.
We do this to stop the current draw
*/
void turnDoorServo(int val, int wait) {
  //doorServo.attach(doorPIN);
  doorServo.write(val);
  delay(wait);
  //doorServo.detach();
}

/*
Attaches to the servo, turns it, waits some time and detaches.
We do this to stop the current draw
*/
void turnHandServo(int val, int wait) {
  //handServo.attach(handPIN);
  handServo.write(val);
  delay(wait);
  //handServo.detach();
}

void resetServos() {
  turnHandServo(handOFF, fullHandRotationTime);
  turnDoorServo(doorCLOSE, fullDoorRotationTime);
}

void setup() {
  pinMode(switchPIN, INPUT_PULLUP);
  doorServo.attach(doorPIN);
  handServo.attach(handPIN);
  resetServos();
}

/*
Simple, plain open, turn off & close
*/
void simpleClose() {
    turnDoorServo(doorOPEN, fullDoorRotationTime);
    turnHandServo(handON, fullHandRotationTime);
    turnHandServo(handOFF, fullHandRotationTime);
    turnDoorServo(doorCLOSE, fullDoorRotationTime);
}

/*
Opens the door, turns the hand half way, make a small pause and then continue.
*/
void simpleCloseSlow() {
    turnDoorServo(doorOPEN, fullDoorRotationTime);
    turnHandServo(handON / 2, fullHandRotationTime / 2);
    simpleClose();
}

/*
Opens the door repeatedly and fakes the trigger
*/
void increasingFakeTouch() {
  int turns = 3;
  for(int i = 0; i < turns; i++) {
    turnDoorServo(doorOPEN, fullDoorRotationTime);
    turnHandServo(handON / (turns - i), fullHandRotationTime / (turns - i));
    turnHandServo(handOFF, fullHandRotationTime);
    turnDoorServo(doorCLOSE, fullDoorRotationTime);
  }
}

/*
Opens the door multiple times without turning the hand at all
 */
void crazyDoor() {
    for (int i = 0; i < 3; i++) {
      turnDoorServo(doorOPEN / 1.5, fullDoorRotationTime / 1.5);
      turnDoorServo(doorCLOSE, fullDoorRotationTime / 1.5);
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
  turnDoorServo(doorOPEN, fullDoorRotationTime);
  turnHandServo(handON / 2, fullHandRotationTime / 2);
  for (int i = 0; i < 4; i++) {
    turnHandServo(handON / 4, fullHandRotationTime / 4);
    turnHandServo(handON / 2, fullHandRotationTime / 2);
  }
  simpleClose();
}

void loop() {
  int sensorVal = digitalRead(switchPIN);
  if (sensorVal == OPEN) {
    delay(startOffsetTime);
    
    // dont make same move twice
    while(rndVal==lastVal) {
      rndVal = random(0, 6);
    }
    lastVal = rndVal;
    
    switch (rndVal) {
      case 0:
        simpleClose();
        break;
      case 1: 
        simpleCloseSlow();
        break;
      case 2:
        crazyDoor();
        break;
      case 3:
        slow();
        break;
      case 4:
        increasingFakeTouch();
        break;
      case 5: 
        cantDecide();
        break;
    }
  }
}
