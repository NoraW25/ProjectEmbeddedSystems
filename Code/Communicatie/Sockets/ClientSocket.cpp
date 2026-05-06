#include "ClientSocket.h"


ClientSocket* ClientSocket::pointerInstantie = 0;

ClientSocket::ClientSocket():
    poort(8080),
    client_fd(-1),
    server_ip("145.52.127.222"),
    status(0){

    buffer[1024] = {0};
    
    // Aanmaken socket file
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_adres.sin_family = AF_INET;
    server_adres.sin_port = htons(poort);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_adres.sin_addr) <= 0) {
        std::cout << "\nAdres kon niet geladen worden\n" << std::endl;
    } else {
        if ((status = connect(client_fd, (struct sockaddr*)&server_adres, sizeof(server_adres))) < 0) {
            std::cout << "\nConnectie gefaald\n" << std::endl;
        }

        std::cout << "Client heeft connectie met de server " << poort << std::endl;
    }
}

ClientSocket::~ClientSocket(){
    // sluit de socket
    if (client_fd >= 0){
        close(client_fd);
    }
}

ClientSocket* ClientSocket::instantie() {
	if (pointerInstantie == 0) {
		pointerInstantie = new ClientSocket();
	}

	return pointerInstantie;
}

void ClientSocket::versturen(std::string bericht){
    if (kanVersturen()) {
        send(client_fd, bericht.c_str(), bericht.size(), 0);
        std::cout<<"In bericht versturen in if"<<std:endl;
    }
    
    std::cout<<"In bericht versturen"<<std:endl;
}

std::string ClientSocket::ontvangst(){
    std::string ontvangenbericht = std::string(buffer);

    memset(buffer, 0, sizeof(buffer));

    return ontvangenbericht;
}

bool ClientSocket::heeftOntvangen(){
    if (client_fd < 0) {
        return false;
    } 
    
    ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false; // geen data beschikbaar
        }
        perror("read");
        return false;
    }

    if (bytes == 0) {
        std::cout << "Client verbinding verbroken" << std::endl;
        close(client_fd);
        client_fd = -1;
        return false;
    }

    if(buffer[0] != 0) {
        return true; 
    }

    return false;
}

bool ClientSocket::kanVersturen(){
    if(status > 0){
        return true;
    } 

    return false;
}