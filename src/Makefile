Client:	Client.cpp Client-main.cpp
	g++ Client.cpp Client-main.cpp -o Client -lboost_system -lpthread

stdout_test:	stdout_test.cpp
	g++ stdout_test.cpp -o out_test -lboost_system -lpthread

ClientUI:	ClientUI.cpp ClientUI.hpp ClientUI-main.cpp
	g++ ClientUI.cpp ClientUI-main.cpp -o ClientUI `pkg-config --cflags --libs gtk+-2.0`
