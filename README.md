# Fiery Swarm

  A Computer Science senior design project at California State University Fullerton.
  
  Fiery Swarm explores the possibilities of swarm robotics. We’ve created an interconnected, distributed miniature vehicle 
  platform designed to work in synchronicity with direct clones to perform autonomously-choreographed intelligent traversal. 
  We aim to discover and explore ways in which to automate, iterate and continuously improve self-driving vehicle platforms 
  with a centralized intelligence.
  
  Our vehicle hardware platform consists of an ESP32 microcontroller mounted onto a modified low-cost remote cotrolled toy 
  vehicle
 
  Our software stack includes Arduino build tools, the FreeRTOS operating system and a selection of products from Amazon Web 
  Services.

## Installing Arduino IDE & Connecting To ESP32:

Linux:
  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/

Windows:
  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

## Setup:

- `pip install pyserial`

- `pip install esptool`

## Thing #'s:

- Tim: `0`
- David: `1`

## Common Problems & Solutions:

1) Ensure Arduino IDE connected to correct port; e.g. /dev/ttyUSB0

2) Ensure Arduino IDE is using correct profile; e.g. ESP32vn IoT Uno

3) Trouble writing to the device?
`sudo chmod 777 /dev/ttyUSB0`
sudo usermod -a -G dialout $USER
4) make menuconfig fails:
sudo apt install flex
sudo apt install bison


## Team Members:

| Member           | GitHub      | Semester    |
|------------------|-------------|-------------|
| Feinzimer, David | kernelpop   | Spring 2019 |
| Parks, Timothy   | skrapmi     | Spring 2019 |
| Purpura, Evan    | evanpurpura | Spring 2019 |
| Yeung, Zach      | zyeungcsuf  | Spring 2019 |
