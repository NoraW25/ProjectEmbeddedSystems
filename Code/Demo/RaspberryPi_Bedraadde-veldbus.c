
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>


volatile int blijvenLezen = 1;


void intHandler(int dummy)
{
    blijvenLezen = 0;
    printf("\nCTRL-C ontvangen. Programma stopt...\n");
}



int open_can_socket(const char *ifname)
{
    int canSocket;
    struct ifreq ifr;
    struct sockaddr_can socketAdres;

    // Maak raw CAN socket
    canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canSocket < 0) {
        perror("Fout bij het openen van de socket.");
        return -1;
    }

    // Interface naam kopiëren naar ifreq struct
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(canSocket, SIOCGIFINDEX, &ifr) < 0) {
        perror("Fout bij ioctl(SIOCGIFINDEX).");
        close(canSocket);
        return -1;
    }

    // CAN‑adres instellen
    socketAdres.can_family  = AF_CAN;
    socketAdres.can_ifindex = ifr.ifr_ifindex;

    // Socket binden
    if (bind(canSocket, (struct sockaddr *)&socketAdres, sizeof(socketAdres)) < 0) {
        perror("Fout bij bind()");
        close(canSocket);
        return -1;
    }

    return canSocket;
}



int verzend_can_frame(int socket)
{
    struct can_frame frame;

    frame.can_id  = 410;   // 11-bit ID
    frame.can_dlc = 0;       // 8 bytes data

    frame.data[0] = 12;
    frame.data[1] = 34;
    //frame.data[2] = 0x30;
    //frame.data[3] = 0x40;
    //frame.data[4] = 0x50;
    //frame.data[5] = 0x60;
    //frame.data[6] = 0x70;
    //frame.data[7] = 0x80;

    int bytes = write(socket, &frame, sizeof(frame));

    if (bytes != sizeof(frame)) {
        perror("Kon CAN frame niet versturen");
        return -1;
    }

    printf("CAN frame verzonden: ID=0x%03X, %d bytes\n", frame.can_id, frame.can_dlc);
    return 0;
}

int verzend_can_frame_buzzer(int socket)
{
    struct can_frame frame;

    frame.can_id  = 420;   // 11-bit ID
    frame.can_dlc = 0;       // 8 bytes data

    int bytes = write(socket, &frame, sizeof(frame));

    if (bytes != sizeof(frame)) {
        perror("Kon CAN frame niet versturen");
        return -1;
    }

    printf("CAN frame verzonden: ID=0x%03X, %d bytes\n", frame.can_id, frame.can_dlc);
    return 0;
}



int lees_can_frames(int socket)
{
    struct can_frame frame;
    int aantalBytes;

    printf("Wachten op CAN berichten...\n");

    while (blijvenLezen) {
        aantalBytes = read(socket, &frame, sizeof(struct can_frame));

        if(!blijvenLezen) {
            printf("Stoppen met lezen van CAN frames.\n");
            break;
        }

        if (aantalBytes < 0) {
            perror("Fout bij read().");
            return -1;
        }

        if (aantalBytes < sizeof(struct can_frame)) {
            fprintf(stderr, "Onvolledig CAN frame ontvangen.\n");
            continue;
        }

        printf("\nOntvangen CAN frame:\n");
        printf("  ID     : 0x%03X\n", frame.can_id & CAN_EFF_MASK);
        printf("  DLC    : %d\n", frame.can_dlc);
        printf("  Data   : ");

        //verzend_can_frame_buzzer(socket);

        for (int i = 0; i < frame.can_dlc; i++)
            printf("%02X ", frame.data[i]);

        printf("\n");
    }

    return 0;
}

// Bij het opstarten van de Pi eerst deze commando geven:
//sudo ip link set can0 up type can bitrate 500000 restart-ms 100
int main()
{
    const char *ifname = "can0";

    // Socket openen
    int socket = open_can_socket(ifname);
    if (socket < 0){
        return 1;
    }

    // Verstuur frame
    verzend_can_frame(socket);


    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);


    // Start CAN ontvangt lus
    lees_can_frames(socket);

    close(socket);
    printf("Programma netjes afgesloten.\n");
    return 0;
}
