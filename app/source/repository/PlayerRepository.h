//
// Created by alienson on 22.3.24.
//

#ifndef WINTER_PLAYERREPOSITORY_H
#define WINTER_PLAYERREPOSITORY_H

#include <vector>
#include "sql/Statement.h"
#include "sql/Repository.h"
#include "../entity/PlayerEntity.h"
#include "util/util.h"

class PlayerRepository : public Repository {
public:
    std::shared_ptr<std::vector<std::shared_ptr<PlayerEntity>>> getAllPlayers(){
        std::shared_ptr<Statement> statement = Repository::createStatement();

        auto players = statement->executeQuery("select * from player")->getResultList(PlayerEntity::getInstance);

        return castTo<PlayerEntity>(players);
    }

    std::shared_ptr<PlayerEntity> getSinglePlayer(){
        std::shared_ptr<Reflect> player = createStatement()->executeQuery("select * from player limit 1")->getResult(PlayerEntity::getInstance);
        return std::dynamic_pointer_cast<PlayerEntity>(player);
    }

    int getPlayerCount(){
        return createStatement()->executeQuery("select count(*) from player")->getInt(0);
    }

    void createPlayer(PlayerEntity p){
        //createStatement()->executeUpdate("insert into player values ") TODO
    }

    std::shared_ptr<PlayerEntity> getPlayerById(long id){
        std::shared_ptr<Statement> statement = createStatement("select * from player where id = :id");
        statement->setLong(id, "id");
        return std::dynamic_pointer_cast<PlayerEntity> (statement->execute()->getResult(PlayerEntity::getInstance));
    }
};


#endif //WINTER_PLAYERREPOSITORY_H
