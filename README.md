# Technical Specification: PT Motorsport CAN Development Board

## 1. Product Overview
The **PT Motorsport CAN Development Board** is a professional-grade prototyping platform designed to integrate the Arduino Nano into automotive Control Area Network (CAN) environments. It provides a robust, vibration-resistant alternative to breadboards for developing custom automotive electronics, data loggers, and vehicle interface modules.

The board features a dedicated CAN physical layer consisting of an **MCP2515 CAN Controller** and a high-speed transceiver, coupled with an expansive prototyping grid for custom circuitry.

---

## 2. Power Architecture
The board is designed to interface directly with vehicle electrical systems (nominal 12V DC). It utilizes the Arduino Nano’s internal regulation to provide logic power to the CAN controller and secondary hardware.

| Rail | Description |
| :--- | :--- |
| **12V** | Input power from the vehicle source. Routes directly to the Arduino `VIN` pin and the top utility header. |
| **GND** | Common ground reference for the vehicle chassis and the PCB logic. |
| **5V** | Regulated output from the Arduino Nano. Used to power the onboard MCP2515 and available for external peripherals. |

---

## 3. Hardware Configuration & Pin Allocation

### 3.1 Integrated CAN Interface
The following pins are hardwired to the onboard MCP2515 controller and are reserved for CAN communication:
* **D10:** Chip Select (CS)
* **D11:** SPI Master Out Slave In (MOSI)
* **D12:** SPI Master In Slave Out (MISO)
* **D13:** SPI Serial Clock (SCK)
* **D2:** CAN Interrupt (INT) — *Routed for high-speed packet handling.*

### 3.2 External Interfacing (JST XHP-4)
Vehicle-side connectivity is handled via a 4-pin **JST XHP-4** connector located at the top of the board.
* **Pin 1:** 12V DC Input
* **Pin 2:** Ground
* **Pin 3:** CAN High
* **Pin 4:** CAN Low

---

## 4. Prototyping & Expansion
The right-hand side of the board contains a 15-column prototyping area designed for permanent component mounting.

### 4.1 Vertical Header Breakdown
* **Top Row:** 12V Power Rail.
* **Middle Row:** Common Ground (GND) Rail.
* **Bottom Row:** 5V Regulated Rail.
* **Signal Breakdown:** All standard Arduino Nano pins (`D0`–`D9`, `A0`–`A7`, and `RST`) are broken out into the grid to facilitate custom hardware integration for various automotive projects.

---

## 5. Technical Implementation Details

### 5.1 Timing & Frequency
The onboard MCP2515 utilizes an **8MHz Crystal Oscillator (X1)**. When initializing CAN libraries in the Arduino IDE, the frequency must be explicitly set to **8MHz** to ensure correct baud rate calculation for the vehicle's bus speed (e.g., 500kbps or 1Mbps).

### 5.2 Application Scope
This development board serves as the hardware foundation for various automotive projects hosted within this repository, ranging from custom diagnostic gauges and sensor hubs to standalone vehicle controllers.

---
*Documentation for PT Motorsport CAN Development Board Revision: October 2025 v1.3.*
