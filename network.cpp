#include "mbed.h"
#include "sensors.h"

NetworkInterface *net = NetworkInterface::get_default_instance();

EventQueue queue_network;


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
 

int send_req() {
    printf("network service starting\n");
    if (!net) {
        printf("Error! No network inteface found.\n");
        return 1;
    }
    
    int result;

    result = net->connect();

    if (result != NSAPI_ERROR_OK) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    SocketAddress address;
    result = net->gethostbyname("weather.slmn.io", &address);
    if (result != NSAPI_ERROR_OK) {
        printf("Error resolving hostname %d\n", result);
        return result;
    }

    address.set_port(8921);
    TCPSocket socket;

    char buffer[] = "GET / HTTP/1.1\r\nHost: weather.slmn.io\r\n\r\n";

    if (socket.open(net) != 0) {
        printf("socket open error");
    }

    if (socket.connect(address) != 0) {
        printf("socket connection error");
    }

    nsapi_size_t size = strlen(buffer);
     while(size) {
        result = socket.send(buffer+result, size);

        if (result < 0) {
            printf("Error! socket.send() returned: %d\n", result);
            return 1;
        }
        size -= result;
        printf("sent %d [%.*s]\n", result, strstr(buffer, "\r\n")-buffer, buffer);
    }

    char rbuffer[256];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    printf("recv %d [%s]\n", rcount, strstr(rbuffer, "\r\n\r\n") +4);
    // printf("%s", strstr(rbuffer, "\r\n\r\n") +4);

    socket.close();
    net->disconnect();




    return 0;
}

int send_request(char method[], char url[], char headers[], char data[]) {
 printf("network service starting\n");
    if (!net) {
        printf("Error! No network inteface found.\n");
        return 1;
    }
    
    int result;

    result = net->connect();

    if (result != NSAPI_ERROR_OK) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    SocketAddress address;
    result = net->gethostbyname("weather.slmn.io", &address);
    if (result != NSAPI_ERROR_OK) {
        printf("Error resolving hostname %d\n", result);
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

    printf("%s", buffer);

    // printf("%s", "post buffer-print");

    if (socket.open(net) != 0) {
        printf("socket open error");
    }

    if (socket.connect(address) != 0) {
        printf("socket connection error");
    }

    nsapi_size_t size = strlen(buffer);
     while(size) {
        result = socket.send(buffer+result, size);

        if (result < 0) {
            printf("Error! socket.send() returned: %d\n", result);
            return 1;
        }
        size -= result;
        printf("sent %d [%.*s]\n", result, strstr(buffer, "\r\n")-buffer, buffer);
    }

    char rbuffer[256];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    // printf("recv %d [%s]\n", rcount, strstr(rbuffer, "\r\n\r\n") +4);
    printf("recv %d\n", rcount);

    socket.close();
    net->disconnect();


    return 0;
}

int send_data(string data) {
    char data_char[1024];
    strcpy(data_char, data.c_str());
    return send_request("POST", "/data", "Content-Type: application/x-www-form-urlencoded", data_char);
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