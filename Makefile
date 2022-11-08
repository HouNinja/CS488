# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := cs488-framework imgui lodepng

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

cs488-framework: 
	@echo "==== Building cs488-framework ($(config)) ===="
	@${MAKE} --no-print-directory -C build -f cs488-framework.make

imgui: 
	@echo "==== Building imgui ($(config)) ===="
	@${MAKE} --no-print-directory -C build -f imgui.make

lodepng: 
	@echo "==== Building lodepng ($(config)) ===="
	@${MAKE} --no-print-directory -C build -f lodepng.make

clean:
	@${MAKE} --no-print-directory -C build -f cs488-framework.make clean
	@${MAKE} --no-print-directory -C build -f imgui.make clean
	@${MAKE} --no-print-directory -C build -f lodepng.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   cs488-framework"
	@echo "   imgui"
	@echo "   lodepng"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
