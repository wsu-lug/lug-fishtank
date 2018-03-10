LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lX11
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
