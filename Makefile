
TOP := .

LDFLAGS := -lpthread -rdynamic -lrt
SUBDIRS = hello_pcap magic_sighandler life_hacker uds_console_example 

include $(TOP)/Makefile.all
