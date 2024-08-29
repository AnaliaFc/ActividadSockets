/******CLIENTE******/

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

int main(int argc, char **argv){
	WSADATA wsa;
	SOCKET skt;
	int puerto = 8080, recv_size;
	char msjLargo[1000];//mensajes generales
	char cadenaGenerada[52]; //Cadena resultado
	struct sockaddr_in server;
	char integer[4];
    int flag = 0;

	printf("Inicializando Winsock...");
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		printf("Error al inicializar winsock\n");
		exit(-1);
	}
	printf("Winsock inicializado\n");

	printf("Creando socket...");
	if((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		printf("Error creando socket\n");
		exit(-1);
	}
	printf("Socket creado\n");

	server.sin_family = AF_INET; // Familia TCP/IP
	server.sin_port = htons(puerto); // Puerto de la aplicacion del servidor
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Direccion IP del servidor

	if(connect(skt, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("Error de conexion\n");
		exit(-1);
	}
	system("cls");
	printf("Conexion exitosa\n");

	//Escogiendo Primera opcion
	do{
        if((recv_size = recv(skt, msjLargo, 1000, 0)) == SOCKET_ERROR)
            {printf("Recepcion fallida\n");}
        msjLargo[recv_size] = '\0';
        printf("%s \n", msjLargo); gets(integer);

        if(send(skt, integer, strlen(integer), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);}

		if((recv_size = recv(skt, integer, 4, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");
        integer[recv_size] = '\0';
		flag=atoi(integer);
		if(flag==0)
            {printf("Opcion incorrecta\n");}
        else if(flag==1)
            {printf("...\n");}
	}while(flag==0);

    flag=0;

	//Escogiendo longitud
	do{
        if((recv_size = recv(skt, msjLargo, 1000, 0)) == SOCKET_ERROR)
          {printf("Recepcion fallida\n");}
        msjLargo[recv_size] = '\0';
        printf("%s \n", msjLargo); gets(integer);

        if(send(skt, integer, strlen(integer), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }
        if((recv_size = recv(skt, integer, 4, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");
        integer[recv_size] = '\0';
		flag=atoi(integer);
		if(flag==0)
            {printf("Valor incorrecto\n");}
        else if(flag==1)
            {printf("...\n");}
	 }while(flag==0);

	printf("Recibiendo la palabra generada...\n");
	if((recv_size = recv(skt, cadenaGenerada, 60, 0)) == SOCKET_ERROR)
		printf("Recepcion fallida\n");

	cadenaGenerada[recv_size] = '\0';
	printf("%s \n", cadenaGenerada);

	system("pause");
	closesocket(skt);
	WSACleanup();

	return 0;
}


