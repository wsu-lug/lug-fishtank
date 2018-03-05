LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lpthread
OUTPUT=lug-fishtank

all: 
	@echo "** Building the fishtank **"
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)

release:
	@echo "** Building the fishtank for release **"
	g++ -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)

install:
	@echo "** Installing the fishtank **"
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

uninstall:
	@echo "** Uninstalling the fishtank **"
	rm /usr/bin/$(OUTPUT)
