all:
	g++ main.cpp Shader.cpp Camera.cpp Object.cpp -o texture -I/usr/include/GL -I./soil/include -L./soil/lib -lglut -lGLEW -lGL -lSOIL 
	./texture