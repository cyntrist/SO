CC = gcc
CFLAGS = -O3 -g
LDFLAGS =
LIBS =
TARGET = ejemplo

TARGET_SRC = $(wildcard *.c)		# Automatically find all .c files in the directory
TARGET_OBJ = $(TARGET_SRC:.c=.o)	# Convert .c source files into .o object files

# Base rule called when making
all: $(TARGET)

# Rule to compile each .c file into a .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Link all .o object files to create the executable
$(TARGET): $(TARGET_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TARGET_OBJ) $(LIBS)

# Rule to clean the project from intermediate files
.PHONY: clean
clean:
	-rm $(TARGET_OBJ) $(TARGET)
