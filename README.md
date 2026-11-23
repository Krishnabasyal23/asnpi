# Motion-triggered traffic light (Raspberry Pi, C)

A traffic light controller written in C for the Raspberry Pi. The light rests on green. When the PIR motion sensor fires, the buzzer sounds and the light cycles through yellow to red and back to green.

## How it works

- Three POSIX threads: one polls the PIR motion sensor, one polls the light sensor, and one drives the LEDs and buzzer.
- The sensor threads and the light thread share state through a small counting semaphore (`semaphore.c`) so readings are never half-written.
- GPIO is driven through the `rpi_gpio` helper from the course's `hardware-component-samples` library, which is included in this repository with its own licence.

| Part | GPIO |
| --- | --- |
| Red LED | 5 |
| Yellow LED | 16 |
| Green LED | 18 |
| Buzzer | 24 |
| PIR sensor (input) | 22 |
| Light sensor (input) | 23 |

## Files I wrote

`main.c`, `traffic.c/.h`, `sensor.c/.h`, `semaphore.c/.h`. Everything under `hardware-component-samples/` is the provided course library.

## Build

On the Pi, compile the files above against `hardware-component-samples/common/rpi_gpio` with `-lpthread`, then run the result with `sudo` for GPIO access.
