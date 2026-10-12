/* Zool v1.51 (Gremlin Graphics 1992/3; ARM version by James Byrne for
 * Cygnus Software Engineering; this release May 2001): what libroger needs
 * to know about the game.
 *
 * !Run: Set Zool$Dir and ZoolData$Dir to the application, then (RISC OS
 * 3.5 or later) WimpSlot 720K and Run !RunImage. On older systems it takes
 * a 640K slot and runs Allocate first (80K of screen memory, for mode 9's
 * two banks). !RunImage is a plain absolute (an uncompressed AIF image) at
 * &8000. It tells the two apart by the OS version (OS_Byte 129, &A5 or
 * later). On a Risc PC it picks its own screen mode: Zool$Mode
 * ("X320 Y256 F50") if set, else the 256-colour mode at least 320x256
 * closest to 320x256 at 50Hz that OS_ScreenMode 2 lists; and its timing:
 * Zool$Timing (0 every frame, 1 every other frame, 2 every 1/50s), else
 * from the mode's frame rate. */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <roger.h>

extern const struct roger_image recomp_image;      /* gen/entries.c */

#define APPDIR "../game/!Zool"

/* The game's filenames are <Zool$Dir>.x and <ZoolData$Dir>.x (and the Prefs
 * it saves, <Zool$Dir>.Prefs). Both are the application directory. */
static const char *const prefixes[] = {"<Zool$Dir>.", "<ZoolData$Dir>.", "$.!Zool.", NULL};

/* !Run's variables, and the two it leaves commented out (--mode, --timing) */
static const char *variables[] = {"Zool$Dir", "$.!Zool", "ZoolData$Dir", "$.!Zool",
                                  NULL, NULL, NULL, NULL, NULL};

static struct roger_config config;

static void setvar(const char *name, const char *value)
{
    int k = 0;
    while (variables[k]) k += 2;
    variables[k] = name;
    variables[k + 1] = value;
}

static int arg(const char *a)
{
    if (!strcmp(a, "--archimedes")) {           /* RISC OS 3.1: mode 9, Allocate's screen */
        config.vidc20 = 0;
        config.wimpslot = 640 * 1024;
        config.screen_size = 80 * 1024;
        return 1;
    }
    if (!strncmp(a, "--mode=", 7)) { setvar("Zool$Mode", a + 7); return 1; }
    if (!strncmp(a, "--timing=", 9)) { setvar("Zool$Timing", a + 9); return 1; }
    return 0;
}

/* !RunImage, whatever ,load-exec suffix the extraction gave it */
static char image_path[1024];

static const char *find_image(void)
{
    DIR *d = opendir(APPDIR);
    struct dirent *e;
    while (d && (e = readdir(d))) {
        size_t l = strcspn(e->d_name, ",");
        if (l == 9 && !strncasecmp(e->d_name, "!RunImage", 9)) {
            snprintf(image_path, sizeof image_path, "%s/%s", APPDIR, e->d_name);
            break;
        }
    }
    if (d) closedir(d);
    if (!image_path[0]) { fprintf(stderr, "no %s/!RunImage: build.sh extracts it\n", APPDIR); exit(1); }
    return image_path;
}

static struct roger_config config = {
    .title = "Zool (recompiled)",
    .image = &recomp_image,
    .appdir = APPDIR,
    .dir_prefixes = prefixes,
    .variables = variables,
    .initial_mode = 12,                          /* the desktop's; the game chooses its own */
    .wimpslot = 720 * 1024,                      /* !Run, RISC OS 3.5 or later */
    .vidc20 = 1,                                 /* a Risc PC; --archimedes: an A3000 */
    .interpret_gaps = 1,
    .arg = arg,
    .usage = "  --mode=\"X320 Y256 F50\"  the screen mode (Zool$Mode: X, Y, F only; Risc PC only)\n"
             "  --timing=N        0 update every frame, 1 every other frame, 2 every 1/50s (Zool$Timing)\n"
             "  --archimedes      an A3000 with RISC OS 3.1 (mode 9), not a Risc PC\n",
};

int main(int argc, char **argv)
{
    config.image_path = find_image();
    roger_main(argc, argv, &config);
}
