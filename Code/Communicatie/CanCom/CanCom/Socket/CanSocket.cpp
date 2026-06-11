
#include "CanSocket.h"


CanSocket* CanSocket::pointer_instance = 0;

CanSocket::CanSocket():
    ifname("can0"),
    status(false)
{    
    canStartup();
}

CanSocket::CanSocket(std::string ifname):
    ifname(ifname),
    status(false)
{    
    canStartup();
}

void CanSocket::canStartup(){
    std::string system_text1 = "sudo ip link set " + ifname + " down";
    system(system_text1.c_str());
    std::string system_text2 = "sudo ip link set " + ifname + " up type can bitrate 500000 restart-ms 100";
    int result_settings = system(system_text2.c_str());
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
    strcpy(ifr.ifr_name, ifname.c_str());
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

CanSocket* CanSocket::instance(std::string ifname) {
	if (pointer_instance == 0) {
		pointer_instance = new CanSocket(ifname);
	}

	return pointer_instance;
}


void CanSocket::send(int id, std::vector<uint8_t> data){
    send_frame = {};

    send_frame.can_id = id;
    send_frame.can_dlc = data.size();

    for (int i = 0; i < data.size(); i++){
        send_frame.data[i] = data[i];
    }   

    sendOnCan();
}


bool CanSocket::canSend(){
    if (status == 1){
        return true;
    } else {
        return sendOnCan();
    }
}

bool CanSocket::received(int* address, std::vector<uint8_t>* data){
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

    *address = received_frame.can_id;

    for(int i = 0; i < received_frame.can_dlc; i++){
        data->push_back(received_frame.data[i]);
    }

    return true;
}


bool CanSocket::sendOnCan(){

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
