# 🔑 RFID Door Lock System

## 📖 Description
This project is an **RFID-based door lock system** built with an Arduino Uno and an RC522 RFID module.  
The system checks RFID tags against a pre-approved list and simulates an electronic access control system:

- ✅ Valid RFID tag: Green LED turns on, servo motor rotates to “unlock” the door, and console prints `Accepted: [UID]`.  
- ❌ Invalid RFID tag: Red LED turns on, and console prints `Denied: [UID]`.  

This demonstrates how hardware and software integrate to build a security-focused embedded system.

---

## 🛠️ Hardware Needed
- Arduino Uno  
- Breadboard  
- RC522 RFID Module  
- LEDs (Red & Green)  
- Servo Motor  
- Jumper wires  

---

## 💻 Software Needed
- [Arduino IDE](https://www.arduino.cc/en/software)  


---

## 📂 Files
- `rfid_doorlock.ino` — Arduino sketch (main code)  
- Circuit diagram:
   <img width="843" height="550" alt="Screenshot 2025-10-02 at 3 40 19 PM" src="https://github.com/user-attachments/assets/a69c1aa9-5fbc-4aa3-84e4-fa845731bd7a" />

---

## ▶️ Usage
1. Clone this repo:  
   ```bash
   git clone https://github.com/yourusername/rfid-doorlock.git
