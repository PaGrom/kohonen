make:
	g++ main.cpp CSOM.cpp CSOMNode.cpp GitParser.cpp Maintainer.cpp -o main `Magick++-config --cppflags --cxxflags --ldflags --libs`