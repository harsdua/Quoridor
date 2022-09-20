#Quoridor 
An implementation of a 2-4 player board game called quoridor in C++ using Qt5, Qt5 designer, linux sockets and SQLite. This project is the result of a collaberation between 6 fellow students. The game is in french and the instructions within the client are fairly straightforward. The instructions below will detail for non-francophone users how to play a game in the normal game mode. The rules can be found here https://en.gigamic.com/files/media/fiche_pedagogique/educative-sheet_quoridor-english.pdf

The game is multiplayer, over LAN, given the users know each other's ip addresses. Otherwise, the game can also be played on one computer with multiple users. Simply launch multiple terminals for each client. 

The game features 3 game modes, messaging functionalities, saving and loading options, and a leaderboard.


If you do not have Qt5, simply launch the program, without compilation, as the object files are included.


Write "make" to recompile in the terminal targetting the source folder


To play, you must first launch a server by using the following command 
"./serveur interface"

For each client that wants to play, type "./client_interface" in a new terminal.

Register an account on the left

Type the username in the "pseudo" box

type the password in the following two boxes

Click "creer un compte"



Log in with the username/pass on the right side if the account has been created.

Go to the Amis tab in the top right
Add a friend by the username via the bottom left box,

In the demandes section, friend invitations will appear. Once accepted, your friends can join your game lobby


Launch a lobby in the "menu principal" and write a name for the lobby in the right text box, and click demarrer le lobby
Your friends can now join. To do so, they must go to the "amis" section in the top right, and click your name under "liste des amis", and click "rejoindre le lobby"


Once your friends are in, click demarrer la partie. Your pawn is the green one. Click on a square or a wall. click valider le deplacement if a square was chosen or valider le mur if a wall was chosen. 
