
#include "CanSocket.h"


CanSocket* CanSocket::pointer_instance = 0;

CanSocket::CanSocket():
    ifname("can0"),
    key_id("ID:"),
    key_dlc("DLC:"),
    key_data("DATA:"),
    status(false)
{    
    system("ip link set can0 down");
    int result_settings = system("ip link set can0 up type can bitrate 500000");
    if (result_settings == -1) {
        perror("Fout bij het configureren van systeeminstellingen voor CAN.");
        return -1;
    }

    send_frame = {};
    received_frame = {};

    // Aanmaken socket file
    can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (can_fd < 0) {
        perror("Fout bij het openen van de socket.");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(can_fd, F_GETFL, 0);
    fcntl(can_fd, F_SETFL, flags | O_NONBLOCK);

    struct ifreq ifr{};
    strcpy(ifr.ifr_name, ifname);
    if(ioctl(s, SIOCGIFINDEX, &ifr) < 0){
        perror("Fout bij ioctl(SIOCGIFINDEX).");
        ~CanSocket();
        return;
    }

    socket_address.can_family = AF_CAN;
    socket_address.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
        perror("Fout bij bind()");
        ~CanSocket();
        return;
    }

    if(can_fd < 0){
        perror("Het opzetten van de CAN socket is mislukt.");
        ~CanSocket();
        return;
    }

    status = 1;
}

CanSocket::~CanSocket(){
    if (can_fd >= 0){
        close(can_fd);
    }
    pointer_instance = 0;
}

CanSocket* CanSocket::instance() {
	if (pointer_instance == 0) {
		pointer_instance = new CanSocket();
	}

	return pointer_instance;
}

void CanSocket::sendSocket(std::string message){
    send_frame.can_id = parseId(message);
    send_frame.can_dlc = parseDlc(message);
    
    std::vector<uint8_t> data = parseData(message);

    for (int i = 0; i < data.size(); i++){
        send_frame.data[i] = data[i];
    }

    send_on_can();
}

std::string CanSocket::received(){
    // Format -> ID:%d;DCL:%d;Data:%d;%d;%d;%d;
    std::string message = "";
    //if (hasReceived()){
        message += key_id + std::to_string(bufferReceivedBytes) + ";" + key_dlc + std::to_string(data.size()) + ";" + key_data;
        for (int i = 0; i < data.size(); i++){
            message += std::to_string(data[i]) + ";";
        }        
    //}
    return message;
}

bool CanSocket::canSend(){
    if (status = 1){
        return true;
    } else {
        return send_on_can();
    }
}

bool CanSocket::hasReceived(){
    // Voor algemeen gebruik in de CanReceiver
    if (can_fd <= 0){
        std::cout<<"Error: Geen CAN-socket aanwezig."<<std::endl;
        return false;
    }

    ssize_t bytes = read(can_fd, &received_frame, sizeof(received_frame));

    if (bytes < 0){
        if (errno == EAGAIN) {
            return false; // Geen data beschikbaar
        } else if (errno == ENETDOWN){
            std::cout<<"Error: CAN interface down."<<std::endl;
            return false;
        } else {
            std::cout<<"Ontvangen in CAN socket is fout gegaan."<<std::endl;
            return false;
        }
    }

    bufferReceivedAddress = received_frame.can_id;
    for(int i = 0; i < received_frame.can_dlc; i++){
        bufferReceivedBytes = received_frame.data[i];
    }

    return true;
}

int CanSocket::parseId(const std::string& message){
    size_t key = message.find(key_id) + key_id.length();
    size_t value = message.find(";", key);
    int id = std::stoi(message.substr(key, value - key));
    return id;
}

int CanSocket::parseDlc(const std::string& message){
    size_t key = message.find(key_dlc) + key_dcl.length();
    size_t value = message.find(";", key);
    int dlc = std::stoi(message.substr(key, value - key));
    return dlc;
}

std::vector<uint8_t> CanSocket::parseData(const std::string& message){
    std::vector<uint8_t> result;

    size_t position = message.find(key_data) + key_data.length();
    size_t next = message.find(";", position);

    while(next != std::string::npos){
        int value = std::stoi(message.substr(pos, next-pos));
        result.push_back((uint8_t) value);

        position = next + 1;
        next = message.find(";", position);
    }

    return result;
}

bool CanSocket::send_on_can(){
    ssize_t bytes = write(can_fd, &send_frame, sizeof(send_frame));

    if (bytes == sizeof(send_frame)) {
        send_frame = {};
        status = 1;
        return true;
    }

    if (bytes < 0) {
        if (errno == EAGAIN) {
            std::cout<<"Error: CAN Buffer is vol."<<std::endl;
            status = 0;
            return false; // buffer vol, later opnieuw
        }
        if (errno == ENETDOWN) {
            std::cout << "Error: CAN interface down." << std::endl;
            status = 0;
            return false;
        }
        std::cout << "Error: CAN write error." << std::endl;
        return false;
    }    
}
