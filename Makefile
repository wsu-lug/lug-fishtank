SFML=$(shell pkg-config --libs sfml-all)
OPENCV=$(shell pkg-config --libs opencv)
LIBS=$(SFML) -lpthread $(OPENCV) -lX11
OUTPUT=lug-fishtank
OUTPUT_PATH=./bin/$(OUTPUT)

all: 
	@echo "** Building the fishtank **"
	g++ -g -std=c++14 *.cpp -o $(OUTPUT_PATH) $(LIBS)

go: 
	g++ -g -std=c++14 *.cpp -o $(OUTPUT_PATH) $(LIBS) && $(OUTPUT_PATH)
release:
	@echo "** Building the fishtank for release **"
	g++ -std=c++14 *.cpp -o $(OUTPUT_PATH) $(LIBS)

install:
	@echo "** Installing the fishtank **"
	cp $(OUTPUT_PATH) /usr/bin/$(OUTPUT)

uninstall:
	@echo "** Uninstalling the fishtank **"
	rm /usr/bin/$(OUTPUT)
