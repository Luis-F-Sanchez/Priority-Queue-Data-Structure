CXX = clang++
# Unused: warn, but annoying to block compilation on
# Sign compare: noisy
# Command line arg: noisy, not relevant to students
CXXFLAGS = \
	-Wall -Wextra -Werror \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=unused-variable \
	-Wno-error=unused-but-set-variable \
	-Wno-error=unused-value \
	-Wno-sign-compare \
	-Wno-unused-command-line-argument \
	-std=c++2a -I. -g -fno-omit-frame-pointer \
	-fsanitize=address,undefined

ENV_VARS = ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppr.txt:print_suppressions=false

# On Ubuntu and WSL, googletest is installed to /usr/include or
# /usr/local/include, which are used by default.

# On Mac, we need to manually include them in our path. Brew installs to
# different locations on Intel/Silicon, so ask brew where things live.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	LLVM_PREFIX := $(shell brew --prefix llvm)
	CXX := $(LLVM_PREFIX)/bin/clang++
	CXXFLAGS += -I$(GTEST_PREFIX)/include
	CXXFLAGS += -L$(GTEST_PREFIX)/lib
endif

prqueue_tests: prqueue_tests.cpp prqueue.h
	$(CXX) $(CXXFLAGS) prqueue_tests.cpp -lgtest -lgmock -lgtest_main -o $@

test_core: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueCore*"

test_using: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueUsing*"

test_duplicates: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueDuplicates*"

test_copying: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueCopying*"

test_equality: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueEquality*"

test_iterator: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="PRQueueIterator*"

test_all: prqueue_tests
	$(ENV_VARS) ./$< --gtest_color=yes

prqueue_main: prqueue_main.cpp
	$(CXX) $(CXXFLAGS) prqueue_main.cpp -o $@

run_prqueue: prqueue_main
	$(ENV_VARS) ./$<

clean:
	rm -f prqueue_tests prqueue_main build/*
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean test_all test_core test_using test_duplicates test_copying test_equality test_iterator