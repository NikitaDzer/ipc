CC     = gcc
CFLAGS = -O3
LFLAGS =  

BUILD_DIR = ./build
TEST_DIR  = ./tests
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


tests: 
	mkdir -p $(TEST_DIR)
	dd if=/dev/urandom of=$(TEST_DIR)/256.txt  bs=1048576 count=256
	dd if=/dev/urandom of=$(TEST_DIR)/1024.txt bs=1048576 count=1024
	dd if=/dev/urandom of=$(TEST_DIR)/4096.txt bs=1048576 count=4096

clean:
	rm -rf $(BUILD_DIR) $(TEST_DIR) $(EXEC_FILE)
