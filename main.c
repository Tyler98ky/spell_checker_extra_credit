// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PORT 12313
#define NUM_WORKERS 23

void* writeToServer(void* arg);

int sock = 0;
struct sockaddr_in serv_addr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t busy = PTHREAD_COND_INITIALIZER;
pthread_cond_t free = PTHREAD_COND_INITIALIZER;

int main(int argc, char const *argv[]) {

    pthread_t threadPool[NUM_WORKERS];
    int threadIDs[NUM_WORKERS];
    printf("Launching threads.\n");
    for (int i = 0; i < NUM_WORKERS; i++) {
        threadIDs[i] = i;
        pthread_create(&threadPool[i], NULL, &writeToServer, &threadIDs[i]);
    }
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(threadPool[i], NULL);
    }

    printf("Threads launched. Now listening for incoming requests...\n");

    // TODO create threads and dispatch them
    return 0;
}

void* writeToServer(void* arg) {
    pthread_mutex_lock(&mutex);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    const char *word = "multithreaded";


    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    send(sock, word, strlen(word), 0);
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
//        pthread_cond_signal(&busy);
        pthread_mutex_unlock(&mutex);
}