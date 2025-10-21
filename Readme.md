Line Clipping Algorithm (C++)

A small demo project implementing a **2D line clipping algorithm** for computer graphics.

This program determines whether a line segment lies inside, outside, or partially within a rectangular
viewport, and clips it accordingly.  
The algorithm was inspired by examples from *"Tricks of the Windows Game Programming Gurus"* by A. Lamoth
and developed as an experiment while learning geometric algorithms.

---

## 🧠 Purpose

To practice geometric computations such as:
- detecting line and rectangle intersections,
- calculating intersection points,
- and trimming (clipping) lines that extend beyond the visible area.

---

## ⚙️ Technologies

- **C++ (STL)** — classes for points, lines, and the clipping algorithm
- **Object-oriented design** — encapsulated geometry logic in `Point`, `Line`, and `LineClipper` classes
- **Math concepts** — line equations, vector products, and intersection formulas

---

## 🚀 How to Run

Compile and run the project using C++ compiler (tested with Visual Studio 2019).

---

## 🧩 Example Output

L4: (17; 12) - (22; 11)
Has intersection with (8; 7) - (8; 13) no
Has intersection with (19; 7) - (19; 13) yes
intersection at (19; 11)
newL4: (19; 11) - (19; 13)


Demonstrates how the algorithm detects intersections and trims the line segment inside the viewport.

---
Created as part of personal learning experiments.