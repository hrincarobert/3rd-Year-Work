from socket import *
import random


run_server = True
stop_server = 0

def multiplayer_game(client_guesser_socket):

    global stop_server
    sender_client_socket = socket(AF_INET, SOCK_STREAM)
    sender_client_socket.bind(('localhost',8001))
    sender_client_socket.listen(3)
    (sender_client, sender_client_address) = sender_client_socket.accept()

    if(sender_client_socket == 0):
        print("Eroare la conectare la sender client!")
        sender_client_socket.close()
        stop_server = 1
    else:
        print("Serverul s-a conectat la sender client!")


    number_to_be_guessed = int(sender_client.recv(4069).decode('ascii'))
    print(f"Numarul primit de sender : {number_to_be_guessed}")

    rounds = 2
    run_per_round = 1
    number_of_guesses = 0
    highest_score = []

    while rounds:
        rounds = rounds -1
        while run_per_round:
            client_guess = client_guesser_socket.recv(4069)
            client_guess_char = client_guess.decode('ascii')
            print(client_guess_char)

            split_values = client_guess_char.split()
            client_guess_string = split_values[1]
            client_guess = int(client_guess_string)

            number_of_guesses += 1
            run_per_round = 1

            if (client_guess == number_to_be_guessed) :
                message_for_client = (f"Ai ghicit! Mai ai de jucat {rounds} runde!")
                message_for_client_sender = (f"Celalalt player a ghicit! Mai sunt de jucat {rounds} runde!")
                client_guesser_socket.send(message_for_client.encode('ascii'))
                sender_client.send(message_for_client_sender.encode('ascii'))
                run_per_round = 0
            else:
                if client_guess < number_to_be_guessed :
                    message_for_client = ("Numarul este mai mare!")
                    message_for_client_sender = ("Celalalt player trebuie sa dea un numar mai mare!")
                else:
                    message_for_client = ("Numarul este mai mic!")
                    message_for_client_sender = ("Celalalt player trebuie sa dea un numar mai mic!")
                client_guesser_socket.send(message_for_client.encode('ascii'))
                sender_client.send(message_for_client_sender.encode('ascii'))
        highest_score.append(number_of_guesses)
        number_of_guesses = 0
        run_per_round = 1
        if(rounds != 0):
            req_number = ("Da un numar!")
            sender_client.send(req_number.encode('ascii'))
            number_to_be_guessed = int(sender_client.recv(4069).decode('ascii'))
        print(number_to_be_guessed)
        
    score_final = min(highest_score)
    message_for_client = (f"Jocul s-a terminat! Scorul maxim este de {score_final}!")
    client_guesser_socket.send(message_for_client.encode('ascii'))
    sender_client.send(message_for_client.encode('ascii'))
    client_guesser_socket.close() 
    sender_client_socket.close()
    stop_server = 1
    print("Jocul s-a terminat, serverul se va inchide!")
    

def singleplayer_game(client_guesser_socket):

    rounds = 2
    run_per_round = 1
    number_of_guesses = 0
    number_to_be_guessed = random.randint(0, 50)
    print(number_to_be_guessed)
    highest_score = []
    while rounds:
        rounds = rounds -1
        while run_per_round:

            client_guess = client_guesser_socket.recv(4069)
            client_guess_char = client_guess.decode('ascii')
            print(client_guess_char)

            split_values = client_guess_char.split()
            client_guess_string = split_values[1]
            client_guess = int(client_guess_string)

            number_of_guesses += 1
            run_per_round = 1

            if (client_guess == number_to_be_guessed) :
                message_for_client = (f"Ai ghicit! Mai ai de jucat {rounds} runde!")
                client_guesser_socket.send(message_for_client.encode('ascii'))
                run_per_round = 0
            else:
                if client_guess < number_to_be_guessed :
                    message_for_client = ("Numarul este mai mare!")
                else:
                    message_for_client = ("Numarul este mai mic!")
                client_guesser_socket.send(message_for_client.encode('ascii'))
        highest_score.append(number_of_guesses)
        number_of_guesses = 0
        run_per_round = 1
        number_to_be_guessed = random.randint(0, 50)
        print(number_to_be_guessed)
    
    score_final = min(highest_score)
    message_for_client = (f"Jocul s-a terminat! Scorul maxim este de {score_final}!")
    client_guesser_socket.send(message_for_client.encode('ascii'))
    client_guesser_socket.close() 
    global stop_server
    stop_server = 1
    print("Jocul s-a terminat, serverul se va inchide!")
            

while run_server:
    
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('localhost', 8000))
    server_socket.listen(3)

    (client_guesser_socket, client_guesser_address) = server_socket.accept()

    if(client_guesser_socket == 0):
        print("Eroare la conectare la guesser client!")
        server_socket.close()
        break
    else:
        print("Serverul s-a conectat la gusser client!")

    gamemode_type = client_guesser_socket.recv(4069).decode('ascii')
    print(f"Jucatorul a ales sa joace {gamemode_type}!")
    if gamemode_type == "Singleplayer":
        singleplayer_game(client_guesser_socket)
    elif gamemode_type == "Multiplayer":
        multiplayer_game(client_guesser_socket)
    else:
        print(f"Gamemode invalid: {gamemode_type}. Serverul se va inchide!")
        client_guesser_socket.close()
        server_socket.close()
        break
       
    if(stop_server == 1):
        server_socket.close()
        break
