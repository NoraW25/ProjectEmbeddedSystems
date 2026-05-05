
#include "ServerSocket.h"


ServerSocket* ServerSocket::pointerInstantie = 0;

ServerSocket::ServerSocket():
    poort(8080),
    actieve_socket(-1),
    server_fd(-1),
    buffer({0}){
    
    // Aanmaken socket file
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    adres.sin_family = AF_INET;
    adres.sin_addr.s_addr = INADDR_ANY;
    adres.sin_port = htons(poort);

    if (bind(server_fd, (struct sockaddr*)&adres, sizeof(adres)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Maak server-socket non-blocking
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    std::cout << "Server luistert (non-blocking) op poort " << poort << std::endl;
}

ServerSocket::~ServerSocket(){
    // sluit de actieve socket
    if (actieve_socket >= 0){
        close(active_socket);
    }
  
    // sluit de luisterende socket
    if (server_fd >= 0){
        close(server_fd);
    }
}

ServerSocket* ServerSocket::instantie() {
	if (pointerInstantie == 0) {
		pointerInstantie = new ServerSocket();
	}

	return pointerInstantie;
}

void ServerSocket::versturen(std::string bericht){
    if (kanVersturen()) {
        send(actieve_socket, bericht.c_str(), bericht.size(), 0);
    }
}

std::string ServerSocket::ontvangst(){
    std::string ontvangenbericht = std::string(buffer);

    for(int i = 0; i < 1024 || buffer[i+1] = '\0'; i++){
        buffer[i] = '\0';
    }

    return ontvangenbericht;
}

bool ServerSocket::heeftOntvangen(){
    if (actieve_socket < 0) {
        return false;
    } 
    
    ssize_t bytes = read(actieve_socket, buffer, sizeof(buffer) - 1);

    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false; // geen data beschikbaar
        }
        perror("read");
        return false;
    }

    if (bytes == 0) {
        std::cout << "Client verbinding verbroken" << std::endl;
        close(actieve_socket);
        actieve_socket = -1;
        return false;
    }

    if(buffer[0] != 0) {
        return true; 
    }

    return false;
}

bool ServerSocket::kanVersturen(){
    if (actieve_socket >= 0) {
        return true;
    } else {
        return false;
    }        
}