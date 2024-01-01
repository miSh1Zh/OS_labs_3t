#include <iostream>
#include "top_node.h"
#include <sys/time.h>
#include "ze_mes_queue.h"
#include <string>
#include "tree.h"
#include <sstream>

#include <map>

int main(int argc, char *argv[]){
    if (argc < 4){
        printf("Usage: ./main childID parentPort parentID\n");
        exit(-1);
    }
    TNode node(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    while(true){
        std::string message;
        std::string command;
        message = receive_message(node.parent);
        std::istringstream request(message);
        request >> command;
        if(command == "pid"){
            std::string answer = std::to_string(getpid());
            send_message(node.parent, answer);
        }
        else if(command == "ping"){
            int child;
            request >> child;
            std::string answer = node.Ping(child);
            send_message(node.parent, answer);
        }
        else if(command == "create"){
            int child;
            request >> child;
            std::string answer = node.createChild(child);
            send_message(node.parent, answer);
        }
        else if(command == "send"){
            int child;
            std::string str;
            request >> child;
            getline(request, str);
            std::string answer = node.sendStr(str, child);
            send_message(node.parent, answer);
        }
        else if(command == "exec"){
            std::string str;
            getline(request, str);
            std::string answer = node.exec(str);
            send_message(node.parent, answer);
        }
        else if(command == "remove"){
            std::string answer = node.removeElem();
            send_message(node.parent, answer);
            disconnect(node.parent, node.parentPort);
            node.parent.close();
            break;
        }else if(command == "clear"){
            int child;
            request >> child;
            std::string answer = node.clearChild(child);
            send_message(node.parent, answer);
        }
    }
    sleep(1);
    return 0;
}