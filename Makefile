make:
	g++ main.cpp tinyxml/tinyxml.cpp tinyxml/tinystr.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp -o main `Magick++-config --cppflags --cxxflags --ldflags --libs`