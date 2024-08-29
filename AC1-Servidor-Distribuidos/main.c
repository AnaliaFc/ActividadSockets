/*SERVIDOR*/
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void imprimirMSJ(int opcion); //Imprime lo que va sucediendo mientras el cliente elige lo que quiere generar
void msjLongCaracteres(bool opc, char msj[]);//Mensaje para el usuario, para informarle la cantidad de caracteres requeridos
void generarCadena (int longitud, char msj[],bool us1pa2); //funcion que genera la cadena
bool longitudCorrecta(int num, bool b); //evalua si la longitud mandada por el cliente es la correcta

int main(int argc, char **argv){
	WSADATA wsa;
	SOCKET skt, skt2;
	int longitud_cliente, puerto = 8080, recv_size;
	struct sockaddr_in server, cliente;
	char msjLargo[1000];//mensajes generales
	char cadenaGenerada[60];
	char integer[4];
	int temp=0;
	bool longCorrecta;
	bool tUser_fPass; //booleano para indicar lo que el cliente quiere generar
                      // true=Usuario, false=Password

    //Primero se crea el socket
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
	server.sin_port = htons(puerto); // Puerto
	server.sin_addr.s_addr = INADDR_ANY; // Cualquier cliente puede conectarse

	if(bind(skt, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
		printf("Error al realizar el bind\n");
		exit(-1);
	}
	printf("Bind realizado\n");
	listen(skt, 5);
	printf("Esperando conexiones entrantes...\n");

	longitud_cliente = sizeof(struct sockaddr_in);
	skt2 = accept(skt, (struct sockaddr*)&cliente, &longitud_cliente);
	if(skt2 == INVALID_SOCKET){
		printf("Fallo al aceptar la conexion\n");
		exit(-1);
	}
	system("cls");
	printf("Cliente %s conectado exitosamente\n", inet_ntoa(cliente.sin_addr));

	/***************Logica de la Actividad********************/
	do{
        strcpy(msjLargo,"Ingrese el numero de opcion que desea generar: \n 1-Generar Usuario.\n 2-Generar Password.");
        if(send(skt2, msjLargo, strlen(msjLargo), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }

        printf("Esperando que realice eleccion...\n");
        if((recv_size = recv(skt2, integer, 4, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");
        integer[recv_size] = '\0';
        temp = atoi(integer);
        imprimirMSJ(temp);

        if ((temp==1)||(temp==2))
            {strcpy(integer,"1");}//Puede continuar eligiendo la longitud
        else
            {strcpy(integer,"0");}//tiene que escoger una opcion correcta
        if(send(skt2, integer, strlen(integer), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }
	}while(!((temp==1)||(temp==2)));

    if(temp==1)
        tUser_fPass=true;
    if(temp==2)
        tUser_fPass=false;

	//Muestra por pantalla la opcion elegida
	do{
	    msjLongCaracteres(tUser_fPass,msjLargo);
        if(send(skt2, msjLargo, strlen(msjLargo), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }
        printf("Esperando que ingrese la longitud deseada:\n");
        if((recv_size = recv(skt2, integer, 4, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");
        integer[recv_size] = '\0';
        temp = atoi(integer);

        longCorrecta=longitudCorrecta(temp,tUser_fPass);

        if (longCorrecta)
            {strcpy(integer,"1");}//Se procede a generar la cadena
        else
            {strcpy(integer,"0");}//tiene que volver a ingresar un numero dentro del rango pedido
        if(send(skt2, integer, strlen(integer), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }
      }while(!longCorrecta);

	generarCadena(temp,cadenaGenerada,tUser_fPass);

	printf("Generacion exitosa!!!\n");
	printf("RESULTADO es: %s  \n",cadenaGenerada);

	printf("Enviando el resultado al usuario. \n");
	if(send(skt2, cadenaGenerada, strlen(cadenaGenerada), 0) < 0){
		printf("Error al enviar mensaje\n");
		exit(-1);
	}
	printf("Respuesta enviada exitosamente\n");

	system("pause");

	closesocket(skt);
	WSACleanup();

	return 0;
}


void imprimirMSJ(int opcion){
    switch (opcion)
    {
        case 1:
            printf("El cliente eligio generar nombre de usuario. \n");
            break;
        case 2:
            printf("El cliente eligio generar password. \n");
            break;
        default:
            printf("El cliente ingreso un numero de opcion incorrecta (%d). Solicitando devuelta el numero \n",opcion);
    }
}

void generarCadena (int longitud, char msj[],bool us1pa2)
{
    char *cadena=malloc(longitud*sizeof(char));
    char vocales[]="aeiouAEIOU";
    char consonantes[]="bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
    char charscontrasenias[]="aeiouAEIOUbcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ1234567890!#$%&()*+,-./:;<=>?@[\]^_`{|}~";//93 elementos
    srand(getpid());
    bool vocal= 1 & (rand()%2);

    if(us1pa2)//USUARIO
        {for (int i=0; i<longitud; i++)
            {
                if(vocal)
                {  if (i==0)//Se chequea si es primera iteracion para mayuscula
                    cadena[i]=vocales[(rand()%4)+5];
                   else
                    cadena[i]=vocales[rand()%4];
                 }
                 else{
                    if(i==0)//Se chequea si es primera iteracion para mayuscula
                       cadena[i]=consonantes[(rand()%20)+21];
                    else
                       cadena[i]=consonantes[rand()%20];
                     }
                vocal=!vocal;
            }
        }
    else//CONTRASEÑA
    {for (int i=0 ; i<longitud ; i++)
         cadena[i]=charscontrasenias[rand()%93];
    }
    cadena[longitud]='\0';
    strcpy(msj,cadena);
    free(cadena);
}

bool longitudCorrecta(int num, bool b)
{
    bool retorno;
    if(b)
    {
        if(num>=5 && num<=15)
            {retorno=true;}
        else
            {retorno=false;}
    }else if(!b)
        {
            if(num>=8 && num<=50)
            {retorno=true;}
            else
            {retorno=false;}
        }

    if(!retorno)
        printf("Longitud ingresada incorrecta (%d). Esperando que ingrese un numero valido. \n",num);
    else
        printf("Longitud correcta! Cantidad de caracteres escogidos: %d.\n",num);
    return retorno;
}

void msjLongCaracteres(bool opc, char msj[])
{
    if(opc)
        strcpy(msj,"Usted eligio generar usuario se le pide un minimo de 5 y un maximo de 15 caracteres.\nIngrese la longitud deseada por favor...");
    if(!opc)
        strcpy(msj,"Usted eligio generar password se le pide un minimo de 8 y un maximo de 50 caracteres.\nIngrese la longitud deseada por favor...");
}


