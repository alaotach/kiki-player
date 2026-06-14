# Kiki-Player
## Project Overview
So basically this whole thing was built for a program by hackclub named fallout and I got an idea to build an mp3 player to replace spotify like good old times from a random youtube video titled "how to quit spotify" and I actually managed to pull it off. It's a fully custom PCB including a 3D case that plays music, has a touch sensitive click wheel.
<img width="1054" height="1492" alt="Zine" src="https://github.com/user-attachments/assets/08ef38b7-8bbc-40c4-b3ee-3b6c2e2ea3e7" />

## What's Inside This Folder
**PCB Files:**
- `kiki-player.kicad_sch` - The main schematic where everything's connected.
- `kiki-player.kicad_pcb` - The actual PCB layout with all components placed and routed.
- `Sample-BOM_JLCSMT.xlsx` - Bill of Materials with all the parts.

**Firmware:**
- `kiki-player/` - It has all the firmware source code written in arduino with platformIo.

**3D Case Files:**
- `kiki-player-B_Cu.glb` / `kiki-player-B_Mask.glb` / `kiki-player-F_CuGlr` etc. - These are the individual layer exports from KiCad.
- `pcb.step` - The 3D model of the PCB itself that I imported into Fusion 360 to design the case around it
- <img width="521" height="702" alt="Screenshot 2026-05-17 161832" src="https://github.com/user-attachments/assets/7c7b27b4-e8d0-4516-8f25-94fe95661591" />
- `kiki-player-case.step` - The 3D model of the PCB case.
<img width="530" height="530" alt="5a1365c1-bdc9-4841-9451-511a8fc57f7d-Photoroom" src="https://github.com/user-attachments/assets/6876ac3d-33b6-48b8-891b-e10f008345f1" />
<img width="1020" height="412" alt="18b236ff-2246-4a2c-be6b-1bc1fa3a94f7-Photoroom" src="https://github.com/user-attachments/assets/9b3e8413-a732-45a8-a846-fcdcf4dda4ef" />


**Misc Files:**
- `.gorjob` files - Job queue files from Fusion 360 (not super important)
- `.csv` files - Component positioning data and BOM exports
- <img width="1866" height="683" alt="image" src="https://github.com/user-attachments/assets/028fd6ef-e570-4b26-9efc-1b88df0043e3" />

## The Files You Actually Need
**To Manufacture the PCB:**
1. `kiki-player.kicad_pcb` - Send this to JLCPCB (export as gerbers first)
2. `Sample-BOM_JLCSMT.xlsx` - Upload this with your PCB order for parts + assembly
3. `kiki-player.kicad_sch` - Reference schematic if anything goes wrong

**For Reference:**
- `kiki-player.kicad_pro` - Open this in KiCad if you want to modify anything
- Schematic files - Check these if you want to understand how everything's wired

---

## Build Guide

### Step 1 - Export Gerbers from KiCad

open `kiki-player.kicad_pcb` in KiCad, then go to **File → Fabrication Outputs → Gerbers**. In the dialog, make sure all layers are checked and hit Generate. Then go back to **File → Fabrication Outputs → Drill Files** and generate those too. Put everything in one folder and zip it up.

---

### Step 2 - Order the PCB from JLCPCB

Go to [jlcpcb.com](https://jlcpcb.com) and upload your gerber zip.

Then enable **PCB Assembly** (PCBA), upload `Sample-BOM_JLCSMT.xlsx` as the BOM and `kiki-player-all-pos.csv` as the component placement file. JLCPCB will match your parts to their library and solder the SMD components for you. Confirm the parts in their viewer and place the order.

---

### Step 3 - Prepare the SD Card

Format a MicroSD card as FAT32 and dump your MP3 files into it.

---

### Step 4 - Solder the Through-Hole Parts

JLCPCB assembly only handles SMD components. You'll need to solder through-hole parts yourself:

- **J5** - 2-pin battery connector (PinHeader 2.54mm) - connects to LiPo
- **LS1** - 2-pin speaker connector (PinHeader 2.54mm) - connects to speaker
- **J3** - 3.5mm audio jack
---

### Step 5 - Connect the Battery

Get a 1000mAh flat LiPo with a 2-pin JST PH2.0 connector.
---

### Step 6 - Connect the Display

The ILI9341 display module connects via a ribbon cable or header pins depending on which variant you got. Match the pin labels:

| Display Pin | PCB Pin |
|---|---|
| VCC | 3V3 |
| GND | GND |
| CS | GPIO10 |
| RESET | GPIO14 |
| DC | GPIO13 |
| MOSI | GPIO11 |
| SCK | GPIO12 |
| LED | GPIO9 |

---

### Step 7 - Flash the Firmware

See the Firmware Flashing section below.

---


### Step 8 - 3D Print and Assemble the Case
- **Material:** PLA works fine, PETG if you want it tougher

When everything fits, seat the PCB, connect the display ribbon, tuck the battery into the remaining space, and close the case with M2 screws through the corner holes.

---

## Firmware Flashing

### Prerequisites

VSCode and PlatformIO

### First Flash

1. Open VSCode, go to the PlatformIO home screen, click **Open Project**, and select the `kiki-player/` folder from this repo.

2. PlatformIO will automatically download the ESP32-S3 toolchain and all libraries listed in `platformio.ini` the first time. This takes a few minutes, let it finish.

3. Connect the ESP32-S3-WROOM-1 board via USB-C to your computer. The board should show up as a COM port (Windows) or `/dev/ttyACM0` or similar (Linux/Mac).

4. If the board doesn't show up, you might need to put it in download mode manually: hold the **BOOT button** (SW4 on your PCB), tap **RESET** (SW3), then release BOOT. The board should now show up as a port.

5. In VSCode, click the **→ Upload** button at the bottom of the screen.

6. PlatformIO will compile and flash. 

7. After flashing, hit the RESET button on your PCB. The display should light up with the splash screen.

---

### Modifying the Firmware

All the important settings are in `include/config.h` - timeouts, colors, volume defaults, sleep timer. Change those without touching any other file. Pin definitions are all in `include/pins.h` if you've rewired anything.

Each feature is in its own file so it's easy to find things - `src/audio.cpp` for playback, `src/ui.cpp` for screens, `src/input.cpp` for the click wheel and buttons.

---

## Controls

| Input | Action |
|---|---|
| Wheel clockwise | Scroll down / Volume up (in player) |
| Wheel counter-clockwise | Scroll up / Volume down (in player) |
| Center touch | Select / Play-Pause |
| Menu button (SW3) | Back |
| Select button (SW4) | Confirm |
| Long press Menu | Deep sleep |
| Long press Select | Next track (in player) |
