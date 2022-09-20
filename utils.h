////////////
//
//  auteur: Harsh Dua
//  
//  date: 03-03-2022
//
//  description : Différents utilitaires pour la réception, l'envoi et la gestion de données sur un socket (client/server) 
//
//  cours: info-f-209 groupe 7
//
////////////

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <unistd.h>
#include <time.h>

#include <vector>
#include <string>
#include <iostream>

#include <random>
#include <sstream>

using namespace std;

/**
 * @brief Message contenant sa taille, le temps interne auquel il est envoyé/reçu, le texte
 * 
 */
struct Message{
    size_t size;
    time_t timestamp;
    char* message;
};




/**
 * @brief écoute sur un port et regarde si il y a de nouvelles données
 * 
 * @param socket channel où envoyer le message MSG 
 * @param msg Message content le temps, du texte et la longueur du texte
 * @return true si c'est un message lambda
 * @return false si il y a une déconnexion
 */
bool readMessage(int socket, Message& msg);

/**
 * @brief envoie un message sur un port
 * 
 * @param socket channel où envoyer le message MSG 
 * @param msg Message content le temps, du texte et la longueur du texte
 */
void sendMessage(int socket, Message& msg);

/**
 * @brief Divise le message MSG reçu et met ses différentes composantes dans VEC_CMD_TO_MODIFY 
 * 
 * @param msg message contenant la taille, le txt et la taille du txt 
 * @param vec_cmd_to_modify vecteur où mettre les différentes parties du message
 */
bool parseMsg(const Message msg, vector<string>& vec_cmd_to_modify);

/**
   * @brief utilisé pour ouvrir un nouveau terminal qui éxécute une commande : gregpaton08's response
   * https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
   * 
   */
string exec(const char* cmd);

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    static std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
}

#endif //SERVER_UTILS_H
