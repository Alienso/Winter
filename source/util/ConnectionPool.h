//
// Created by alienson on 22.3.24..
//

#ifndef WINTER_CONNECTIONPOOL_H
#define WINTER_CONNECTIONPOOL_H

#include <queue>
#include <functional>

#include "../sql/Connection.h"
#include "Logger.h"

using namespace std;
using namespace wt::sql;


class ConnectionPool {
public:
    ConnectionPool(int initialSize, int maxSize_, Connection* (*allocator)()) : currentSize(initialSize), maxSize(maxSize_), connectionAllocator(nullptr){
        for (int i=0; i<initialSize; i++){
            Connection* connection = allocator();
            pool.push(connection);
        }
    }

    shared_ptr<Connection> getConnection(){
        scoped_lock lock(mutex_);
        if (pool.empty()){
            //Try allocate more instances
            if (currentSize >= maxSize) {
                wtLogError("Maximum number of db connections reached!");
                return nullptr;
            }

            Connection* connection = connectionAllocator();
            pool.push(connection);
            currentSize++;
        }
        Connection* conn = pool.front();
        pool.pop();
        return shared_ptr<Connection>(conn, [this](Connection* conn){
            wtLogTrace("DB Connection released!");
            return returnToPool(conn, pool, mutex_);
        });
    }

    static void returnToPool(Connection* connection, queue<Connection*>& pool, mutex& mutex_){
        scoped_lock lock(mutex_);
        pool.push(connection);
    }

private:
    int currentSize;
    int maxSize;
    Connection* (*connectionAllocator)();
    queue<Connection*> pool;
    mutex mutex_;
};


#endif //WINTER_CONNECTIONPOOL_H
