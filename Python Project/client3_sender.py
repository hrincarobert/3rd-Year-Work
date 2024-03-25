from socket import *


client_sender_socket = socket(AF_INET, SOCK_STREAM)
client_sender_socket.connect(('localhost', 8001))

if(client_sender_socket == 0):
	print("Eroare conectare la server!")
	exit(0)
else:
	print("Clientul s-a conectat la server!")

print("Tu esti playerul care da un numar spre ghicire!")

first_number_given = input("Introdu numarul sa fie ghicit: ")
first_number_given_string = str(first_number_given) 
client_sender_socket.send(first_number_given_string.encode('ascii'))

client_running =1

while client_running:

    server_command = client_sender_socket.recv(4069).decode('ascii')

    if(server_command == "Da un numar!"):
        nr= input("New number: ")
        nr_string = str(nr)
        client_sender_socket.send(nr_string.encode('ascii'))
    elif (server_command == "Celalalt player a ghicit! Mai sunt de jucat 0 runde!"):
        server_command = client_sender_socket.recv(4069).decode('ascii')
        print(server_command)
        client_running = 0
    else:
        print(server_command)

client_sender_socket.close()