CC := g++
FLAGS := -std=c++11 -w
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o 
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@

$(BUILD_DIR)/Meeting.o: $(SRC_DIR)/Meeting.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Meeting.cpp

$(BUILD_DIR)/User.o:    $(SRC_DIR)/User.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/User.cpp

$(BUILD_DIR)/Date.o:  $(SRC_DIR)/Date.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Date.cpp

$(BUILD_DIR)/Storage.o: $(SRC_DIR)/Storage.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Storage.cpp

$(BUILD_DIR)/AgendaService.o: $(SRC_DIR)/AgendaService.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaService.cpp

$(BUILD_DIR)/AgendaUI.o: $(SRC_DIR)/AgendaUI.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaUI.cpp

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

clean: 
	@rm -rf $(BUILD_DIR)/*.o
	@rm -rf $(BIN_DIR)/*
