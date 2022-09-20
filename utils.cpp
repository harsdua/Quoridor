#include "utils.h"


#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>

#include <cstdio> //exec
#include <memory>
#include <stdexcept>
#include <array>

#include <vector>
#include <string>
#include <cstring>

using namespace std;

bool readMessage(int socket,Message& msg){
    int r;
    if (read(socket, &msg.size, sizeof(ssize_t)) == 0) return false;
    if (read(socket, &msg.timestamp, sizeof(time_t)) == 0) return false;

    msg.message = new char[msg.size+1];//static_cast<char *>(malloc(sizeof(char) * msg.size + 1));
    msg.message[msg.size] = '\0';

    int readBytes = 0;
    while (readBytes < ((int)msg.size)) {
        if ((r = read(socket, &msg.message[readBytes], msg.size - readBytes)) == -1) {
            delete[] msg.message;
            msg.message = nullptr;
            return false;
        }
        readBytes += r;
    }

    return true;
}

void sendMessage(int socket, Message& msg){
    send(socket , &msg.size , sizeof(ssize_t) , 0 );
    send(socket , &msg.timestamp , sizeof(time_t) , 0 );
    send(socket , msg.message , strlen(msg.message) , 0 );
}

bool parseMsg(const Message msg, vector<string>& vec_cmd_to_modify) {
    bool add;
    string current_parse="";
    short quote=0;
    vec_cmd_to_modify.clear();
    
    for (size_t i=1; i<msg.size; i++) {
        add=true;

        if (msg.message[i] == ' ') {
            if (!quote) {
                vec_cmd_to_modify.push_back(current_parse);
                current_parse="";
                add = false;
                
            } 
                
        } else if (msg.message[i] == '"') {
            add = false;
            quote = (quote+1)%2;
        }

        if (add) {
            current_parse.push_back(msg.message[i]);
        }      
    }

    if(current_parse.size()>0) {
        vec_cmd_to_modify.push_back(current_parse);//add the last part that has no end
    }

    if (quote) {
        printf("Erreur dans les guillemets !");
        vec_cmd_to_modify.clear();
        return false;
    }

    return true;
    
}

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    printf("%s\n",cmd);
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


