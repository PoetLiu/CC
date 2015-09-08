TARGET=sort.out
DEPEND=data.in
CLEAN_OBJ=data.in data.out
include $(PROG)/make.inc

$(DEPEND):
	make -C data;
	./data/data.out
	ln -sf ./data/data.in ./data.in;

