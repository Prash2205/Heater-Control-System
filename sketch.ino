#include <LiquidCrystal_I2C.h>

// Pin definitions
#define TEMP_SENSOR_PIN 34  // Analog pin for temperature sensor
#define RELAY_PIN 26        // Relay for heater control
#define LED_PIN 27          // LED for status indication

// Temperature thresholds
#define TARGET_TEMP 25.0    // Target temperature (°C)
#define HYSTERESIS 2.0      // Hysteresis for stabilizing state
#define OVERHEAT_TEMP 35.0  // Overheat threshold (°C)

// State machine enumeration
enum HeaterState {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

HeaterState currentState = IDLE;
float temperature = 0.0;
hw_timer_t *timer = NULL; // Timer handle
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD at address 0x27

void IRAM_ATTR readTemperature() {
  // Simulate temperature reading (replace with actual sensor read)
  int raw = analogRead(TEMP_SENSOR_PIN);
  temperature = (raw / 4095.0) * 50.0; // Scale to 0-50°C for simulation
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Heater off initially
  digitalWrite(LED_PIN, LOW);   // LED off initially
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Heater Control");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000); // Show splash screen
  
  // Timer setup for 1-second interrupts
  timer = timerBegin(1000000); // Timer with 1MHz frequency (1µs tick)
  timerAttachInterrupt(timer, &readTemperature); // Attach interrupt
  timerAlarm(timer, 1000000, true, 0); // Trigger every 1,000,000 ticks (1s)
}

void loop() {
  updateStateMachine();
  logStatus();
  updateLCD();
  delay(500);
}

// State machine logic
void updateStateMachine() {
  switch (currentState) {
    case IDLE:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      if (temperature < TARGET_TEMP - HYSTERESIS) {
        currentState = HEATING;
      }
      break;

    case HEATING:
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH); // Yellow (simulated)
      if (temperature >= TARGET_TEMP) {
        currentState = STABILIZING;
      } else if (temperature >= OVERHEAT_TEMP) {
        currentState = OVERHEAT;
      }
      break;

    case STABILIZING:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH); // Blue (simulated)
      if (temperature >= TARGET_TEMP - HYSTERESIS && temperature <= TARGET_TEMP + HYSTERESIS) {
        currentState = TARGET_REACHED;
      } else if (temperature < TARGET_TEMP - HYSTERESIS) {
        currentState = HEATING;
      } else if (temperature >= OVERHEAT_TEMP) {
        currentState = OVERHEAT;
      }
      break;

    case TARGET_REACHED:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH); // Green (simulated)
      if (temperature < TARGET_TEMP - HYSTERESIS) {
        currentState = HEATING;
      } else if (temperature >= OVERHEAT_TEMP) {
        currentState = OVERHEAT;
      }
      break;

    case OVERHEAT:
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, HIGH); // Red (simulated)
      if (temperature < OVERHEAT_TEMP - HYSTERESIS) {
        currentState = STABILIZING;
      }
      break;
  }
}

// Log temperature and state to Serial
void logStatus() {
  String stateStr;
  switch (currentState) {
    case IDLE: stateStr = "Idle"; break;
    case HEATING: stateStr = "Heating"; break;
    case STABILIZING: stateStr = "Stabilizing"; break;
    case TARGET_REACHED: stateStr = "Target Reached"; break;
    case OVERHEAT: stateStr = "Overheat"; break;
  }
  Serial.printf("Temperature: %.1f°C, State: %s\n", temperature, stateStr.c_str());
}

// Update LCD display
void updateLCD() {
  String stateStr;
  switch (currentState) {
    case IDLE: stateStr = "Idle"; break;
    case HEATING: stateStr = "Heating"; break;
    case STABILIZING: stateStr = "Stabilizing"; break;
    case TARGET_REACHED: stateStr = "TargetReached"; break;
    case OVERHEAT: stateStr = "Overheat"; break;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Temp: %.1f C", temperature);
  lcd.setCursor(0, 1);
  lcd.print(stateStr);
}