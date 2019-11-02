GPP=g++
FLAGS=-Wall
	
main: main.cpp httpsclient.cpp songinfoparser.cpp
	$(GPP) $(FLAGS) main.cpp httpsclient.cpp songinfoparser.cpp -o main -lssl -lcrypto -ljsoncpp
	
sslclient: SSLClient.cpp
	$(GPP) $(FLAGS) SSLClient.cpp -o sslclient -lssl -lcrypto -ljsoncpp -lcurl
	
clean:
	rm -f main sslclient