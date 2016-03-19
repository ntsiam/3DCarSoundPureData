#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int conn1 = 0;
int conn2 = 0;
int conn3 = 0;

struct Signals {
	double md_ax1;
	double md_ay1;
	double md_bx1;
	double md_by1;
};

// Die folgenden Definitionen sind zur Einbindung der SPU in das SILAB-System nötig.
// ------------------------------------------------------------------
// sSPUSendBorderSignals (Implementation)
// ------------------------------------------------------------------

// ------------------------------------------------------------------
// cSPUSendBorderSignals (Implementation)
// ------------------------------------------------------------------

bool connectPD() {
	struct sockaddr_in serv_addr; 
	if((conn1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return false;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
	//conn1
    serv_addr.sin_port = htons(50070); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return false;
    } 

    if( connect(conn1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return false;
    } 

//    //conn2
//    serv_addr.sin_port = htons(50002); 
//
//    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//    {
//        printf("\n inet_pton error occured\n");
//        return false;
//    } 
//
//    if( connect(conn2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//    {
//       printf("\n Error : Connect Failed \n");
//       return false;
//    }
//
//	//conn3
//	serv_addr.sin_port = htons(50003); 
//
//    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//    {
//        printf("\n inet_pton error occured\n");
//        return false;
//    } 
//
//    if( connect(conn3, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//    {
//       printf("\n Error : Connect Failed \n");
//       return false;
//    }
	return true;
}

double dist=-10;

void send(double value, int conn) {
	char sendBuff[1025];
	sprintf(sendBuff,"%d;",(int)value);
	write(conn, sendBuff, strlen(sendBuff)); 

}

void sendline(char* sendBuff, int conn) {
	write(conn, sendBuff, strlen(sendBuff)); 
}

bool Prepare()
{
	return connectPD();
}

void Trigger()
{
	//m_Data.md_Out = m_Data.md_In;
	char msg[100];
	dist+=0.001;
	//if (dist > -0.1 && dist < 0.1)
	//	usleep(200000);
	sprintf(msg,"%lf 1 0 0;",dist);
	printf("Sending: %s\n",msg);
	sendline(msg,conn1);
}

int main() {
	bool res;
	int i=0;
	res = Prepare();
	printf("Sending: x_car y_car x_ego y_ego\n");
	for (i=0;i<20000;i++) {
		Trigger();
		usleep(500);
	}
	return 0;
}
