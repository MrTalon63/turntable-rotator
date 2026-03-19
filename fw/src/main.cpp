#include <Arduino.h>
#include <FastAccelStepper.h>

#define DIR_PIN 5
#define STEP_PIN 6

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
  Serial.begin(115200);
  delay(5000); // Wait for USB Serial because it sucky
  Serial.println("Setting up...");
  Serial.println("ESP SDK version: " + String(esp_get_idf_version()));

  engine.init();
  Serial.println("Engine initialized.");
  stepper = engine.stepperConnectToPin(STEP_PIN, DRIVER_I2S_DIRECT);
  if (stepper) {
    Serial.println("Stepper connected to pin.");
  } else {
    Serial.println("Failed to connect stepper to pin.");
    while (1); // Halt execution
  }
  stepper->setDirectionPin(DIR_PIN);
  stepper->setSpeedInUs(10);
  stepper->setAcceleration(1000);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    int space = input.indexOf(' ');
    String command;
    String value;
    if (space) {
      command = input; //no val
      value = "";
    } else {
      command = input.substring(0, space);
      value = input.substring(space + 1);
      value.trim();
    }
    command.toLowerCase();

    if (command == "f" || command == "forward") {
      if (value.isEmpty()) {
        Serial.println("Moving forward...");
        stepper->runForward();
      } else {
        uint32_t steps = value.toInt();
        Serial.println("Moving forward " + String(steps) + " steps...");
        stepper->move(steps);
      }

    } else if (command == "b" || command == "backward") {
      if (value.isEmpty()) {
        Serial.println("Moving backward...");
        stepper->runBackward();
      } else {
        uint32_t steps = value.toInt();
        Serial.println("Moving backward " + String(steps) + " steps...");
        stepper->move(-steps);
      }

    } else if (command == "s" || command == "speed") {
      if (value.isEmpty()) {
        Serial.println("Current speed: " + String(stepper->getCurrentSpeedInUs()) + " us/step.");
      } else {
        uint32_t speed = value.toInt();
        stepper->setSpeedInUs(speed);
        Serial.println("Speed set to " + String(speed) + " us/step.");
      }

    } else if (command == "a" || command == "acceleration") {
      if (value.isEmpty()) {
        Serial.println("Current acceleration: " + String(stepper->getAcceleration()) + " steps/s^2.");
      } else {
        uint32_t accel = value.toInt();
        stepper->setAcceleration(accel);
        Serial.println("Acceleration set to " + String(accel) + " steps/s^2.");
      }

    } else if (command == "l" || command == "stop") {
      Serial.println("Stopping...");
      stepper->stopMove();

    } else if (command == "status") {
      Serial.println("Current status:");
      Serial.println("  Speed: " + String(stepper->getCurrentSpeedInUs()) + " us/step");
      Serial.println("  Acceleration: " + String(stepper->getAcceleration()) + " steps/s^2");
      Serial.println("  Current position: " + String(stepper->getCurrentPosition()) + " steps");
    
    } else if (command == "h" || command == "help") {
      Serial.println("Available commands:");
      Serial.println("  f [steps] - Move forward (optional steps)");
      Serial.println("  b [steps] - Move backward (optional steps)");
      Serial.println("  s [speed] - Set speed in us/step or get current speed");
      Serial.println("  a [accel] - Set acceleration in steps/s^2 or get current acceleration");
      Serial.println("  l         - Stop movement");
      Serial.println("  status    - Show current status");
      Serial.println("  h         - Show this help message");
    }

    else {
      Serial.println("Unknown command: " + command);
    }
  }
}