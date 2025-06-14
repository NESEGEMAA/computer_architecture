# 💻 Computer System Architecture Course Project

## 🧠 Project Title

**Double Big Harvard Combo – Large Arithmetic Shifts Processor Simulation**

---

## 📚 Course Information

* **Course Name:** CSEN 601 – Computer System Architecture
* **Instructor:** Dr. Catherine M. Elias

---

## 👥 Team Members

* Noureldin Salaheldin – *[noureldin.gamaleldin@gmail.com](mailto:noureldin.gamaleldin@gmail.com)*
* Salma Tarek Soliman – *[salmaaburahmah@gmail.com](mailto:salmaaburahmah@gmail.com)*
* Habiba Mahmoud – *[hrefaee72004@gmail.com](mailto:hrefaee72004@gmail.com)*
* Layla Khaled – *[laila.khaled.04@gmail.com](mailto:laila.khaled.04@gmail.com)*
* Lama Abdeldayem – *[lamaadayem1111@gmail.com](mailto:lamaadayem1111@gmail.com)*
* Yasmeen Tarek – *[yasmeen.tarek.elabsawy@gmail.com](mailto:yasmeen.tarek.elabsawy@gmail.com)*
* Mai Hazem – *[maihazem607@gmail.com](mailto:maihazem607@gmail.com)*

---

## 📌 Project Overview

This project involves the design and implementation of a **C-based simulator** for a fictional processor based on a **Harvard architecture**, developed as part of the CSEN601 course. The processor supports a **16-bit ISA**, a **3-stage pipeline**, and manages instruction/control hazards.

The simulator is capable of parsing and executing custom assembly code while providing detailed live output per clock cycle, showcasing register updates, memory changes, and pipeline state transitions.

---

## ✨ Key Features

* **Harvard Architecture** with separated instruction and data memory
* **16-bit custom ISA** supporting 12 instructions (ADD, SUB, MOVI, BEQZ, etc.)
* **3-stage pipeline**: Instruction Fetch (IF), Instruction Decode (ID), Execute (EX)
* **Control hazard handling** with flushing logic
* **Status register updates** with correct flag handling (Carry, Overflow, Sign, etc.)
* **Cycle-accurate output logging** showing full register/memory state

---

## 🛠️ Technologies Used

* **C Language** – core simulation logic
* **WSL (Windows Subsystem for Linux)** – development environment
* **GCC / CMake** – build and compile system

---

## 📁 Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── README.md               # Project documentation
├── build/                  # Build artifacts
├── include/                # Header files (interfaces)
│   ├── decoder.h
│   ├── globals.h
│   ├── instruction_map.h
│   ├── instructions.h
│   ├── memory.h
│   ├── parser.h
│   ├── pipeline.h
│   ├── queue.h
│   └── types.h
├── src/                    # Source code
│   ├── decoder.c
│   ├── instruction_map.c
│   ├── instructions.c
│   ├── main.c
│   ├── memory.c
│   ├── parser.c
│   ├── pipeline.c
│   └── queue.c
└── test.asm                # Sample test assembly program
```
