#include <IRremote.h>

const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;

int motorState = 0; // 0: Stop, 1: Forward, 2: Left Turn, 3: Right Turn, 4: Backward

const int RECV_PIN = 8;  // Pin for IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

// Define IR remote codes
const unsigned long FORWARD_CODE = 0xFF18E7;
const unsigned long LEFT_CODE = 0xFF10EF;
const unsigned long RIGHT_CODE = 0xFF5AA5;
const unsigned long BACKWARD_CODE = 0xFF4AB5;
const unsigned long STOP_CODE = 0xFF38C7;

void setup() {
  pinMode(StepX, OUTPUT);
  pinMode(DirX, OUTPUT);
  pinMode(StepY, OUTPUT);
  pinMode(DirY, OUTPUT);

  Serial.begin(9600); // Start serial communication
  irrecv.enableIRIn();  // Start the IR receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long command = results.value;

    if (command == FORWARD_CODE) {
      motorState = 1; // Move forward
    } else if (command == LEFT_CODE) {
      motorState = 2; // Turn left
    } else if (command == RIGHT_CODE) {
      motorState = 3; // Turn right
    } else if (command == BACKWARD_CODE) {
      motorState = 4; // Move backward
    } else if (command == STOP_CODE) {
      motorState = 0; // Stop motors
      stopMotors();   // Call the stop function
    }

    irrecv.resume();  // Receive the next value
  }

  // Motor control logic based on motorState
  if (motorState == 1) {  // Move forward
    digitalWrite(DirX, HIGH); // Set direction forward for both motors
    digitalWrite(DirY, HIGH);
  } else if (motorState == 2) {  // Left turn
    digitalWrite(DirX, LOW);  // One motor backward
    digitalWrite(DirY, HIGH); // Other motor forward
  } else if (motorState == 3) {  // Right turn
    digitalWrite(DirX, HIGH); // One motor forward
    digitalWrite(DirY, LOW);  // Other motor backward
  } else if (motorState == 4) {  // Move backward
    digitalWrite(DirX, LOW);  // Both motors backward
    digitalWrite(DirY, LOW);
  }

  if (motorState != 0) {
    // Step both motors
    digitalWrite(StepX, HIGH);
    digitalWrite(StepY, HIGH);
    delayMicroseconds(500);

    digitalWrite(StepX, LOW);
    digitalWrite(StepY, LOW);
    delayMicroseconds(500);
  }
}

// Function to stop the motors
void stopMotors() {
  digitalWrite(StepX, LOW);  // Set the step pin to LOW
  digitalWrite(StepY, LOW);
  digitalWrite(DirX, LOW);   // Ensure direction is set to LOW to stop
  digitalWrite(DirY, LOW);

  Serial.println("Motors stopped.");  // Optional: Print a message to Serial
}