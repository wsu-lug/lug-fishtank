LIBS= -lpthread
OUTPUT=lug-fishtank



all: 
	@echo "** Building the fishtank **"
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)


mac:
	@echo " Building for a Mac "
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS) -framework SDL2 -framework SDL2_image -framework Cocoa -I/Library/Frameworks/SDL2.framework/Headers/ -I/Library/Frameworks/SDL2_image.framework/Headers/

release:
	@echo "** Building the fishtank for release **"
	g++ -std=c++14 *.cpp -o $(OUTPUT) $(LIBS)

install:
	@echo "** Installing the fishtank **"
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

uninstall:
	@echo "** Uninstalling the fishtank **"
	rm /usr/bin/$(OUTPUT)
