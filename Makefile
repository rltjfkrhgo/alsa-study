CC = g++
LDFLAGS = -lasound
SOURCES = *.cpp
EXECUTABLE = alsa-study

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o $(EXECUTABLE)