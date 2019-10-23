SFML=$(shell pkg-config --libs sfml-all)
OPENCV=$(shell pkg-config --libs opencv)
LIBS=$(SFML) -lpthread $(OPENCV) -lX11
OUTPUT=lug-fishtank

all: 
	@echo "** Building the fishtank **"
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)

go: 
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS) && ./lug-fishtank
release:
	@echo "** Building the fishtank for release **"
	g++ -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)

install:
	@echo "** Installing the fishtank **"
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

uninstall:
	@echo "** Uninstalling the fishtank **"
	rm /usr/bin/$(OUTPUT)
