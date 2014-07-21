# Makefile for my renderer library PaCaLib
# This is the Linux implementation
#

NAME                 =  PaCaLib_Base
VERS_MAJOR           =  0
VERS_MINOR           =  1

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

export SRCDIR        =  .

.PHONY: all
all: _everything

include $(SCRIPTDIR)/makelib

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

.PHONY: clean
clean:
	@echo " - Cleaning $(NAME)..."
	@rm -f $(BINDIR)/* $(OBJECTS) $(Y_CPPFILES) $(Y_HEADERS) $(DEPENDS)

