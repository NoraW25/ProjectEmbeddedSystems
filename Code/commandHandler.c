#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

typedef int (*CommandFunc)(char* args);
typedef struct {
    char commandName[20];
    CommandFunc commandFunc;
} Command;

int command_exit(char* str) {
    return -2;
}

int command_sendData(char* str) {
    printf("received: %s\n", str);
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
            else if(res == 0) {
                printf("failed to execute command %s\n", commandList[i].commandName);
            }
            return res;
        }
    }
    printf("command %s not found", commandBuffer);
    return -1;
}

int main() {
    char entireCommandBuffer[100];

    while (1) {
        printf("Enter command:\n\n> ");
        fflush(stdout);  // make sure prompt appears

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
            }
        }

        // Do other non-blocking work here if needed
    }

    printf("Exiting\n");
    return 0;
}