
EXECUTABLE  = elevator

COMPILER    = clang
CFLAGS      = -Wall -g -std=gnu11 -fsanitize=address -pthread -Isource -Isource/driver
LDFLAGS     = -fsanitize=address
EXCLUDE     = '*test*'


SOURCEDIR = source
OBJECTDIR = build

SOURCES := $(patsubst $(SOURCEDIR)/%, %, $(shell find $(SOURCEDIR) -name '*.c'  ! -name $(EXCLUDE)  ! -path $(EXCLUDE)))
OBJECTS = $(addprefix $(OBJECTDIR)/, $(SOURCES:.c=.o))

TESTDIR	     = tests
TEST_SOURCES := $(wildcard $(TESTDIR)/*.c)
TEST_NAMES   := $(notdir $(TEST_SOURCES:.c=))
TEST_BINS    := $(addprefix $(OBJECTDIR)/tests/, $(TEST_NAMES))
PROJECT_OBJECTS := $(filter-out $(OBJECTDIR)/main.o, $(OBJECTS))

all: $(EXECUTABLE) 

rebuild: clean all
	
clean:
	rm -f $(EXECUTABLE)
	rm -rf $(OBJECTDIR)

test: $(TEST_BINS)
	@echo "Running tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test"; \
		$$test || exit 1; \
	done
	@echo "All tests passed!"

test_%: $(OBJECTDIR)/tests/test_%
	@echo "Running $<"
	@$<

$(OBJECTDIR)/tests/%: $(TESTDIR)/%.c $(PROJECT_OBJECTS)
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) $< $(PROJECT_OBJECTS) -o $@ $(LDFLAGS)

## executable depends on object files: link them
$(EXECUTABLE): $(OBJECTS) 
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## object files depend on source files: compile them
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS) $<

	
.PHONY: all rebuild clean

