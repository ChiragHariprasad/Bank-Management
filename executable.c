#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <conio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 2048

// Structure definitions (same as original)
struct pass {
    char username[50];
    char password[50];
    int date, month, year;
    char pnumber[15];
    char adharnum[20];
    char fname[20];
    char lname[20];
    char fathname[20];
    char mothname[20];
    char address[50];
    char typeaccount[20];
};

struct money {
    char usernameto[50];
    char userpersonfrom[50];
    long int money1;
};

void create_success_response(char* buffer, int success) {
    sprintf(buffer, "{\"success\": %s}", success ? "true" : "false");
}


void create_balance_response(char* buffer, const char* username) {
    FILE* fm;
    struct money m1;
    long int summoney = 0;

    // Initialize JSON response
    strcpy(buffer, "{\"transactions\": [");

    fm = fopen("mon.txt", "rb");
    if (fm == NULL) {
        strcpy(buffer, "{\"error\": \"Error opening file\"}");
        return;
    }

    int first = 1;
    while (fread(&m1, sizeof(m1), 1, fm)) {
        if (strcmp(username, m1.usernameto) == 0) {
            if (!first) {
                strcat(buffer, ",");
            }
            char transaction[200]; // Use sufficient size
            snprintf(transaction, sizeof(transaction),
                "{\"from\":\"%s\",\"amount\":%ld}", 
                m1.userpersonfrom, 
                m1.money1);
            strcat(buffer, transaction);
            summoney += m1.money1;
            first = 0;
        }
    }
    fclose(fm);

    // Append total balance and close JSON
    char total[100];
    snprintf(total, sizeof(total), "],\"total_balance\":%ld}", summoney);
    strcat(buffer, total);
}


int validate_login(const char* username, const char* password) {
    FILE* fp;
    struct pass u1;
    int valid = 0;
    
    fp = fopen("username.txt", "rb");
    if (fp == NULL) return 0;
    
    while (fread(&u1, sizeof(u1), 1, fp)) {
        if (strcmp(username, u1.username) == 0 &&
            strcmp(password, u1.password) == 0) {
            valid = 1;
            break;
        }
    }
    
    fclose(fp);
    return valid;
}

int transfer_money(const char* from_user, const char* to_user, long amount) {
    FILE *fm, *fp;
    struct pass u1;
    struct money m1;
    int found = 0;
    
    fp = fopen("username.txt", "rb");
    fm = fopen("mon.txt", "ab");
    
    if (fp == NULL || fm == NULL) {
        return 0;
    }
    
    while (fread(&u1, sizeof(u1), 1, fp)) {
        if (strcmp(to_user, u1.username) == 0) {
            found = 1;
            strcpy(m1.usernameto, u1.username);
            strcpy(m1.userpersonfrom, from_user);
            m1.money1 = amount;
            break;
        }
    }
    
    if (!found) {
        fclose(fp);
        fclose(fm);
        return 0;
    }
    
    fwrite(&m1, sizeof(m1), 1, fm);
    
    fclose(fp);
    fclose(fm);
    return 1;
}

int main() {
    WSADATA wsaData;
    SOCKET server_fd = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation error\n");
        WSACleanup();
        return 1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    printf("Server listening on port %d\n", PORT);
    
    while(1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }
        
        // Read client request
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            // Simple request parsing (you might want to use a proper JSON parser)
            char action[50] = {0};
            char username[50] = {0};
            char password[50] = {0};
            
            if (strstr(buffer, "\"action\":\"login\"")) {
                sscanf(buffer, "{\"action\":\"login\",\"username\":\"%[^\"]\",\"password\":\"%[^\"]\"}", 
                       username, password);
                
                int success = validate_login(username, password);
                char response[BUFFER_SIZE];
                create_success_response(response, success);
                printf("Sending response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                
            } else if (strstr(buffer, "\"action\":\"check_balance\"")) {
                sscanf(buffer, "{\"action\":\"check_balance\",\"username\":\"%[^\"]\"}", username);
                
                char response[BUFFER_SIZE];
                create_balance_response(response, username);
                printf("Sending response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
                
            } else if (strstr(buffer, "\"action\":\"transfer\"")) {
                char to_user[50];
                long amount;
                sscanf(buffer, "{\"action\":\"transfer\",\"from_user\":\"%[^\"]\",\"to_user\":\"%[^\"]\",\"amount\":%ld}",
                       username, to_user, &amount);
                
                int success = transfer_money(username, to_user, amount);
                char response[BUFFER_SIZE];
                create_success_response(response, success);
                printf("Sending response: %s\n", response);
                send(client_socket, response, strlen(response), 0);
            }
        }
        
        closesocket(client_socket);
    }
    
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
