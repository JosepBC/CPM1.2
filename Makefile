CC = gcc
PAR = src/sudokuCPM_P2.c
SER = src/serial.c
BIN = par
CFLAGS = -fopenmp -g
BUILD_FOLDER = build



all : $(PAR)
	@mkdir -p $(BUILD_FOLDER)
	@$(CC) -O3 $(PAR) $(CFLAGS) -o $(BUILD_FOLDER)/$(BIN).out

serial :
	@mkdir -p $(BUILD_FOLDER)
	@$(CC) -O3 $(SER) $(CFLAGS) -o $(BUILD_FOLDER)/serial.out

clean : 
	@rm -rf $(BUILD_FOLDER)/$(BIN).out

run : all
	@./$(BUILD_FOLDER)/$(BIN).out

time : all
	/usr/bin/time -p ./$(BUILD_FOLDER)/$(BIN).out