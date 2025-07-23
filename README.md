# Heater Control System

## Overview
This project implements a basic heater control system on Wokwi using an ESP32. It reads temperature, controls a heater (via a relay), and logs status via Serial.

## Hardware
- ESP32 (Wokwi simulation)
- Potentiometer (simulated temperature sensor, GPIO34)
- Relay Module (GPIO 26)
- 16x2 I2C LCD (SDA: GPIO21, SCL: GPIO22)
- Active Buzzer (9V, <50mA, connected to relay’s NO and COM, powered by 9V DC power source)
- LED (GPIO 27 - for status indication)

## Features
- State machine with Idle, Heating, Stabilizing, Target Reached, and Overheat states
- Continuous temperature monitoring via timer interrupts
- Relay controls an active buzzer (simulating heater with audible feedback)
- LCD displays temperature and state
- Serial logging
- External 9V DC power source for buzzer (simulating battery)

## Setup
1. Open the Wokwi project.
2. Connect components as described (buzzer via relay to 9V DC power source).
3. Upload the code.
4. Adjust the potentiometer to simulate temperature changes.
5. Monitor the LCD, Serial output (115200 baud), and buzzer behavior.

## Wokwi Simulation
https://wokwi.com/projects/437201915805953025

## Future Improvements
- Add PID control for precise temperature regulation.
- Implement overheating protection with a secondary sensor.
- Add a mobile app for remote control and monitoring.
