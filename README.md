# Kiki-Player

## Project Overview

So basically this whole thing was built for a program by hackclub named fallout and I got an idea to build an mp3 player to replace spotify like good old times from a random youtube video titled "how to quit spotify" and I actually managed to pull it off. It's a fully custom PCB including a 3D case that plays music, has a touch sensitive click wheel.

Note: I am yet to create the firmware.

## What's Inside This Folder

**PCB Files:**
- `kiki-player.kicad_sch` - The main schematic where everything's connected.
- `kiki-player.kicad_pcb` - The actual PCB layout with all components placed and routed.
- `Sample-BOM_JLCSMT.xlsx` - Bill of Materials with all the parts.

**3D Case Files:**
- `kiki-player-B_Cu.glb` / `kiki-player-B_Mask.glb` / `kiki-player-F_CuGlr` etc. - These are the individual layer exports from KiCad.
- `updated-pcb.step` - The 3D model of the PCB itself that I imported into Fusion 360 to design the case around it

**Misc Files:**
- `.gorjob` files - Job queue files from Fusion 360 (not super important)
- `.csv` files - Component positioning data and BOM exports

## The Files You Actually Need

**To Manufacture the PCB:**
1. `kiki-player.kicad_pcb` - Send this to JLCPCB (export as gerbers first)
2. `Sample-BOM_JLCSMT.xlsx` - Upload this with your PCB order for parts + assembly
3. `kiki-player.kicad_sch` - Reference schematic if anything goes wrong

**For Reference:**
- `kiki-player.kicad_pro` - Open this in KiCad if you want to modify anything
- Schematic files - Check these if you want to understand how everything's wired
