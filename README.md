# Howland-VCCS-for-GVS-experiment-prototype
# Voltage controlled Constant Current Source Circuit for Galvanic Vestibular Stimulation (GVS)

## Project Overview
This project is a voltage controlled constant current source circuit designed for Galvanic Vestibular Stimulation (GVS) applications. The circuit aims to provide a compact solution for stable and precise current output to ensure the safety and effectiveness. The prototype contains two major parts, an enhanced Howland current source and a DC-DC boost for easy access. 

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Circuit Design](#circuit-design)
- [Usage](#usage)
- [License](#license)
- [Contact](#contact)

## Features
- **Stable Current Output**: Provides a -4mA to 4mA stable current output suitable for transcranial electrical stimulation, the output granularity depends on the precision of the DAC module you are using, e.g., a 12bit Digital-to-Analog Converter (DAC) module can theoretically reach 4096 steps.
- **Adjustable Current Range**: You can adjust the output current as needed by using a DAC module. Current output is linear with given voltage.
- **Compact Design**: The size of the circuit is 37×28mm, but the size could be future reduced such as by using SOIC op-amps instead of using DIP.
- **Calibration**: To achieve higher accuracy in calibrating the output current, you can use either a hardware approach or a software approach.
  - Evaluation Method: When the reference voltage is 5.2V and the DAC input is 2.6V, the current output should be 0mA. This is based on the idea that a 5.2V input results in a 4mA output, and a 0V input           results in a -4mA output. However, due to voltage accuracy and resistor tolerance issues, deviations can occur. If there's a linear shift in the output, the following approaches can help correct it.
    Hardware Approach:
  - Howland current sources are very sensitive to resistor tolerances. Carefully check the actual values of your resistors, particularly R3, R4, R5, and R6. Fine-tuning the resistance of R8 can help you           achieve better performance.
  - Software Approach:
    If there is a linear shift in the output, you can adjust the DAC voltage value by adding an offset. While this approach may affect the maximum current output, it is suitable for situations where hardware      adjustment is difficult and an exact ±4mA output is not required.
    By using these methods, you can improve the accuracy of your current output calibration.

## Hardware Requirements
- **Power Supply**: 3.7V DC power supply (lithium battery)
- **Controller**: Arduino or other microcontroller
- **Key Components**:
  - Operational amplifier
  - Step-up converter
  - Voltage reference IC
  - Other basic components (resistors, capacitors, etc.)

## Circuit Design
Please refer to the `schematics` folder for the circuit diagram and bill of materials.
- [Circuit Schematic](./schematics/schematic.png)
- [Bill of Materials](./schematics/BOM_GVS.csv)

## Usage
1. **Hardware Setup**:
    - Order this prototype at any PCB manufacturer such as JLCPCB.
    - Connect the circuit board to other components. You would need a DAC module which supports 0v to 5.2v voltage output, a micro-controller, and few electrodes

2. **Usage Instructions**:
    - IMPORTANT: Always use a multimeter and potentiometer to evaluate the actual output current before try it on yourself or anyone else. 
    - This prototype is designed for loads ranging from 0 to 5 kOhm. Please use professional electrodes and conductive gel to reduce the impedance for best experience.
      
## Contributing: 
    Contributions are welcome! Please read CONTRIBUTING.md for more information.
  
## License: 
    This project is licensed under CERN-OHL-W-2.0
  
## Contact: 
    shawnliu@digitalnature.slis.tsukuba.ac.jp or shawnliu092@yahoo.com
