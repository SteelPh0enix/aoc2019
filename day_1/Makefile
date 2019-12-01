CXX = g++
MKDIR = mkdir -p
RM = rm -rf

CFLAGS_MISC = -std=c++14 -O2
CFLAGS_WARNINGS = -Wall -Wextra -Wpedantic
CFLAGS = $(CFLAGS_MISC) $(CFLAGS_WARNINGS)
LFLAGS = -L.
IFLAGS = -I. -I../utils
LIB_DEPS = 

OUT_DIR = build
OBJS_DIR = build/objs
OUT_EXEC = build/challenge

SOURCES = 
MAIN_SRC = main.cpp
OBJS = $(addprefix $(OBJS_DIR),$(subst .cpp,.o,$(SOURCES)))

.PHONY: make_build_dir all clean

all: make_build_dir main

main: $(OBJS)
	$(CXX) $(CFLAGS) $(LFLAGS) $(IFLAGS) $(MAIN_SRC) -o $(OUT_EXEC) $(OBJS) $(LIB_DEPS)

.c.o:
	$(CXX) $(CFLAGS) $(IFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJS_DIR)
	$(RM) $(OUT_DIR)

make_build_dir:
	$(MKDIR) $(OUT_DIR)
	$(MKDIR) $(OBJS_DIR)