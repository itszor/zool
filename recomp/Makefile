# Static recompilation of Zool: the translated program (gen/, from
# SuperSpider's analyse and gen) and game.c, linked with libroger (the RISC
# OS runtime).
ROGER   ?= $(HOME)/stuff/libroger
SS      ?= $(HOME)/stuff/superspider
SSBIN   := $(SS)/_build/default/bin/main.exe
CC      ?= gcc
CFLAGS  ?= -O2 -g -w -fno-strict-aliasing $(if $(TRACE),-DRECOMP_TRACE)
GEN     := $(wildcard gen/code*.c) gen/entries.c
OBJ     := $(GEN:.c=.o) game.o
LIBS    := $(shell pkg-config --libs sdl3) -lm -lpthread
INC     := -I$(ROGER)/include
# !RunImage, whatever its ,load-exec suffix
IMAGE   := $(firstword $(wildcard ../game/!Zool/!RunImage,*) ../game/!Zool/!RunImage)

zool: $(OBJ) $(ROGER)/libroger.a
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(ROGER)/libroger.a $(LIBS)

$(ROGER)/libroger.a: FORCE
	$(MAKE) -C $(ROGER) libroger.a

gen/%.o: gen/%.c $(ROGER)/include/roger_cpu.h gen/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

game.o: game.c $(ROGER)/include/roger.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# make regen: analyse !RunImage (its roots: ../analysis/zool.roots) and
# translate it
regen:
	cd $(SS) && dune build
	touch ../analysis/zool.roots
	$(SSBIN) analyse '$(IMAGE)' 8000 --full --roots ../analysis/zool.roots -o ../analysis/zool.json
	rm -rf gen
	$(SSBIN) gen '$(IMAGE)' 8000 ../analysis/zool.json --c gen

# make roots: merge the gap entries a --trace-gaps run logged
# (untranslated.log, or LOG=...) into ../analysis/zool.roots
LOG ?= untranslated.log
roots:
	python3 -I $(SS)/tools/gap_roots.py '$(IMAGE)' 8000 ../analysis/zool.roots $(LOG)

clean:
	rm -f gen/*.o game.o zool

.PHONY: FORCE regen roots clean
