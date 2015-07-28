include $(MAKEFILE_TOP_INCLUDE)

PCAP_SRC=libpcap-1.7.3

$(PCAP_SRC)/Makefile:
	(\
	cd $(PCAP_SRC); \
		./configure \
		--host=$(CROSS_PREFIX) \
		--with-pcap=linux\
		--prefix=/usr\
		 \
	)

install: $(PCAP_SRC)/Makefile
	make -C $(PCAP_SRC) DESTDIR=$(USERDIR)/ruijieclient/libpcap/_install install

clean:
	make -C $(PCAP_SRC) clean
	rm -rf _install
	rm -f $(PCAP_SRC)/Makefile
