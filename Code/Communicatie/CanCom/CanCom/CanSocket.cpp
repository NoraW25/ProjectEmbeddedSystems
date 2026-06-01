
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
    if (result_settings != 0) {
        perror("Fout bij het configureren van systeeminstellingen voor CAN.");
        return;
    } else {
        std::cout<<"Na result settings in can constructor"<<std::endl;
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
    if(ioctl(can_fd, SIOCGIFINDEX, &ifr) < 0){
        perror("Fout bij ioctl(SIOCGIFINDEX).");
        return;
    }

    socket_address.can_family = AF_CAN;
    socket_address.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
        perror("Fout bij bind()");
        return;
    }

    if(can_fd < 0){
        perror("Het opzetten van de CAN socket is mislukt.");
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
    send_frame = {};

    send_frame.can_id = parseId(message);

    std::cout<<"SendSocket na parse id & message"<<std::endl;
    
    std::vector<uint8_t> data = parseData(message);

    std::cout<<"SendSocket na parse data"<<std::endl;

    for (int i = 0; i < data.size(); i++){
        send_frame.data[i] = data[i];
    }
    
    send_frame.can_dlc = i;

    std::cout<<"Voor send on can"<<std::endl;
    send_on_can();
}

void CanSocket::setFrameCan(int address, int dlc, int data){
    send_frame.can_id = address;
    send_frame.can_dlc = 1;
    send_frame.data[0] = data;
}


std::string CanSocket::received(){
    // Format -> ID:%d;DCL:%d;Data:%d;%d;%d;%d;
    std::string message = "";
    message += key_id + std::to_string(bufferReceivedAddress) + ";" + 
        key_dlc + std::to_string(bufferReceivedBytes.size()) + ";" + 
        key_data;
    
    for (int i = 0; i < bufferReceivedBytes.size(); i++){
        message += std::to_string(bufferReceivedBytes[i]) + ";";
    }
    
    bufferReceivedBytes.clear();
    
    return message;
}

bool CanSocket::canSend(){
    if (status == 1){
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
        bufferReceivedBytes.push_back(received_frame.data[i]);
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
    size_t key = message.find(key_dlc) + key_dlc.length();
    size_t value = message.find(";", key);
    int dlc = std::stoi(message.substr(key, value - key));
    return dlc;
}

std::vector<uint8_t> CanSocket::parseData(const std::string& message){
    std::vector<uint8_t> result;

    std::cout << "In parseData" << std::endl;

    size_t position = message.find(key_data);   // "DATA:"
    if (position == std::string::npos){
        return result;
    }

    position += key_data.length();              // <-- NA "DATA:" beginnen
    size_t next = message.find(";", position);

    while (next != std::string::npos){
        std::string part = message.substr(position, next - position);
        std::cout << "Part " << part << std::endl;

        try {
            int value = std::stoi(part);
            result.push_back((uint8_t)value);
        } catch (const std::exception&) {
            std::cout << "value geen text" << std::endl;
        }

        position = next + 1;
        next = message.find(";", position);
    }

    return result;
}


bool CanSocket::send_on_can(){

    std::cout << "ID: " << std::hex << send_frame.can_id << std::endl;
    std::cout << "DLC: " << std::dec << (int)send_frame.can_dlc << std::endl;
    ssize_t bytes = write(can_fd, &send_frame, sizeof(send_frame));

    std::cout<<"Na write send_on_can"<<std::endl;
    if (bytes == sizeof(send_frame)) {
        send_frame = {};
        status = 1;
        return true;
    }

    if (bytes < 0) {
        std::cout << "CAN write error, errno=" << errno << std::endl;
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
    
    return false;
}
