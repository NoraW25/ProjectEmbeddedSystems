
#include "CanSocket.h"


CanSocket* CanSocket::pointer_instance = 0;

CanSocket::CanSocket():
    ifname("can0")
{    
    system("ip link set can0 down");
    int result_settings = system("ip link set can0 up type can bitrate 500000");
    if (result_settings == -1) {
        perror("Fout bij het configureren van systeeminstellingen voor CAN.");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));

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

    struct can_frame& send_frame;

    

    ssize_t bytes = write(can_fd, &send_frame, sizeof(send_frame));

    if (bytes == sizeof(send_frame)) {
        return true; // OK
    }

    if (bytes < 0) {
        if (errno == EAGAIN) {
            return false; // buffer vol, later opnieuw
        }
        if (errno == ENETDOWN) {
            std::cout << "CAN interface down." << std::endl;
            return false;
        }
        std::cout << "CAN write error." << std::endl;
        return false;
    }
}

std::string CanSocket::received(){
    char buffer[64];
    int length = sprintf(buffer, "ID:%03X;DLC:%d;DATA:", frame.can_id, frame.can_dlc);

    for (int i = 0; i < frame.can_dlc; i++) {
        length += sprintf(buffer + length, "%02X", frame.data[i]);
    }

    return std::string(buffer);
    std::string received_message = std::string(buffer);

    memset(buffer, 0, sizeof(buffer));

    return received_message;
}

bool CanSocket::canSend(){
    return can_fd;
}

bool CanSocket::hasReceived(){
    if (can_fd <= 0){
        std::cout<<"Error: Geen CAN-socket aanwezig."<<std::endl;
        return false;
    }

    ssize_t bytes = read(can_fd, &frame, sizeof(frame));

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

    return true;
}

