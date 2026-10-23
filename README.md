Zool v1.51 (Gremlin Graphics, 1992/3; ARM version by James Byrne; 2001 release) - static recompilation
======================================================================================

A recipe: this repository holds no game data. It rebuilds the game as a
native executable from your own copy, recognised by content (recipe.json),
so it can be shared without distributing the game.

    ./build.sh Zool              (the ArcFS archive the release came as, or a .zip holding it)
    cd recomp && ./zool

build.sh runs recipe.json's steps with SuperSpider's tools/weave.py
(~/stuff/superspider, or $SUPERSPIDER; $SS below): it extracts !Zool from
the archive ($SS/tools/arcfs.py), analyses and translates !RunImage, and
links it with libroger (~/stuff/libroger, or make ROGER=...). Needs
python3, OCaml/dune, gcc and SDL3.

The input is the May 2001 release of v1.51: the ArcFS archive "Zool"
holding !Zool, or a zip holding that archive.

    extracted/arc            the archive, via weave.py
    game/!Zool               libroger's appdir (../game/!Zool)
    analysis/zool.json       SuperSpider's analysis
    analysis/zool.roots      gap entries (make roots)
    recomp/gen/              the C translation
    recomp/zool              the game

Committed: recipe.json, build.sh, recomp/Makefile, recomp/game.c,
analysis/zool.roots, this file.

How the game is put together
----------------------------

!Run sets Zool$Dir and ZoolData$Dir to the application. On RISC OS 3.5 or
later it takes a 720K WimpSlot and runs !RunImage; on older systems a 640K
slot, after Allocate (80K of screen memory, for mode 9's two banks). It
leaves two variables commented out: Zool$Mode (only X, Y and F of a mode
string) and Zool$Timing (0 update every frame, 1 every other frame, 2
every 1/50s).

!RunImage is a plain absolute, an uncompressed AIF image, at &8000. It
tells the two systems apart by the OS version (OS_Byte 129, &A5 or
later). On a Risc PC it picks its own screen mode - Zool$Mode if set,
else the 256-colour mode closest to 320x256 at 50Hz that OS_ScreenMode 2
lists - and its timing, Zool$Timing or from the mode's frame rate.

What's translated (recomp/Makefile, make regen): !RunImage, whole (gen/),
with the gap entries a --trace-gaps run logged as roots (the start-up code
at &87B8-&8938 and the routines at &25130-&26194). game.c still has
libroger interpret any gap silently (interpret_gaps). A headless run to
the first level's "Get ready" interprets nothing.

game.c: !Run's variables, a Risc PC by default (RISC OS 3.5, the 720K
slot, the game choosing its own 256-colour mode), the game's files found
under <Zool$Dir>. and <ZoolData$Dir>. (both the application; it saves
<Zool$Dir>.Prefs), and options for the rest.

Running it
----------

    ./zool                      a Risc PC (RISC OS 3.5)
    ./zool --archimedes         an A3000 with RISC OS 3.1: mode 9, Allocate's screen
    ./zool --mode="X320 Y256 F50"   Zool$Mode (Risc PC only)
    ./zool --timing=N           Zool$Timing

Keys (from the game's manual, Manual.Zool/html): Z left, X right, ' up,
/ down, Return fire. F1 and F2 lower and raise the volume, F3 turns the
internal speaker on and off (not on a Risc PC), F4 chooses music, sound
effects or no sound, P pauses, Q quits when paused, and F12 returns to the
desktop. The main options screen (level, sound, music, inertia,
continues, speed) is worked with up, down, left and right; fire leaves it.

A game controller: the stick or D-pad as Z X ' /, A fire, B jump, Start
pause; connected at start (or --joystick), it is the Acorn joystick the
game reads (Joystick_Read) when its options choose the joystick.
