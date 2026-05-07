#include "ClientSocket.h"


ClientSocket::ClientSocket():
    poort(8080),
    client_fd(-1),
    server_ip("145.52.127.222"),
    status(-1){

    memset(buffer, 0, sizeof(buffer));
    
    // Aanmaken socket file
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_adres.sin_family = AF_INET;
    server_adres.sin_port = htons(poort);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_adres.sin_addr) <= 0) {
        std::cout << "Adres kon niet geladen worden\n";
        return;
    }

    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

    status = connect(client_fd, (struct sockaddr*)&server_adres, sizeof(server_adres));

    if (status == 0) {
        std::cout << "Direct verbonden met server" << std::endl;
    } else if (status < 0 && errno == EINPROGRESS) {
        std::cout << "Bezig met verbinden" << std::endl;
        status = 1;
    } else {
        std::cout << "Connectie gefaald" << std::endl;        
        status = -1;
        close(client_fd);
        client_fd = -1;
    }

    std::cout << "Client heeft connectie met de server " << poort << std::endl;
}

ClientSocket::~ClientSocket(){
    // sluit de socket
    if (client_fd >= 0){
        close(client_fd);
    }
}

void ClientSocket::versturen(std::string bericht){
    if (kanVersturen()) {
        int resultaat = send(client_fd, bericht.c_str(), bericht.size(), 0);
        if(resultaat < 0){
            std::cout<<"Error: bericht niet verzonden"<<std::endl;
        } else {
            std::cout << "Aantal verzonden bytes: " << resultaat << std::endl;
        }
    }
}

std::string ClientSocket::ontvangst(){
    std::string ontvangenbericht = std::string(buffer);

    memset(buffer, 0, sizeof(buffer));

    return ontvangenbericht;
}

bool ClientSocket::heeftOntvangen(){
    if (status != 0) {
        heeftVerbinding();

        if (status != 0){
            return false;
        }
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
    if(status == 0){
        return true;
    } else {
        return heeftVerbinding();
    }
}

bool ClientSocket::heeftVerbinding(){
    if (status == 0) {
        return true;
    }

    fd_set wfds; // Lijst met sockets
    FD_ZERO(&wfds); // Maak lijst leeg
    FD_SET(client_fd, &wfds); // Zet client_fd in de lijst

    struct timeval tijdinterval = {0, 0}; // wacht geen seconden tijdens het checken -> non blocking

    int result = select(client_fd + 1, NULL, &wfds, NULL, &tijdinterval); // Kijk of de verbinding is opgezet

    if (result > 0 && FD_ISSET(client_fd, &wfds)) {
        int err;
        socklen_t len = sizeof(err);
        getsockopt(client_fd, SOL_SOCKET, SO_ERROR, &err, &len); // Kijkt of de verbinding succesvol is opgezet

        if (err == 0) {
            status = 0; // verbonden
            std::cout << "Connectie voltooid\n";
            return true;
        } else {
            std::cout << "Connectie mislukt: " << strerror(err) << "\n";
            status = -1;
            close(client_fd);
            client_fd = -1;
            return false;
        }
    }

    return false;
}