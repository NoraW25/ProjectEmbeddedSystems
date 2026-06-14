#include "ClientSocket.h"

ClientSocket::ClientSocket(std::string ip) : 
    port(8080),
    client_fd(-1),
    server_ip(ip),
    status(-1),
    is_wemos(false),
    last_reconnection_time(std::time(nullptr)),
    translator(MessageTranslator::instance())
{
    tcpStartup();
}

ClientSocket::ClientSocket(std::string ip, bool wemos) : 
    port(8080),
    client_fd(-1),
    server_ip(ip),
    status(-1),
    is_wemos(wemos),
    last_reconnection_time(std::time(nullptr)),
    translator(MessageTranslator::instance())
{
    tcpStartup();
}

ClientSocket::ClientSocket(std::string ip, int port, bool wemos) : 
    port(port),
    client_fd(-1),
    server_ip(ip),
    status(-1),
    is_wemos(wemos),
    last_reconnection_time(std::time(nullptr)),
    translator(MessageTranslator::instance())
{
    tcpStartup();
}

void ClientSocket::tcpStartup()
{
    memset(buffer, 0, sizeof(buffer));

    // Aanmaken socket file
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        std::cout << "Adres kon niet geladen worden, ip: " << server_ip << std::endl;
        return;
    }

    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

    // Maak connectie met server
    status = connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address));

    if (status == 0) {
        std::cout << "Direct verbonden met server" << std::endl;
    } else if (status < 0 && errno == EINPROGRESS) {
        std::cout << "Bezig met verbinden, ip: " << server_ip << std::endl;
        status = 1;
    } else {
        std::cout << "Connectie gefaald, verbindt opnieuw, ip: " << server_ip << std::endl;
        status = -1;
        close(client_fd);
        client_fd = -1;
        reconnect();
    }
}

ClientSocket::~ClientSocket()
{
    // sluit de socket
    if (client_fd >= 0) {
        close(client_fd);
    }
}

void ClientSocket::send(int id, std::vector<uint8_t> data)
{
    if (canSend()) {
        std::string message = translator->translate(id, data);

        if (is_wemos) {
            message += "\n";
        }

        int result = ::send(client_fd, message.c_str(), message.size(), 0);
        if (result < 0) {
            std::cout << "Error: bericht niet verzonden" << std::endl;
        } else {
            std::cout << "Aantal verzonden bytes1: " << result << std::endl;
            std::cout << message << std::endl;
        }
    } else {
        if (status == -1 || client_fd == -1) {
            reconnect();
        }
    }
}

bool ClientSocket::received(int *id, std::vector<uint8_t> *data)
{
    if (status != 0 || client_fd < 0){
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
        std::cout << "Server heeft de verbinding gesloten, ip: " << server_ip << std::endl;
        reconnect();
        return false;
    }

    buffer[bytes] = '\0';

    if (buffer[0] != '\n') {
        translator->translate(id, data, std::string(buffer, bytes));
        return true;
    }

    return false;
}

bool ClientSocket::canSend()
{
    if (status == 0){
        return true;
    }

    if(client_fd < 0){
        return false;
    }

    fd_set wfds;              // Lijst met sockets
    FD_ZERO(&wfds);           // Maak lijst leeg
    FD_SET(client_fd, &wfds); // Zet client_fd in de lijst

    struct timeval timeinterval = {0, 0}; // wacht geen seconden tijdens het checken -> non blocking

    int result = select(client_fd + 1, NULL, &wfds, NULL, &timeinterval); // Kijk of de verbinding is opgezet

    if (result > 0 && FD_ISSET(client_fd, &wfds)){
        int err;
        socklen_t len = sizeof(err);
        getsockopt(client_fd, SOL_SOCKET, SO_ERROR, &err, &len); // Kijkt of de verbinding succesvol is opgezet

        if (err == 0) {
            status = 0; // verbonden
            std::cout << "Connectie voltooid, ip: " << server_ip << std::endl;
            return true;
        } else {
            std::cout << "Connectie mislukt: " << strerror(err) << ", ip: " << server_ip << std::endl;
            status = -1;
            close(client_fd);
            client_fd = -1;
            return false;
        }
    }

    return false;
}

void ClientSocket::reconnect()
{
    std::time_t time_now = std::time(nullptr);
    
    if (time_now - last_reconnection_time < 2){
        return; // Te weinig tijd voorbij (minder dan 2 sec), voorkomt spam en lag
    }

    last_reconnection_time = time_now;

    if (client_fd >= 0) {
        close(client_fd);
    }

    client_fd = -1;
    status = -1;

    std::cout << "Poging om opnieuw te verbinden, ip: " << server_ip << std::endl;
    tcpStartup();
}