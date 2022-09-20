#ifndef OBSERVER_SUBJECT_H
#define OBSERVER_SUBJECT_H

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Observer{
    virtual void subjectChat(string message)=0;
    virtual void subjectNotifFR(string name)=0;
    virtual void subjectAccept(string name)=0;
    virtual void subjectDecline(string name)=0;
    virtual void subjectAllChat(string name)=0;
    virtual void subjectMove(string board)=0;
    virtual void subjectDeleteFriend(string name)=0;

    virtual void subjectDisplayBoard(string board, string wall)=0;
    virtual void subjectTurnPlayer(int playerId)=0;
    virtual void subjectGameLaunch()=0;

};

class Subject{
  vector <Observer *> observers;
public:
  void registerObserver(Observer *observer){
    observers.push_back(observer);
  }
  void removeObserver(Observer *observer){
    remove(begin(observers),end(observers),observer);
  }
  void notifyObserversChat(string message) const {
    for (auto &observer:observers)
      observer->subjectChat(message);
  }
  void notifyObserverFR(string name){
      for (auto &observer:observers)
        observer->subjectNotifFR(name);
  }
  void notifyAcceptedFR(string name){
      for (auto &observer:observers)
        observer->subjectAccept(name);
  }
  void notifyDeclinedFR(string name){
      for (auto &observer:observers)
        observer->subjectDecline(name);
  }
  void notifyObserversAllChat(string message) const {
      for (auto &observer:observers)
        observer->subjectAllChat(message);
    }

  void notifyObserversMove(string message) const {
      for (auto &observer:observers)
        observer->subjectMove(message);
    }
  void notifyObserversDeleteFriend(string name) const {
      for (auto &observer:observers)
        observer->subjectDeleteFriend(name);
    }

    // Partie Lobby

  void notifyObserversDisplayBoard(string board, string wall) const {
      for (auto &observer:observers)
        observer->subjectDisplayBoard(board, wall);
  }

  void notifyObserversTurnPlayer(int playerId)const {
      for (auto &observer:observers)
        observer->subjectTurnPlayer(playerId);
  }

  void notifyObserversGameLaunch()const {
      for (auto &observer:observers)
        observer->subjectGameLaunch();
  }
};


#endif // OBSERVER_SUBJECT_H
