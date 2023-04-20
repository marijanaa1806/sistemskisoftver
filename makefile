AS_SOURCES = src/asembler.cpp\
	src/lekser.cpp\
	src/parser.cpp\
	src/sekcija.cpp\
	src/backpatch.cpp\
	src/tabelaRel.cpp\
	src/tabelaSim.cpp\
	src/mainAs.cpp

LD_SOURCES = src/sekcija.cpp\
	src/tabelaRel.cpp\
	src/tabelaSim.cpp\
	src/linker.cpp\
	src/mainLink.cpp

EM_SOURCES = src/emulator.cpp\
	src/mainEm.cpp

all: assembler linker emulator

assembler: ${AS_SOURCES}
	g++ -g -o ${@} ${^}

linker: ${LD_SOURCES}
	g++ -g -o ${@} ${^}

emulator: ${EM_SOURCES}
	g++ -g -o ${@} ${^}

clean:
	rm -f assembler
	rm -f linker
	rm -f emulator