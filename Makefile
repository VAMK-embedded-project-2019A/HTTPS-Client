GPP=g++
FLAGS=-Wall
	
main: main.cpp httpsclient.cpp
	$(GPP) $(FLAGS) main.cpp httpsclient.cpp -o main -lssl -lcrypto
	
sslclient: SSLClient.cpp
	$(GPP) $(FLAGS) SSLClient.cpp -o sslclient -lssl -lcrypto -ljsoncpp -lcurl
	
clean:
	rm -f main sslclient