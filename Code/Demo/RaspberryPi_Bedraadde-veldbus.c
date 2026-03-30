#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>

volatile int blijvenLezen = 1;
volatile int globalCANsocket;

void intHandler(int dummy)
{
    blijvenLezen = 0;
    printf("\nCTRL-C ontvangen. Programma stopt...\n");
}

int open_can_socket(const char* ifname)
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
    socketAdres.can_family = AF_CAN;
    socketAdres.can_ifindex = ifr.ifr_ifindex;

    // Socket binden
    if (bind(canSocket, (struct sockaddr*)&socketAdres, sizeof(socketAdres)) < 0) {
        perror("Fout bij bind()");
        close(canSocket);
        return -1;
    }

    return canSocket;
}

int verzend_can_frame(int ID, int dataLen, int data[8])
{
    struct can_frame frame;

    frame.can_id = ID;   // 11-bit ID
    frame.can_dlc = dataLen;       // 8 bytes data

    printf("%d %d\n", ID, dataLen);
    for (int i = 0; i < dataLen; i++) {
        printf("%d\n", data[i]);
        frame.data[i] = data[i];
    }

    int bytes = write(globalCANsocket, &frame, sizeof(frame));

    if (bytes != sizeof(frame)) {
        perror("Kon CAN frame niet versturen");
        return -1;
    }

    printf("CAN frame verzonden: ID=0x%03X, %d bytes\n", frame.can_id, frame.can_dlc);
    return 0;
}

int lees_can_frames()
{
    struct can_frame frame;
    int aantalBytes;

    printf("Wachten op CAN berichten...\n");

    while (blijvenLezen) {
        aantalBytes = read(globalCANsocket, &frame, sizeof(struct can_frame));

        if (!blijvenLezen) {
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

        //verzend_can_frame(socket);

        for (int i = 0; i < frame.can_dlc; i++)
            printf("%02X ", frame.data[i]);

        printf("\n");
    }

    return 0;
}

typedef int (*CommandFunc)(char* args);
typedef struct {
    char commandName[20];
    CommandFunc commandFunc;
} Command;

int command_exit(char* str) {
    intHandler(0);
    return -2;
}

int command_sendData(char* str) {
    int addr;
    int data[8];
    int dataLen=0;

    sscanf(str, " %d", &addr);
    char* ptr = str + 1;
    while (*ptr != ' ') ptr++;
    while (*ptr == ' ') ptr++;

    for (int i = 0; i < 8; i++) {
        if (sscanf(ptr, " %d", &data[i]) != 1) break;
        dataLen++;
        // Move pointer past the number just read
        while (*ptr != ' ') ptr++;
        while (*ptr == ' ') ptr++;
        if (*ptr=='\0') break;
    }

    verzend_can_frame(addr, dataLen, data);

    return 1;
}

Command commandList[] = {
    {
        "exit",
        &command_exit
    },
    {
        "senddata",
        &command_sendData
    }
};

int processCommand(char* str) {
    //Returns: -1 if no command found, -2 if shutting down, 0 if command failed, 1 if command succeeded
    char commandBuffer[20];
    sscanf(str, "%s", commandBuffer);
    int commandLength = strlen(commandBuffer);
    char* remainder = str + commandLength;
    while (*remainder == ' ') {
        remainder += 1;
    }
    for (int i = 0; i < sizeof(commandList) / sizeof(Command); i++) {
        if (strcmp(commandBuffer, commandList[i].commandName) == 0) {
            int res = commandList[i].commandFunc(remainder);
            if (res > 0) {
                printf("succesfully executed command %s\n", commandList[i].commandName);
            }
            else if (res == 0) {
                printf("failed to execute command %s\n", commandList[i].commandName);
            }
            return res;
        }
    }
    printf("command %s not found", commandBuffer);
    return -1;
}

int main() {
    const char* ifname = "can0";
    // Socket openen
    globalCANsocket = open_can_socket(ifname);
    if (globalCANsocket < 0) {
        return 1;
    }

    // Verstuur frame
    //verzend_can_frame(socket);


    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);


    // Start CAN ontvangt lus
    //lees_can_frames(socket);

    //Command line code
    char entireCommandBuffer[100];

    printf("Enter command:\n\n> ");
    fflush(stdout);  // make sure prompt appears

    while (1) {

        // Set up select()
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;   // 0 seconds
        timeout.tv_usec = 100000; // 100ms -> loop every 0.1 sec

        int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ready > 0) {
            // input is available
            if (fgets(entireCommandBuffer, sizeof(entireCommandBuffer), stdin) != NULL) {
                entireCommandBuffer[strcspn(entireCommandBuffer, "\n")] = 0; // remove newline
                int res = processCommand(entireCommandBuffer);
                if (res == -2) break;
                printf("Enter command:\n\n> ");
                fflush(stdout);  // make sure prompt appears
            }
        }

        // Do other non-blocking work here if needed
    }

    printf("Exiting\n");
    close(globalCANsocket);//close CANBUS
    return 0;
}