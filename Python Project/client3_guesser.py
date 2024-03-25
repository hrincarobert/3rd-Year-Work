from socket import *

client_guesser_socket = socket(AF_INET, SOCK_STREAM)
client_guesser_socket.connect(('localhost', 8000))

if(client_guesser_socket == 0):
	print("Eroare conectare la server!")
	exit(0)
else:
	print("Clientul s-a conectat la server!")

print("Alege dintre Singpleplayer si Multiplayer...")
type_of_game = input("Introdu gamemode-ul dorit : ")
type_of_game_string = str(type_of_game)
client_guesser_socket.send(type_of_game_string.encode('ascii'))

client_running = 1

while client_running:
	
	guess_number = input("Introdu ghicire : ")

	guess_number_string = "Ghicire: " + str(guess_number) 

	client_guesser_socket.send(guess_number_string.encode('ascii'))

	server_command = client_guesser_socket.recv(4069).decode('ascii')

	if(server_command != "Ai ghicit! Mai ai de jucat 0 runde!"):
		print(server_command)

	if (server_command == "Ai ghicit! Mai ai de jucat 0 runde!"):
		server_command = client_guesser_socket.recv(4069).decode('ascii')
		print(server_command)
		client_running = 0

client_guesser_socket.close()