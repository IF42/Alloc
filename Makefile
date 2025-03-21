PLATFORM?=linux_x86_64

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=  


TARGET=liballoc.a
CACHE=.cache
OUTPUT=$(CACHE)/release


MODULES += arena.o
MODULES += general.o

TEST += test.o


OBJ=$(addprefix $(CACHE)/,$(MODULES))
T_OBJ=$(addprefix $(CACHE)/,$(TEST))


all: env $(OBJ)
	ar -crs $(OUTPUT)/$(TARGET) $(OBJ)


-include config/$(PLATFORM).mk


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


exec: all $(T_OBJ)
	$(CC) $(T_OBJ) $(OBJ) $(LIBS) -o $(OUTPUT)/test
	$(OUTPUT)/test


.PHONY: env dep clean


dep:
	$(FIND) src test -name "*.c" | xargs $(CC) -MM | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list
env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)

install:
	rm -rvf $(INDIR)/alloc
	cp -v $(OUTPUT)/$(TARGET) $(LIBDIR)/$(TARGET)
	cp -vr src/alloc $(INDIR)

clean: 
	rm -rvf $(OUTPUT)
	rm -vf $(CACHE)/*.o



