# RoboCup Soccer Auto LightWeight V2

> **1st Place Champion - Junior RoboCup Malay Regional (RYSC by the Rotary Foundation)**

## Overview
The RoboCup Soccer Auto LightWeight V2 is a high-speed, fully autonomous soccer robot engineered for highly dynamic and high-impact competitive environments. The system leverages a distributed microcontroller architecture to separate real-time motion control from high-frequency sensor acquisition, ensuring deterministic performance during matches.

![Robot 3D Render/Photo](./images/your_3d_render_here.png) *(Add your 3D render here)*

##  Match Highlights & Testing

Check out the robot's dynamic performance during the RoboCup Malay Regional matches and our high-speed lab testing sessions:
[![RoboCup Highlights](https://img.youtube.com/vi/YOUR_YOUTUBE_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_YOUTUBE_VIDEO_ID)

*(Click the image above to watch the video)*

**Alternative Link:** [Watch the Match Highlights & Testing Here](https://www.youtube.com/watch?v=YOUR_YOUTUBE_VIDEO_ID)

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

![PCB Layout](./images/your_pcb_layout_here.png) *(Add your PCB image here)*
![Schematic](./images/your_schematic_here.png) *(Add your Schematic image here)*

## Repository Structure

```text
├── 3D_Models/            # STEP files and chassis renders
├── Hardware/             # Altium/KiCad Schematic and PCB layout files
├── Firmware_Master/      # STM32F4 C-code (Motion control & Kinematics)
├── Firmware_Sensor/      # STM32F1 C-code (TSSP4038 IR Array parsing)
└── images/               # Documentation assets
