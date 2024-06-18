CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Ofast
LIBS= -lthr 


INCLUDE_PATH=
LIB_PATH=


ifeq ($(UNAME), Linux)	
	INCLUDE_PATH+=/usr/include/
	LIB_PATH+=/usr/lib64/
else
	INCLUDE_PATH+=/usr/include/
	LIB_PATH+=/usr/lib/
endif


TARGET=liballoc.a
CACHE=.cache
OUTPUT=$(CACHE)/release


MODULES += stack_alloc.o
MODULES += free_list_alloc.o
MODULES += sys_alloc.o


TEST += test.o


OBJ=$(addprefix $(CACHE)/,$(MODULES))
T_OBJ=$(addprefix $(CACHE)/,$(TEST))


all: env $(OBJ)
	ar -crs $(OUTPUT)/$(TARGET) $(OBJ)


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


exec: all $(T_OBJ)
	$(CC) $(T_OBJ) $(OBJ) $(LIBS) -o $(OUTPUT)/test
	$(OUTPUT)/test


.PHONY: env dep clean


dep:
	$(CC) -MM test/*.c src/*.c | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list


env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)

install:
	cp -v $(OUTPUT)/$(TARGET) $(LIB_PATH)/$(TARGET)
	cp -vr src/alloc $(INCLUDE_PATH)

clean: 
	rm -rvf $(OUTPUT)
	rm -vf $(CACHE)/*.o



