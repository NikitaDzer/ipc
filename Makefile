CC     = gcc
CFLAGS = -O3
LFLAGS =  

BUILD_DIR = ./build
EXEC_FILE = ipc


all: $(EXEC_FILE)

$(EXEC_FILE): $(BUILD_DIR) $(BUILD_DIR)/main.o $(BUILD_DIR)/ipc.o
	$(CC) $(LFLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/ipc.o -o $(EXEC_FILE)

$(BUILD_DIR)/main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/ipc.o: ipc.c ipc.h
	$(CC) -c $(CFLAGS) ipc.c -o $(BUILD_DIR)/ipc.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXEC_FILE)
