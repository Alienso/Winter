//
// Created by alienson on 22.3.24..
//

#ifndef WINTER_PLAYERREPOSITORY_H
#define WINTER_PLAYERREPOSITORY_H

#include <vector>
#include <../sql/Statement.h>
#include "../../sql/Repository.h"
#include "../entity/PlayerEntity.h"

#include "util.h"

using namespace std;

class PlayerRepository : public Repository {
public:
    vector<PlayerEntity*>* getAllPlayers(){
        shared_ptr<Statement> statement = Repository::createStatement();
        vector<PlayerEntity*>* players = wt::vector_ptr_cast<Reflect, PlayerEntity>(
                statement->executeQuery("select * from player")->getResultList());
        return players;
        //players = statement->executeQuery("select * from player");
        //getResultList //getResultListAsInt //getSingleResultAsInt ...
    }
};


#endif //WINTER_PLAYERREPOSITORY_H
