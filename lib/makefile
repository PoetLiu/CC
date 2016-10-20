TARGET=mylib.so
INC_DIRS=$(shell find $(PROG)/include -type d)
INCLUDE_DIRS=$(foreach dir,$(INC_DIRS),-I$(dir))
CFLAGS+=$(INCLUDE_DIRS) -fPIC

DIRS=$(shell find $(PROG)/lib -type d)
SRC=$(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))

$(TARGET): $(PROG)/include $(SRC)
	$(CC) -o $@ $(CFLAGS) -shared $(SRC)  -lm

