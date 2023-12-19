# Solar Tracking using Fuzzy Logic


Welcome to the Solar Tracking using Fuzzy Logic project! This Arduino code enables solar panels to track the sun's current orientation using a fuzzy logic algorithm. The fuzzy logic implementation involves initializing fuzzy sets, defining fuzzy rules, and utilizing a fuzzy inference engine.

## Table of Contents

- [How to Use](#how-to-use)
- [Library Used](#library-used)
- [Fuzzy Logic Implementation Details](#fuzzy-logic-implementation-details)
- [Contributions and Issues](#contributions-and-issues)
- [License](#license)

## How to Use

1. **Upload the Code:**
   Upload the `Fuzzy_Logic_Solar_Tracking.ino` file to your Arduino board using the Arduino IDE or any compatible programming environment.

2. **Automatic Tracking:**
   After uploading, the solar tracker will automatically start working. It continuously adjusts the orientation of the solar panels to maximize sun exposure.

## Library Used

This fuzzy logic algorithm relies on the `Fuzzy.h` library. Make sure to install this library in your Arduino IDE before uploading the code.

## Fuzzy Logic Implementation Details

- **Fuzzy Sets Initialization:**
  The fuzzy sets are initialized to represent different linguistic variables like Sensor Fuzzy Set(Dim,Normal,Bright), Vertical Motor Fuzzy Set(Low, Medium, High) and Horizonal Fuzzy Set(Low, Medium, High).

- **Fuzzy Rules:**
  Fuzzy rules define the relationship between input variables Sensors Light Intensity(Top Sensor, Left Sensor, Buttom Sensor, Right Sensor) and output actions (solar panel orientation). These rules guide the fuzzy inference engine in making decisions.

- **Fuzzy Inference Engine:**
  The fuzzy inference engine processes the fuzzy rules and input variables to determine the optimal orientation for the solar panels based on the current sun position.

## Contributions and Issues

Feel free to contribute to the project by submitting pull requests or reporting issues. Your input is valuable in enhancing the performance and reliability of the solar tracking system.

## License

This project is under the [MIT License](LICENSE), granting you the freedom to use, modify, and share the code.

Happy solar tracking!
