# RoboCup Soccer Auto LightWeight V2
![Hardware](https://img.shields.io/badge/Hardware-EasyEDA-blue)
![Firmware](https://img.shields.io/badge/Firmware-C%2FSTM32%20HAL-brightgreen)
![Status](https://img.shields.io/badge/Status-Prototype-orange)

## Overview
The RoboCup Soccer Auto LightWeight V2 is a high-speed, fully autonomous soccer robot engineered for highly dynamic and high-impact competitive environments. The system leverages a distributed microcontroller architecture to separate real-time motion control from high-frequency sensor acquisition, ensuring deterministic performance during matches.

![Robot 3D Render/Photo](./Images/3D%20Back%20Side%20View.jpeg)

##  Match Highlights & Testing

Check out the robot's dynamic performance during the RoboCup Malay Regional matches and our high-speed lab testing sessions:
# RoboCup Highlights
* Click the image to watch the video*
[![RoboCup Highlights](./Images/Match%20Tubnail.jpeg)](https://drive.google.com/file/d/1jBLn-xqyJGWdrelZajv76mLIV3Pv5ss4/view?usp=drive_link)

# Solenoid and Ball Backspin Control
* Click the image to watch the video*
[![Solenoid and Ball Backspin Control](./Images/Shoot%20Tubnail.jpeg)](https://drive.google.com/file/d/1ghSHrVGV2VAAAy8IgdZurh89m6u4_ynl/view?usp=drive_link)

## System Architecture

### Distributed Control System
To ensure zero-latency motion control while processing heavy sensor data, the architecture is split across two microcontrollers:
* **Master Controller (STM32F4):** Handles the core state machine, kinematics calculations, and real-time motion control for the omni-directional drive.
* **Sensor Sub-Controller (STM32F1):** Dedicated exclusively to polling and filtering data from the sensor arrays, packaging it, and transmitting it to the master MCU.

### Sensor Fusion & Vision
* **Ball Detection:** Utilizes a custom 16-bit **IR TSSP4038 array** connected to the STM32F1 for ultra-fast, 360-degree infrared ball tracking.
* **Vision Processing:** Integrated with a **Maixcam Pro2** to handle advanced object recognition, field boundary detection, and goal tracking. 

### Mechanical & Chassis Design
* **Drive System:** 4-wheel non-orthogonal omni-directional chassis designed for holonomic movement, allowing simultaneous translation and rotation.
* **Durability:** Built around a custom-designed, structurally integrated PCB that acts as both the electrical backbone and a rigid mechanical mounting plate, engineered specifically to survive high-impact collisions.

## Hardware & PCB Design

The custom PCB was designed to handle high-current motor transients while protecting the sensitive STM32 logic and vision systems from ground bounce and EMI.

![PCB Layout](./Images/Master%20System%20PCB.png) *(Add your PCB image here)*

> **1st Place Champion - Junior RoboCup Malay Regional (RYSC by the Rotary Foundation)**

## Repository Structure

```text
├── 3D_Models/            # STEP files and chassis renders
├── Hardware/             # Altium/KiCad Schematic and PCB layout files
├── Firmware_Master/      # STM32F4 C-code (Motion control & Kinematics)
├── Firmware_Sensor/      # STM32F1 C-code (TSSP4038 IR Array parsing)
└── images/               # Documentation assets
