SRC = src
BIN = bin
MAIN = main
TEST = test
MAIN_FILES = $(filter-out $(SRC)/$(TEST).cpp, $(wildcard $(SRC)/*.cpp))
TEST_FILES = $(filter-out $(SRC)/$(MAIN).cpp, $(wildcard $(SRC)/*.cpp))

run: compile
	@echo "Running $(BIN)/$(MAIN)..."
	@$(BIN)/./$(MAIN)

compile: $(BIN)
	@echo "Compiling files in $(SRC)..."
	g++ $(MAIN_FILES) -o $(BIN)/$(MAIN)

test: compile_test
	@echo "Running tests in $(BIN)/$(TEST)..."
	@$(BIN)/./$(TEST)

compile_test: $(BIN)
	@echo "Compiling files in $(SRC)..."
	g++ $(TEST_FILES) -o $(BIN)/$(TEST)

$(BIN):
	@echo "Folder $(BIN) not found"
	@echo "Creating $(BIN)..."
	mkdir -p $(BIN)

clean:
	@echo "Removing $(BIN)..."
	rm -rf $(BIN)