# In-Car-Simulation-Based-On-I-O-Autosar-Stack

## Overview

Welcome to our In-Car Simulation project, a comprehensive exploration of embedded systems using AUTOSAR standards. This project focuses on creating a simulated environment within the embedded context, leveraging the classical AUTOSAR standards to develop standardized I/O MCAL drivers.

## Project Scope

### Standardized I/O MCAL Drivers

To streamline the integration of various project modules, we have developed standardized I/O MCAL drivers for essential modules, including:
- ADC (Analog-to-Digital Converter)
- DIO (Digital I/O)
- ICU (Input Capture Unit)
- OCU (Output Compare Unit)
- PORT (Port Interface)
- PWM (Pulse Width Modulation)

These drivers play a crucial role in facilitating seamless communication and interaction among different components of our project.

### ECU Communication via CAN Bus

The project is organized into three Electronic Control Units (ECUs), each utilizing the TM4C123GXL microcontroller and communicating through the CAN bus:

1. **Sensor ECU Node:**
   - Collects real-time data, including Motor Speed, Temperature, Front Object Detection, and Rear Parking Sensor readings.

2. **Actuator ECU Node:**
   - Receives updated data from the Sensor ECU through CAN bus messages.
   - Takes actions based on received data, such as controlling the Fan or Motor using PWM techniques.

3. **Dashboard ECU Node:**
   - Receives updated data from the Sensor ECU via CAN bus messages.
   - Updates the Dashboard LCD reading to relay messages or notify the driver based on the sensor data.

## How to Use

Follow these steps to get started with our In-Car Simulation project:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/dina12118/In-Car-Simulation-Based-On-I-O-Autosar-Stack.git

2. **Setup TM4C123GXL Environment:**
   - Ensure you have the necessary development environment set up for the TM4C123GXL microcontroller.

3. **Compile and Flash Code:**
   - Compile the source code using your preferred compiler.
   - Flash the compiled code onto each TM4C123GXL microcontroller.

4. **Connect CAN Bus:**
   - Establish a CAN bus connection between the three ECUs to enable communication.

5. **Run the Simulation:**
   - Power on the ECUs and observe the simulated interaction between the Sensor, Actuator, and Dashboard nodes.


## Contributing

#### We welcome contributions from the community to enhance and expand the capabilities of our In-Car Simulation project. Feel free to open issues, propose new features, or submit pull requests.

Happy coding! 🚗✨
