# Kiki-Player
## Project Overview
So basically this whole thing was built for a program by hackclub named fallout and I got an idea to build an mp3 player to replace spotify like good old times from a random youtube video titled "how to quit spotify" and I actually managed to pull it off. It's a fully custom PCB including a 3D case that plays music, has a touch sensitive click wheel.
<img width="1054" height="1492" alt="zine-kikii" src="https://github.com/user-attachments/assets/69871eaa-2eba-4f57-80d8-d32bfb0cc1c8" />

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
<img width="530" height="530" alt="kiki-case-1" src="https://github.com/user-attachments/assets/4fd8bc28-0faa-4192-a84d-99f71c83b31a" />
<img width="1020" height="412" alt="kiki-case-2" src="https://github.com/user-attachments/assets/c89307f0-a1be-4b86-ae5f-180ca74ecf49" />


**Misc Files:**
- `.csv` files - Component positioning data and BOM exports
- <img width="1866" height="683" alt="image" src="https://github.com/user-attachments/assets/028fd6ef-e570-4b26-9efc-1b88df0043e3" />

## The Files You Actually Need
**To Manufacture the PCB:**
1. `kiki-player.kicad_pcb` - Send this to JLCPCB
2. `Sample-BOM_JLCSMT.xlsx` - Upload this with your PCB order for parts + assembly
3. `kiki-player.kicad_sch` - Reference schematic if anything goes wrong

**For Reference:**
- `kiki-player.kicad_pro` - Open this in KiCad if you want to modify anything
---

## Build Guide

### Step 1 - Export Gerbers from KiCad

open `kiki-player.kicad_pcb` in KiCad, then go to **File → Fabrication Outputs → Gerbers**. In the dialog, make sure all layers are checked and hit Generate. Then go back to **File → Fabrication Outputs → Drill Files** and generate those too. Put everything in one folder and zip it up.

---

### Step 2 - Order the PCB from JLCPCB

Go to jlcpcb and upload your gerber zip.

- Then enable PCBA and upload `Sample-BOM_JLCSMT.xlsx` as the BOM and `kiki-player-all-pos.csv` as the component placement file.
---

### Step 3 - Prepare the SD Card

Format a MicroSD card as FAT32 and dump MP3 files into it.

---

### Step 4 - Solder the TH Parts

- **J5** - 2-pin JST PH2.0 MALE Connector - connects to battery lipo
- **LS1** - 2-pin JST PH2.0 MALE Connector - connects to speaker
- **J3** - 3.5mm audio jack
---

### Step 5 - Connect the Battery

Get a 1000mAh flat LiPo with a 2-pin JST PH2.0 connector.
---

### Step 6 - Connect the Display

- The ILI9341 display module connects via a ribbon cable.
---

### Step 7 - Flash the Firmware

```bash
cd firmware/kiki-player
pio run --target upload
```
( You need VSCode and PlatformIO for it )

### Step 8 - 3D Print and Assemble the Case
- **Material:** PLA works fine.

When everything fits, seat the PCB, connect the display ribbon, tuck the battery into the remaining space, and close the case.

---

## Modifying the Firmware

All config parameters live in:

```
firmware/kiki-player/include/Config.h
```

Features are split into modules under `firmware/kiki-player/src/`.

---
