#include <Arduino.h>
#include <FastAccelStepper.h>
#include <TMCStepper.h>

#define AZ_DIR_PIN 5
#define AZ_STEP_PIN 6
#define AZ_EN_PIN 2137
#define AZ_CS_PIN 2137
#define R_SENSE 0.11f // Idfk that's what is in example

TMC2130Stepper azDriver(AZ_CS_PIN, R_SENSE);
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *azStepper = NULL;

void setup() {
  Serial.begin(115200);
  delay(5000); // Wait for USB Serial because it sucky
  Serial.println("Setting up...");
  Serial.println("ESP SDK version: " + String(esp_get_idf_version()));

  SPI.begin();
  azDriver.begin();
  azDriver.toff(5);
  azDriver.rms_current(600); // mA
  azDriver.microsteps(16);

  // Enable sum fancy shit, allegedly stepper should be silky smooth
  azDriver.en_pwm_mode(true);
  azDriver.pwm_autoscale(true);

  engine.init();
  Serial.println("Engine initialized.");
  azStepper = engine.stepperConnectToPin(AZ_STEP_PIN, DRIVER_I2S_DIRECT);

  if (azStepper) {
    Serial.println("Stepper connected to pin.");
  } else {
    Serial.println("Failed to connect stepper to pin.");
    while (1); // Halt execution
  }

  azStepper->setDirectionPin(AZ_DIR_PIN);
  azStepper->setSpeedInUs(10);
  azStepper->setAcceleration(1000);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    int space = input.indexOf(' ');
    String command;
    String value;
    if (!space) {
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
        azStepper->runForward();
      } else {
        uint32_t steps = value.toInt();
        Serial.println("Moving forward " + String(steps) + " steps...");
        azStepper->move(steps);
      }

    } else if (command == "b" || command == "backward") {
      if (value.isEmpty()) {
        Serial.println("Moving backward...");
        azStepper->runBackward();
      } else {
        uint32_t steps = value.toInt();
        Serial.println("Moving backward " + String(steps) + " steps...");
        azStepper->move(-steps);
      }

    } else if (command == "s" || command == "speed") {
      if (value.isEmpty()) {
        Serial.println("Current speed: " + String(azStepper->getCurrentSpeedInUs()) + " us/step.");
      } else {
        uint32_t speed = value.toInt();
        azStepper->setSpeedInUs(speed);
        Serial.println("Speed set to " + String(speed) + " us/step.");
      }

    } else if (command == "a" || command == "acceleration") {
      if (value.isEmpty()) {
        Serial.println("Current acceleration: " + String(azStepper->getAcceleration()) + " steps/s^2.");
      } else {
        uint32_t accel = value.toInt();
        azStepper->setAcceleration(accel);
        Serial.println("Acceleration set to " + String(accel) + " steps/s^2.");
      }

    } else if (command == "l" || command == "stop") {
      Serial.println("Stopping...");
      azStepper->stopMove();

    } else if (command == "status") {
      Serial.println("Current status:");
      Serial.println("  Speed: " + String(azStepper->getCurrentSpeedInUs()) + " us/step");
      Serial.println("  Acceleration: " + String(azStepper->getAcceleration()) + " steps/s^2");
      Serial.println("  Current position: " + String(azStepper->getCurrentPosition()) + " steps");
    
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