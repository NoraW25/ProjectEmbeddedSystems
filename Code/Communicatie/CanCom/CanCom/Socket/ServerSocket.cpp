
#include "ServerSocket.h"


ServerSocket* ServerSocket::pointerInstance = 0;

ServerSocket::ServerSocket():
    port(8080),
    active_socket(-1),
    server_fd(-1),
    translator(MessageTranslator::instance()){
    
    tcpStartup();    
}

ServerSocket::ServerSocket(int port):
    port(port),
    active_socket(-1),
    server_fd(-1),
    translator(MessageTranslator::instance()){
    
    tcpStartup();    
}

void ServerSocket::tcpStartup(){
    memset(buffer, 0, sizeof(buffer));
    
    // Aanmaken socket file
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
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

    std::cout << "Server luistert (non-blocking) op poort " << port << std::endl;
}

ServerSocket::~ServerSocket(){
    // sluit de actieve socket
    if (active_socket >= 0){
        close(active_socket);
    }
  
    // sluit de luisterende socket
    if (server_fd >= 0){
        close(server_fd);
    }
}

ServerSocket* ServerSocket::instance() {
	if (pointerInstance == 0) {
		pointerInstance = new ServerSocket();
	}

	return pointerInstance;
}

ServerSocket* ServerSocket::instance(int port) {
	if (pointerInstance == 0) {
		pointerInstance = new ServerSocket(port);
	}

	return pointerInstance;
}

void ServerSocket::send(int id, std::vector<uint8_t> data){
    if (canSend()) {
        std::string message = translator->translate(id, data);

        ::send(active_socket, message.c_str(), message.size(), 0);
    }
}

bool ServerSocket::canSend(){
    if (active_socket >= 0) {
        return true;
    } else {
        return false;
    }        
}

bool ServerSocket::received(int* id, std::vector<uint8_t>* data){
    if (active_socket < 0) {
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        if (new_socket >= 0) {
            std::cout << "Nieuwe client verbonden!" << std::endl;
            active_socket = new_socket;

            // Maak client-socket non-blocking
            int flags = fcntl(active_socket, F_GETFL, 0);
            fcntl(active_socket, F_SETFL, flags | O_NONBLOCK);
        } else {
            return false;
        }
    } 

    
    
    ssize_t bytes = read(active_socket, buffer, sizeof(buffer) - 1);
    //std::cout<<"heeftOntvangen na read()"<<std::endl;

    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false; // geen data beschikbaar
        }
        perror("read");
        return false;
    }

    if (bytes == 0) {
        std::cout << "Client verbinding verbroken" << std::endl;
        close(active_socket);
        active_socket = -1;
        return false;
    }

    if(buffer[0] != 0) {        
        if (buffer[0] != '\n'){
            // translate message to
        }
        return true;
    }

    return false;
}

