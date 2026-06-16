# ESP32 LED Blink Rate Controller with Button and Potentiometer

This project uses an ESP32 with ESP-IDF to control an LED blink rate using the BOOT button while also reading potentiometer input through the ADC. Each press of the BOOT button cycles the LED through different blink frequencies, and the potentiometer value is printed to the serial monitor in real time.

## Features

- Controls an LED connected to GPIO23
- Uses the ESP32 BOOT button on GPIO0 as an input
- Cycles through multiple LED blink frequencies
- Reads potentiometer input using ADC1
- Prints LED state, potentiometer value, and blink rate to the serial monitor
- Uses FreeRTOS delays for timing
- Built using ESP-IDF

## Hardware Used

- ESP32 development board
- LED
- Resistor for LED
- Potentiometer
- Breadboard
- Jumper wires

## Pin Configuration

| Component | ESP32 Pin |
|---|---|
| LED output | GPIO23 |
| BOOT button | GPIO0 |
| Potentiometer signal | GPIO34 / ADC1 Channel 6 |

## Blink Rates

Each BOOT button press cycles to the next blink rate.

| Speed Index | Blink Frequency | Half-Cycle Delay |
|---|---:|---:|
| 0 | 0.25 Hz | 2000 ms |
| 1 | 0.5 Hz | 1000 ms |
| 2 | 1 Hz | 500 ms |
| 3 | 2 Hz | 250 ms |
| 4 | 4 Hz | 125 ms |
| 5 | 8 Hz | 62 ms |
| 6 | 16 Hz | 31 ms |

After reaching 16 Hz, the next button press wraps the blink rate back to 0.25 Hz.

## Project Structure

```text
esp32-led-blink-rate-controller/
├── assets/
│   └── potentiometer_blink.mp4
├── main/
│   ├── assignment_file.c
│   └── CMakeLists.txt
├── CMakeLists.txt
├── sdkconfig
├── .gitignore
└── README.md
```

## How It Works

The LED is configured as a GPIO output on GPIO23. The BOOT button is configured as an input on GPIO0 with the internal pull-up resistor enabled.

The program checks for a button press by detecting when the previous button state was high and the current button state is low. When a press is detected, the program advances to the next blink speed.

The LED toggles on and off based on the current half-cycle delay. At the same time, the ESP32 reads the potentiometer value through ADC1 Channel 6, which corresponds to GPIO34. The current LED state, ADC reading, and blink frequency are printed to the serial monitor.

Example output:

```text
BOOT0 pressed | New blink rate: 1.00 Hz
LED changed to ON | Potentiometer value: 2048 | Blink rate: 1.00 Hz
LED changed to OFF | Potentiometer value: 2061 | Blink rate: 1.00 Hz
```

## Build Instructions

Make sure ESP-IDF is installed and configured.

Set the target:

```bash
idf.py set-target esp32
```

Build the project:

```bash
idf.py build
```

Flash and monitor:

```bash
idf.py flash monitor
```

## Notes

- This project was developed using ESP-IDF.
- GPIO0 is commonly connected to the ESP32 BOOT button.
- GPIO34 is input-only, making it suitable for potentiometer ADC readings.
- The button uses a simple edge-detection check with a short delay for basic debounce handling.
- Some VS Code or clangd warnings may appear because ESP-IDF uses toolchain-specific compiler flags. The project should be verified using `idf.py build`.

## Demo

[Watch the demo video](assets/potentiometer_blink.mp4)

## License

This project is for educational use.