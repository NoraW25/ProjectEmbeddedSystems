#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdint.h>
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

#define KLOK_ADDR 410
#define BUZZER_ADDR 420

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

int lees_can_frames_nonblocking(struct can_frame *frame)
{
    int aantalBytes;

    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(globalCANsocket, &readfds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0; // return immediately (polling)

    int ready = select(globalCANsocket + 1, &readfds, NULL, NULL, &timeout);

    if (ready < 0) {
        //perror("select() fout");
        return 0;
    }

    if (ready == 0) {
        // No data available
        return 0;
    }

    if (FD_ISSET(globalCANsocket, &readfds)) {
        aantalBytes = read(globalCANsocket, frame, sizeof(struct can_frame));

        if (aantalBytes < 0) {
            //perror("Fout bij read()");
            return 0;
        }

        if (aantalBytes < sizeof(struct can_frame)) {
            //fprintf(stderr, "Onvolledig CAN frame ontvangen.\n");
            return 0;
        }

        /*printf("  ID     : 0x%03X\n", *frame.can_id & CAN_EFF_MASK);
        printf("  DLC    : %d\n", *frame.can_dlc);
        printf("  Data   : ");

        for (int i = 0; i < *frame.can_dlc; i++)
            printf("%02X ", *frame.data[i]);

        printf("\n");*/
    }

    return 1;
}

typedef int (*CommandFunc)(char* args);
/**
 * @brief the name (alias) and function of a command
 */
typedef struct {
    char commandName[20];
    CommandFunc commandFunc;
} Command;

/**
 * @brief exit command.
 * 
 * This function shuts down running processes and returns -2, signifying to the main thread to exit any loops.
 * 
 * @param str string containing arguments for function
 * @return -2
 */
int command_exit(char* str) {
    intHandler(0);
    return -2;
}

/**
 * @brief undefined command.
 * 
 * This is a placeholder function for undefined commands, this function returns 0 and will thus cause an error.
 * 
 * @param str string containing arguments for function
 * @return 0
 */
int command_undefined(char* str) {
    return 0;
}

/**
 * @brief send data command.
 * 
 * This function sends a CAN message to the first number address in the argument string with up to 8 data bytes as successive arguments in the string.
 * 
 * @param str string containing arguments for function
 * @return 0 or 1
 */
int command_sendData(char* str) {
    int addr;
    int data[8];
    int dataLen=0;

    sscanf(str, " %d", &addr);
    char* ptr = str;
        while (*ptr != ' '&&*ptr!='\0') ptr++;
        while (*ptr == ' ') ptr++;

        for (int i = 0; i < 8; i++) {
            if (sscanf(ptr, " %d", &data[i]) != 1) break;
            dataLen++;
            // Move pointer past the number just read
            while (*ptr != ' ') ptr++;
            while (*ptr == ' ') ptr++;
            if (*ptr=='\0') break;
        }

    int res = verzend_can_frame(addr, dataLen, data);

    return res+1;
}

/**
 * @brief read data command.
 * 
 * This function enables or disables printouts of incoming CAN data.
 * 
 * @param str string containing arguments for function, must be on\0 or off\0
 * @return 0 or 1
 */
volatile int readoutIncomingCAN=0;
int command_enableReadoutData(char* str) {
    if (strcmp(str, "on")==0) {
        readoutIncomingCAN=1;
        return 1;
    }
    else if (strcmp(str, "off")==0) {
        readoutIncomingCAN=0;
        return 1;
    }
    return 0;
}

volatile int updateClockwSolar = 0;
int command_enableUpdateClockwSolar(char* str){
    if (strcmp(str, "on")==0) {
        updateClockwSolar=1;
        return 1;
    }
    else if (strcmp(str, "off")==0) {
        updateClockwSolar=0;
        return 1;
    }
    return 0;
}

/**
 * @brief start CAN command.
 * 
 * This function enables the CAN interface and re-opens the socket.
 * 
 * @param str string containing arguments for function (not used)
 * @return 0 or 1
 */
int command_startCAN(char* str) {
    system("ip link set can0 down");
    int res = system("ip link set can0 up type can bitrate 500000");
    if (res==-1) {
        return 0;
    }
    if (globalCANsocket > 0) close(globalCANsocket); // close if already open
    const char* ifname = "can0";
    globalCANsocket = open_can_socket(ifname);
    if (globalCANsocket < 0) {
        return 0;
    }
    return 1;
}

Command commandList[] = {
    {
        "exit",
        &command_exit
    },
    {
        "help",
        &command_undefined
    },
    {
        "senddata",
        &command_sendData
    },
    {
        "readdata",
        &command_enableReadoutData
    },
    {
        "enableSolarClock",
        &command_enableUpdateClockwSolar
    },
    {
        "startcan",
        &command_startCAN
    }
};

/**
 * @brief help command.
 * 
 * This function prints all of the available commands.
 * 
 * @param str string containing arguments for function
 * @return 1
 */
int command_help(char* str) {
    printf("----------------------\nCommands:\n\n");
    for (int i=0; i<sizeof(commandList)/sizeof(Command); i++) {
        printf("%d. %s\n", i+1, commandList[i].commandName);
    }
    printf("----------------------\n");
    return 1;
}

/**
 * @brief process command.
 * 
 * This function processes the entered string into commands.
 * 
 * @param str string containing the command and it's arguments seperated by spaces.
 * @return -1 if no command found, -2 if shutting down, 0 if command failed, 1 if command succeeded
 */

int processCommand(char* str) {
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


typedef int (*CANfunc)(struct can_frame *frame);
/**
 * @brief the id of a can message
 */
typedef struct {
    int canID;
    int reqRTR;
    CANfunc canFunc;
} CANbinding;

int process_solarPanel(struct can_frame *frame) {
    if(updateClockwSolar==0) return 0;
    int data[8];
    for (int i=0; i<frame->dlc;i++){
        data[i]=frame->data[i];
    }
    verzend_can_frame(KLOK_ADDR, 2, data);
    return 1;
}

CANbinding canBindings[] = {
    {
        0x310,
        0,
        &process_solarPanel
    }
};

int processCANframe(struct can_frame *frame) {
    for (int i = 0; i < sizeof(canBindings)/sizeof(CANbinding); i++) {
        uint32_t id;
        // Extract ID correctly
        if (frame->can_id & CAN_EFF_FLAG)
            id = frame->can_id & CAN_EFF_MASK;
        else
            id = frame->can_id & CAN_SFF_MASK;

        int isRTR = (frame->can_id & CAN_RTR_FLAG) != 0;

        if (canBindings[i].canID == id &&
            canBindings[i].reqRTR == isRTR) {
            return canBindings[i].canFunc(frame);
        }
    }
    return -1;
}

int main() {
    commandList[1].commandFunc = &command_help;//Set the help command to the help function, this function is defined later to use the length of the command array.

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

        struct can_frame frame;
        int res = lees_can_frames_nonblocking(&frame);
        
        if (res == 1) {
            if (readoutIncomingCAN==1) {
                printf("\nincoming can message:");
                printf("  ID     : 0x%03X\n", frame.can_id & CAN_EFF_MASK);
                printf("  DLC    : %d\n", frame.can_dlc);
                printf("  Data   : ");

                for (int i = 0; i < frame.can_dlc; i++)
                    printf("%02X ", frame.data[i]);

                printf("\n\n> ");
            }
            int procRes = processCANframe(&frame);
        }
    }

    printf("Exiting\n");
    close(globalCANsocket);//close CANBUS
    return 0;
}