#include "ClientSocket.h"


ClientSocket* ClientSocket::pointerInstantie = 0;

ClientSocket::ClientSocket():
    poort(8080),
    client_fd(-1),
    buffer({0}),
    server_ip("145.52.127.222"),
    status(0){
    
    // Aanmaken socket file
    server_fd = socket(AF_INET, SOCK_STREAM, 0)
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_adres.sin_family = AF_INET;
    server_adres.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &server_adres.sin_addr) <= 0) {
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
    }
}

std::string ClientSocket::ontvangst(){
    std::string ontvangenbericht = std::string(buffer);

    for(int i = 0; i < 1024 || buffer[i+1] = '\0'; i++){
        buffer[i] = '\0';
    }

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