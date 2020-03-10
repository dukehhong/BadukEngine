#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <netinet/in.h>
#include <sys/time.h>

#include<curl/curl.h>
#include "board.h"

#define PORT 8080
#define MAX_CLIENTS 10

int main(){
    int opt = true;
    int master_socket, addrlen, new_socket, activity, valread, sd, max_sd;
    int client_socket[MAX_CLIENTS] = {0};
    char buffer[1024];

    std::string message = "";

    fd_set readfds;

    struct sockaddr_in address;
    
    //create master socket
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //research this
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        perror("setsocketopt failed");
        exit(EXIT_FAILURE);
    }

    //type of socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //bind socket to localhost port
    if(bind(master_socket, (sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Listening on port " << PORT << std::endl;

    //specify maximum of N pending connections 
    if(listen(master_socket, 3) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    std::cout << "Waiting for connections ..." << std::endl;

    while(true){
        //clear socket set
        FD_ZERO(&readfds);

        //add master_socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for(int i = 0; i< MAX_CLIENTS; i++){
            //socket descriptor
            sd = client_socket[i];
            
            //if valid sd, add to read list
            if(sd > 0 ){
                FD_SET(sd, &readfds);
            }

            //highest fd number
            if(sd > max_sd){
                max_sd = sd;
            }
        }

        //wait for activity on one of the sockets with no timeout
        activity = select(max_sd+1, &readfds, NULL, NULL, NULL);

        if((activity < 0) && (errno!=EINTR)){
            perror("select error");
        }

        //if something changed on master socket, then its an incoming connection
        if(FD_ISSET(master_socket, &readfds)){
            if((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            //print out connection info
            std::cout << "New connection, socket fd=" << new_socket << ", IP: " << inet_ntoa(address.sin_addr)
            << " port: " << ntohs(address.sin_port) << std::endl;

            if( send(new_socket, &message, message.size(),0) != message.size()){
                perror("send failed");
            }

            //add new socket to array of sockets
            for (int i = 0; i < MAX_CLIENTS; i++){
                if (client_socket[i] == 0){
                    client_socket[i] = new_socket;
                    std::cout << "Adding to list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        //else then some IO operation on some other socket
        for(int i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];
            if(FD_ISSET(sd, &readfds)){
                //check if it was closing, and read incoming message
                if( (valread = read(sd, buffer, 1024)) < 0){
                    //someone disconnected, get the person's details
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    std::cout << "Host disconnected, IP: " << inet_ntoa(address.sin_addr) << " port: " << ntohs(address.sin_port)
                    << std::endl;
                    close(sd);
                    client_socket[i] = 0;
                }
                
                else{
                    buffer[valread] = '\0';
                    send(client_socket[i], buffer, strlen(buffer), 0);
                }
            }
        }
 
    }
    return 0;
}