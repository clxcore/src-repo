CC := gcc
LD := $(CC)
RM  := rm -rf

CFLAGS  := -Wall -std=c89 -g #-DLOG_ALL_OFF
LDFLAGS := 

LIBS :=

TARGET := main
OBJS := main.o log.o

INCD :=
LIBD :=

CFLAGS  += $(INCD)
LDFLAGS += $(LIBD)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(strip $(LD) $(LDFLAGS) -o $@ $^ $(LIBS))

$(OBJS): %.o: %.c
	$(strip $(CC) $(CFLAGS) -c $<)

.PHONY: clean
clean:
	$(strip $(RM) $(TARGET) $(OBJS))

log.o: log.c log.h
