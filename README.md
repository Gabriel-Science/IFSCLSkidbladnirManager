# IFSCLSkidbladnirManager
An Arduino UNO R4 WiFi program for controlling efficiently the Skidbladnir in IFSCL in Replika missions.

## The trailer
https://youtu.be/FsugQy2wQs0
![La miniature du trailer YouTube](https://github.com/user-attachments/assets/6693a550-1b30-40ca-a942-0e0d8806528a)



## How to use ?
### Wiring the project
Here is all the required equipment.
- A PC with the Arduino IDE 2.3.7 or later.
- 1 Arduino UNO R4 WiFi
- 1 breadboard
- A few jumper wires
- 6 buttons
- 1 RGB LED
- 3 220 Ω resistors

Connect the circuit like in the [circuit diagram](https://github.com/user-attachments/files/24869664/IFSCL.Skidbladnir.manager.circuit.diagram.pdf).

### Uploading the program to the board
1. Open the file in the Arduino IDE.
2. [Update the board's firmware](https://support.arduino.cc/hc/en-us/articles/360013896579-Use-the-Firmware-Updater-in-Arduino-IDE) if the firmware isn't updated.
3. Install the program's library. They are shown at the top of the program, for example, `#include <Keyboard.h>`.
4. Compile and upload to the board.
> [!WARNING]
> Sometimes, the program won't be immediately uploaded to the board and you will need to reselect the board. To avoid this, rapidely press the RESET button twice. This will put the board in [*bootloader mode*](https://support.arduino.cc/hc/en-us/articles/5779192727068-Reset-your-board).

### Usage
> [!CAUTION]
> Please note that this program is not lag-proof. A powerful PC is advised. If you can't get enough FPS even when lowering down the game's quality, the only consistent way is to set the `minCommandTime` integer to a bigger value.

> [!IMPORTANT]
> The Lyoko-warriors who are selected in the first step will be shown on the first line of the LED matrix. The LW that are translated are shown on the second line too. The order is Aelita, Odd, Yumi, Ulrich and William.
#### LW selection
- Select the LW who are in the Skidbladnir with the first five buttons.
- You can deselect and select again any warrior during this step.
- Once your selection is done, press the main button (the sixth one).
#### Going into the virtual world (VWorld).
- Everything is automated. It will go to the tower and even activate it ! Some scanners will be closed too for virtual energy.
> [!WARNING]
> Normally, the Skidbladnir shouldn't try to go to the tower while it's still in elevation. However, because the delay for that is hardcoded, it may or may not work on some Replikas. If that happens to you, feel free to open an issue. The same applies for any other delay issues.
#### Translation
- Around half of your warriors will be automatically translated.
- If some LW remains in the Skidbladnir, they will be energized out of it.
- When you need it, press the main button to go to the next step.
#### Swapping
> [!NOTE]
> This step will be skipped it everyone is translated.
- All the currently translated LW will be detranslated.
- The LW that were currently not translated will be translated
- When you need it, press the main button to go to the next step.
#### Devirt & autopilot
- All the currently translated LW will be detranslated (the reason is to avoid unconscious LW).
- The Skidbladnir will undock from the tower. This will cause everyone to get back into the VWorld.
- After reactivating the scanners, it will devirt everyone making sure to `devirt.exe` after three devirts.
- The Skidbladnir will return to Lyoko automatically with autopilot.
- You can end the mission and skip time.

> [!CAUTION]
> Make sure to never put your LW out of their Skid docks at any time, or if you do, make sure to make them step on their pod before starting a new step. It could maybe, for some steps, completely mess the process. Also, this program might not work correctly if one of your LW gets devirted / detranslated.

## The LW status lamp
The LED is called the LW status lamp. Its main purpose is to indicate in a pretty way which LW is being "used" (selected, translated, et cetera). The colour depends on the warrior : light pink for Aelita, purple for Odd, purple-ish red for Yumi, yellow for Ulrich and light blue for William.
