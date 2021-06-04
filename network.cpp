/* COMP2004-W2 - Solomon Cammack (solo) - 10613265 */
/* Network: builds and sends HTTP requests to my remote server */

#include "mbed.h"
#include "sensors.h"
#include "serial.h"
#include <string>

NetworkInterface *net = NetworkInterface::get_default_instance();

EventQueue queue_network;
Timeout networkTimeout;

void network_timeout_call() {
    if (net) {
        net->disconnect();
        log(true, "Network timeout");
    }
}


extern "C" void mbed_mac_address(char *mac);
 
int get_mac() {
    char mac[6];
    mbed_mac_address(mac);
    for(int i=0; i<6;i++) {
        printf("%02X ", mac[i]);
    }
    printf("\n");
    return 0;
}
 

int send_request(char method[], char url[], char headers[], char data[]) {
    printf("[DEBUG] network: request starting\n");

    if (!net) {
        // check if network device is attached;
        log(true, "Error: no network interface");
        return 1;
    }
    
    int result;
    result = net->connect();

    // NSAPI_ERROR_OK = no error, so these check if there's no error

    if (result != NSAPI_ERROR_OK) {
        // printf("Error! net->connect() returned: %d\n", result);
        log(true, "Error opening network connection");
        net->disconnect();
        return result;
    }

    SocketAddress address;
    result = net->gethostbyname("weather.slmn.io", &address);

    char logInfo[100] = "IP of remote server: ";
    strcat(logInfo,(address.get_ip_address()));
    log(false, logInfo);

    if (result != NSAPI_ERROR_OK) {
        net->disconnect();
        // printf("Error resolving hostname %d\n", result);
        log(true, "Error resolving hostname");
        return result;
    }

    address.set_port(8921);
    TCPSocket socket;

    char buffer[1024] = "";// = "POST /data HTTP/1.1\r\nHost: weather.slmn.io\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 30\r\n\r\ndata=poggers&no_data=unpoggers";

    strcat(buffer, method);
    strcat(buffer, " ");
    strcat(buffer, url);
    strcat(buffer, " HTTP/1.1\r\nHost: weather.slmn.io\r\n");
    strcat(buffer, headers);
    if (strcmp(method, "POST") == 0) {
        char _temp_buffer[8];
        sprintf(_temp_buffer, "%d", strlen(data));
        strcat(buffer, "\r\nContent-Length: ");
        strcat(buffer, _temp_buffer);
    }
    strcat(buffer, "\r\n\r\n");
    strcat(buffer, data);

    // printf("%s", "pre-buffer-print");

    // printf("%s", buffer);

    // printf("%s", "post buffer-print");

    if (socket.open(net) != 0) {
        // printf("socket open error");
        log(true, "Socket open error");
    }

    if (socket.connect(address) != 0) {
        // printf("socket connection error");
        log(true, "Socket connection error");
    }

    nsapi_size_t size = strlen(buffer);
     while(size) {
        result = socket.send(buffer+result, size);

        if (result < 0) {
            // printf("Error! socket.send() returned: %d\n", result);
            log(true, "Socket send error");
            socket.close();
            net->disconnect();
            return 1;
        }
        size -= result;
        printf("[DEBUG] network: sent %d bytes\n", result);
    }

    char rbuffer[256];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    // printf("recv %d [%s]\n", rcount, strstr(rbuffer, "\r\n\r\n") +4);
    printf("[DEBUG] network: recv %d bytes\n", rcount);

    socket.close();
    net->disconnect();


    return 0;
}

int send_data(string data) {
    char data_char[1024];
    strcpy(data_char, data.c_str());
    int req = send_request("POST", "/data", "Content-Type: application/x-www-form-urlencoded", data_char);
    // networkTimeout.attach(&network_timeout_call, 6s);
    return req;
}

void prefilled_request() {
    send_request("POST", "/data", "Content-Type: application/x-www-form-urlencoded", "data=poggers");
}
/*
void network_main() {
    get_mac();
    queue_network.call_every(5s, &prefilled_request);
    queue_network.dispatch();
};
*/