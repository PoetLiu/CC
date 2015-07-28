include $(MAKEFILE_TOP_INCLUDE)

RUIJIE_SRC=ruijieclient-1.0-rc1
RUIJIE_BIN=$(RUIJIE_SRC)/src/ruijieclient
PCAP_SRC=$(USERDIR)/ruijieclient/libpcap/
PCAP_DIR=$(PCAP_SRC)/_install/usr/

$(RUIJIE_SRC)/Makefile:
	(export CPPFLAGS="-I$(PCAP_DIR)/include -I$(SWITCH)/include -I$(SWITCH)/libzebos/include -I$(USERDIR)/app/ruijie";\
	 export LDFLAGS="$(SWITCH)/share/libnorouter.so -lm";\
	cd $(RUIJIE_SRC); \
		./configure \
		--host=$(CROSS_PREFIX) \
		 \
	)
.PHONY: build libpcap romfs clean

build: $(RUIJIE_SRC)/Makefile
	make -C $(RUIJIE_SRC)

libpcap:
	make -C $(PCAP_SRC) install

romfs: libpcap build
	$(STRIP) $(RUIJIE_BIN)
	#cp  $(PCAP_DIR)/lib/libpcap.so.1.7.3 $(ROMFSDIR)/lib/libpcap.so.1	
	#cp -f $(RUIJIE_BIN) $(ROMFSDIR)/bin/
clean:
	make -C $(PCAP_SRC) clean
	make -C $(RUIJIE_SRC) clean
	rm -f $(RUIJIE_SRC)/Makefile
