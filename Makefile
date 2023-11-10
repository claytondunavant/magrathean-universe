sources = \
	src/main.cpp
libs = \
	-lGL \
	-lglut
outname = magrathean-universe

all:
	g++ -std=c++17 $(sources) $(libs) -o $(outname)
clean:
	rm $(outname)
