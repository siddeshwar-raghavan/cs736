### USERS SHOULD NOT NEED TO EDIT THIS FILE ###

# See Makefile in Examples directory for variable definitions

TARGETS = $(BINDIR)/TweetAnalysis_FE \
          $(BINDIR)/TweetAnalysis_BE \

TARGETS_SO = $(LIBDIR)/TweetAnalysisFilter.so

default: all

.PHONY: all clean install

all: $(TARGETS) $(TARGETS_SO)

$(BINDIR)/TweetAnalysis_FE: TweetAnalysis_FE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ TweetAnalysis_FE.C $(LIBS)

$(BINDIR)/TweetAnalysis_BE: TweetAnalysis_BE.C
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ TweetAnalysis_BE.C $(LIBS)

$(LIBDIR)/TweetAnalysisFilter.so: TweetAnalysisFilter.C
	$(CXX) $(CXXFLAGS) $(SOFLAGS) -o $@ TweetAnalysisFilter.C

clean:
	$(RM) -f $(TARGETS) $(TARGETS_SO)

install: $(TARGETS) $(TARGETS_SO)
	for libso in $(TARGETS_SO) ; do \
	    $(INSTALL) -m 755 $$libso $(ILIBDIR)/ ; \
	done
	for exe in $(TARGETS) ; do \
	    $(INSTALL) -m 755 $$exe $(IBINDIR)/ ; \
	done
