include $(MAKEFILE_TOP_INCLUDE)

CFLAGS+=-I$(SWITCH)/include/ -I$(SWITCH)/libzebos/include/ -I$(USERDIR)/libnl/include 
CFLAGS+=-I$(USERDIR)/app/include

LDFLAGS=$(SWITCH)/share/libnorouter.so -lm -L$(USERDIR)/openssl/openssl-1.0.1g/lib_install_rtk_8196d/lib/ -lcrypto
CGILDFLAGS=$(LDFLAGS) $(USERDIR)/app/cgi.so
libs+=ps.o json.o
app_sign=ruijie
dir=opk
target=$(app_sign).opk
app=$(app_sign)_app
cgi=$(app_sign)_cgi
ruijie_src=ruijieclient

src=$(wildcard *.c)
obj=$(src:.c=.o)

.PHONY: romfs all ruijieclient
romfs: all
	-cp -rf opk/* $(USERDIR)/rootfs/app/ruijie

all: $(app) $(cgi) ruijieclient
	rm -f $(target)
	mkdir -p $(dir)
	mkdir -p $(dir)/bin
	mkdir -p $(dir)/webs
	#app
	cp -rf $(app) $(dir)/bin/
	cp -rf $(cgi) $(dir)/webs/app.cgi
	cp -rf app.json $(dir)/
	cp -rf config $(dir)/
	cp -rf ruijie.png $(dir)/
	#ruijieclient
	cp -pP ruijieclient/libpcap/_install/usr/lib/libpcap.so.1.7.3 opk/bin/libpcap.so.1	
	cp -pP ruijieclient/ruijieclient-1.0-rc1/src/ruijieclient opk/bin/
	$(NOSPACK) $(dir) $(target)

$(app): $(obj) 
	$(CC) -o $@ -g $(app).o $(libs) $(LDFLAGS) $(CFLAGS) 

$(cgi): $(obj)
	$(CC) -o $@ $(cgi).o $(CGILDFLAGS) $(CFLAGS)

$(app).o:ruijie.h
$(cgi).o:ruijie.h
ps.o:ps.h
json.o:json.h

ruijieclient:
	make -C $(ruijie_src) romfs
	
clean:
	rm -rf *.o *.opk opk *~ $(app) $(cgi) 

distclean: clean
	make -C $(ruijie_src) clean
