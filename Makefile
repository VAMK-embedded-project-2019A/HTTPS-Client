GPP=g++
FLAGS=-Wall
	
main: main.cpp httpsclient.cpp songinfoparser.cpp sftpclient.cpp
	$(GPP) $(FLAGS) main.cpp httpsclient.cpp sftpclient.cpp songinfoparser.cpp -o main -lssl -lcrypto -ljsoncpp -lcurl
	
sslclient: SSLClient.cpp
	$(GPP) $(FLAGS) SSLClient.cpp -o sslclient -lssl -lcrypto -ljsoncpp -lcurl
	
clean:
	rm -f main sslclient