#include "server.h"
#include "lobby.h"
#include "masterserver.h"
#include <unistd.h>


int main (int argc, const char* argv[]) {
    int port=8080;
    if ((argc >=3) && (atoi(argv[2])))
        port=atoi(argv[2]);
    if (!strcmp(argv[1], "terminal")) {
        printf("serveur\n");
        MasterServer server(port, false);
    }
    	else if(!strcmp(argv[1], "interface")){
    		printf("serveur\n");
        	MasterServer server;
    	}
	else if (!strcmp(argv[1], "lobby")) {
	printf("lobby\n");
	if (port==8080){
	    printf("Error: Cannot launch lobby on the port 8080 as it is occupied by the masterServer");
	    printf("Returning in 5 seconds");
	    sleep(5);
	    return 1;
	}

	Lobby Lobby(port, false);



    }
}

