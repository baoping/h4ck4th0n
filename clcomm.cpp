#include "clcomm.h"
#include "hack.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <vector>
#include <cstring>
using namespace std;

#define MYPORT "3490"
#define BACKLOG 10

vector<ClientCommunicator> clients;

void listen_for_clients(World *world) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, MYPORT, &hints, &res);


	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(sockfd, res->ai_addr, res->ai_addrlen);
	listen(sockfd, BACKLOG);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	int next_id = 0;

	while(true) {
		sockaddr_storage their_addr;
		socklen_t addr_size = sizeof their_addr;
		int new_fd = accept(sockfd, (sockaddr *)&their_addr, &addr_size);
		if(new_fd != -1) {
			ClientCommunicator clcomm;
			clcomm.sock = Socket(new_fd);
			clcomm.object_id = next_id;

			Object o;
			o.p = Vector2D(0.0f, 0.0f);
			o.v = Vector2D(0.0f, 0.0f);
			o.mass = 1.0f;
			o.rad = 1.0f;
			o.color = Color(0.0f, 1.0f, 0.0f);
			o.h = 1.0f;
			o.id = next_id;

			world->objects.insert(pair<int, Object>(next_id, o)); 
			clients.push_back(clcomm);

			next_id++;
		}

		for(vector<ClientCommunicator>::iterator it = clients.begin(); it != clients.end(); ++it) {
			world->sendObjects(it->sock);
				while(it->sock.hasRemaining()) {
				char keypress[2];
				it->sock.receive(keypress, 2);
				it->key_pressed[keypress[1]] = (bool)keypress[0];
			}
		}

	}
}
