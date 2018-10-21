# Allow settiing a project name from the environment, default to firmware.
# Only affects the name of the generated binary.
# TODO: Set this from the directory this makefile is stored in
PROJ 			?= Arm
# Points where the SJSUOne libraries sources are located

# IMPORTANT: Must be accessible via the PATH variable!!!
CPPC            = g++

# Internal build directories
OBJ_DIR			= $(BASE)/Build/$(PROJ)/source
BIN_DIR			= bin
BASE = $(abspath $(dir $(firstword $(MAKEFILE_LIST))))"/../../.."
CFLAGS = -Wall -Wshadow -Wlogical-op -Wfloat-equal \
    -fabi-version=0 -fno-exceptions \
    -DDEBUG=1 \
    -std=gnu++11 \
    -I"$(BASE)/Utilities/" \
    -I"$(BASE)/Testing/" \
    -I"../" \
    -I"../source/" \
    -I"$(BASE")/Testing/" \
    -I"$(BASE)/Testing/stubs/" 

TESTFILES 			= $(shell cat "test-files.list")
COMPILABLES		 	= test.cpp $(addprefix ../source/, $(TESTFILES)) 
CGREEN_COMPILABLES	= $(addprefix ../source/, $(TESTFILES)) cgreen-test.cpp
TEST_EXEC 			= ./test-suite
CGREEN_TEST_EXEC	= ./cgreen-test-suite

.PHONY: test build build-cgreen cgreen clean debug

test: $(TEST_EXEC)
	@$(TEST_EXEC) -s

cgreen: $(CGREEN_TEST_EXEC)
	@$(CGREEN_TEST_EXEC) -s

$(TEST_EXEC): clean
	@echo " \\───────────────────────────────────────/"
	@echo "  \\      Generating test executable     /"
	@$(CPPC) $(CFLAGS) -fexceptions -o $(TEST_EXEC) $(COMPILABLES)
	@echo "   \\───────────────────────────────────/"
	@echo "    \\            Finished             /"
	@sleep .25
	@echo "     \\───────────────────────────────/"
	@echo "      \\        Running Test         /"
	@echo "       \\───────────────────────────/"
	@sleep .25

$(CGREEN_TEST_EXEC): clean
	@echo -n 'Generating CGREEN test executable '
	@$(CPPC) $(CFLAGS) -fexceptions -o $(CGREEN_TEST_EXEC) $(CGREEN_COMPILABLES) -lcgreen
	@echo '--> Finished'

debug:
	@echo $(TESTFILES)
	@echo "=================="
	@echo $(COMPILABLES)
	@echo "=================="
	@echo $(TEST_EXEC)
	@echo "=================="
	@echo $(CGREEN_TEST_EXEC)
	@echo "=================="

clean:
	@rm -f $(TEST_EXEC) $(CGREEN_TEST_EXEC)
