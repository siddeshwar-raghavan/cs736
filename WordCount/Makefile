### USERS SHOULD NOT NEED TO EDIT THIS FILE ###

# See Makefile in Examples directory for variable definitions

TARGETS = $(BINDIR)/WordCount_FE \
          $(BINDIR)/WordCount_BE \

TARGETS_SO = $(LIBDIR)/WordCountFilter.so

default: all

.PHONY: all clean install

all: $(TARGETS) $(TARGETS_SO)

$(BINDIR)/WordCount_FE: WordCount_FE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ WordCount_FE.C $(LIBS)

$(BINDIR)/WordCount_BE: WordCount_BE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ WordCount_BE.C $(LIBS)

$(LIBDIR)/WordCountFilter.so: WordCountFilter.C
	$(CXX) $(CXXFLAGS) $(SOFLAGS) -o $@ WordCountFilter.C

clean:
	$(RM) -f $(TARGETS) $(TARGETS_SO)

install: $(TARGETS) $(TARGETS_SO)
	for libso in $(TARGETS_SO) ; do \
	    $(INSTALL) -m 755 $$libso $(ILIBDIR)/ ; \
	done
	for exe in $(TARGETS) ; do \
	    $(INSTALL) -m 755 $$exe $(IBINDIR)/ ; \
	done
