DS = wd_test
DS1 = sorted_list
DS2 = dlist
DS3 = p_queue
DS4 = task
DS5 = uid
DS6 = scheduler

SRC_DIR := ./src
TEST_DIR := ./test
INC_DIRS := ./include

INC_FLAGS := $(addprefix -iquote, $(INC_DIRS))

CC = gcc
CPPFLAGS = $(INC_FLAGS) -pedantic-errors -Wall -Wextra -g -lm -pthread 

$(DS).out: $(TEST_DIR)/$(DS).c $(DS1).o $(DS2).o $(DS3).o $(DS4).o $(DS5).o $(DS6).o 
	$(CC) $(CPPFLAGS) $^ -L. -lwatchdog -o $@

$(DS1).o: $(SRC_DIR)/$(DS1).c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DS2).o: $(SRC_DIR)/$(DS2).c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DS3).o: $(SRC_DIR)/$(DS3).c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DS4).o: $(SRC_DIR)/$(DS4).c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DS5).o: $(SRC_DIR)/$(DS5).c
	$(CC) $(CPPFLAGS) -c $< -o $@

$(DS6).o: $(SRC_DIR)/$(DS6).c
	$(CC) $(CPPFLAGS) -c $< -o $@

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(DS).out

.PHONY: clean
clean:
	-rm -f *.out
	-rm -f *.o
