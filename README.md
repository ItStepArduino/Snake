# Snake
Snake game for Arduino

## Setup
Requires an 8x8 led matrix, a joystick, and optionally 16 resistors to tune the led matrix brightness to your desired level.

The row side of the led matrix connects to the following pins from left to right on the led matrix: 10, 6, 4, 13, 9, 11, 3, 2.

The collumn side of the led matrix connects to the following pins from right to left on the led matrix: 12, A1, 5, A3, 7, 8, A2, A0.

If the led matrix is too bright, you can connect resistors from the wires to the led matrix.

(if the led matrix isn't working properly, try rotating it and connecting it to the opposite wires)

The joystick is wired as follows:
- **GND**: Any ground
- **+5V**: 5V output pin
- **VRx**: A5 pin
- **VRy**: A4 pin
- **SW**: Any ground

![Circuit Image](/Circuit.JPG?raw=true "Title")


If the snake moves too slowly or quickly for you, you can edit the `WaitingTime` definition at the top of the code file.
Increasing the number makes the snake slower, decreasing the number makes the snake faster.


## Demo
<a href="https://www.youtube.com/watch?v=YYSAoE4YP3I" target="_blank">
	<img src="https://img.youtube.com/vi/YYSAoE4YP3I/0.jpg" alt="Snake Game Demo" width="240" height="180" border="10" />
</a>
