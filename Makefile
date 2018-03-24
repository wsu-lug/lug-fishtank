LIBS= -lpthread -I/opt/vc/include -L/opt/vc/lib
LINUXLIBS= -lSDL2 -lSDL2_image 
OPENCV= -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
SDL_CONFIG := $(shell sdl2-config --cflags --libs)
OUTPUT=lug-fishtank



all: 
	@echo "** Building the fishtank **"
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS) $(LINUXLIBS) $(SDL_CONFIG) $(OPENCV)


mac:
	@echo " Building for a Mac "
	g++ -g -std=c++14 *.cpp -o $(OUTPUT) $(LIBS) -framework SDL2 -framework SDL2_image -framework Cocoa -I/Library/Frameworks/SDL2.framework/Headers/ -I/Library/Frameworks/SDL2_image.framework/Headers/

release:
	@echo "** Building the fishtank for release **"
	g++ -std=c++14 *.cpp -o $(OUTPUT) $(LIBS) $(LINUXLIBS)

install:
	@echo "** Installing the fishtank **"
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

uninstall:
	@echo "** Uninstalling the fishtank **"
	rm /usr/bin/$(OUTPUT)
test:
	g++ -std=c++14 test.cpp -o tester $(LIBS) $(LINUXLIBS)
