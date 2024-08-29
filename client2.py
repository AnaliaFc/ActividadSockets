# Import the socket module
import socket,os

def userOPass(csocket):
    elegiBien = False

    while not (elegiBien):
        # Se escoge la opcion que se manda al servidor     
        data = csocket.recv(1024).decode()
        print(data)#menu del servidor
        message = input("")
        csocket.send(message.encode())
        data = csocket.recv(1024).decode() #se recibe el flag
        if int(data) == 0:
            print("Opcion incorrecta")
        elif int(data) == 1:
            print("...")
            elegiBien=True
        

def eleccionLong(csocket):

    longValida = False
     
    while  not longValida:
        #Se reciben las condiciones para generar lo pedido
        data = csocket.recv(1024).decode()
        print(data)
        message = input("")#Se mandan los caracteres
        csocket.send(message.encode())
        data = csocket.recv(1024).decode() #se recibe el flag
        if int(data) == 0:
            print("Valor incorrecto")
        elif int(data) == 1:
            print("...\n")
            longValida=True
        
def client():
    host = socket.gethostname()
    port = 8080
    client_socket = socket.socket()
    client_socket.connect((host, port))
    
    userOPass(client_socket) #Definiendo que es lo que quiere hacer el cliente
    eleccionLong(client_socket) #Escogiendo la longitud

    print("Recibiendo la palabra generada...\n")
    data = client_socket.recv(1024).decode()
    print(data)
	
    #print("Presione cualquier tecla para terminar y presione ENTER")
    #input()
    os.system("pause")
    client_socket.close() # Se cierra la conexion

if __name__ == "__main__":
    client()