CC = gcc
CFLAGS = -Wall
PROGRAMS = air_elemental earth_elemental fire_elemental water_elemental task1

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)
