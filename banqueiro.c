#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t mutex;

void print_state() {
    printf("Available: ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");
}

bool is_safe_state() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];
    
    bool found;
    do {
        found = false;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                
                if (possible) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);
    
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        if (!finish[i])
            return false;
    return true;
}

int request_resources(int customer_num, int request[]) {
    pthread_mutex_lock(&mutex);
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }
    
    if (is_safe_state()) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }
    
    pthread_mutex_unlock(&mutex);
    return -1;
}

int release_resources(int customer_num, int release[]) {
    pthread_mutex_lock(&mutex);
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (release[i] > allocation[customer_num][i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }
    
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* customer_thread(void* arg) {
    int customer_num = *(int*)arg;
    
    while (1) {
        int request[NUMBER_OF_RESOURCES];
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            request[i] = rand() % (need[customer_num][i] + 1);
        }
        
        printf("Cliente %d tentando requisitar: ", customer_num);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
            printf("%d ", request[i]);
        printf("\n");
        
        if (request_resources(customer_num, request) == 0) {
            printf("Cliente %d: Requisição ACEITA\n", customer_num);
            printf("Cliente %d fez requisição: ", customer_num);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%d ", request[i]);
            printf("\n");
            print_state();
            sleep(1);
            
            if (release_resources(customer_num, request) == 0) {
                printf("Cliente %d liberou: ", customer_num);
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                    printf("%d ", request[i]);
                printf("\n");
                print_state();
            }
        } else {
            printf("Cliente %d: Requisição NEGADA\n", customer_num);
            print_state();
        }
        
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != NUMBER_OF_RESOURCES + 1) {
        printf("Usage: %s <resource1> <resource2> <resource3>\n", argv[0]);
        return 1;
    }
    
    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    }
    
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            need[i][j] = maximum[i][j];
            allocation[i][j] = 0;
        }
    }
    
    pthread_t threads[NUMBER_OF_CUSTOMERS];
    int customer_ids[NUMBER_OF_CUSTOMERS];
    
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        customer_ids[i] = i;
        pthread_create(&threads[i], NULL, customer_thread, &customer_ids[i]);
    }
    
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
