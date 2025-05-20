hepsi: derle calistir

derle:
	@echo "Derleniyor..."
	

	gcc -I include/ -c src/Planet.c       -o lib/Planet.o
	gcc -I include/ -c src/RockyPlanet.c  -o lib/RockyPlanet.o
	gcc -I include/ -c src/GasGiantPlanet.c -o lib/GasGiantPlanet.o
	gcc -I include/ -c src/IceGiantPlanet.c -o lib/IceGiantPlanet.o
	gcc -I include/ -c src/DwarfPlanet.c  -o lib/DwarfPlanet.o
	gcc -I include/ -c src/Person.c       -o lib/Person.o
	gcc -I include/ -c src/FileReader.c   -o lib/FileReader.o
	gcc -I include/ -c src/SpaceShip.c    -o lib/SpaceShip.o
	gcc -I include/ -c src/SimTime.c      -o lib/SimTime.o
	gcc -I include/ -c src/Simulation.c   -o lib/Simulation.o

	gcc -I include/ -o bin/Simulasyon \
	    lib/Planet.o lib/RockyPlanet.o lib/GasGiantPlanet.o \
	    lib/IceGiantPlanet.o lib/DwarfPlanet.o \
	    lib/Person.o lib/FileReader.o lib/SpaceShip.o lib/SimTime.o lib/Simulation.o \
	    src/main.c

calistir:
	@./bin/Simulasyon
