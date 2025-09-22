//
// Created by alienson on 22.3.24.
//

#ifndef WINTER_DBCONNECTIONPOOL_H
#define WINTER_DBCONNECTIONPOOL_H

#include <queue>
#include <functional>

#include "../sql/DbConnection.h"
#include "../log/Loggy.h"


class DbConnectionPool {
public:
    DbConnectionPool(int initialSize, int maxSize_, DbConnection* (*allocator)()) : currentSize(initialSize), maxSize(maxSize_), connectionAllocator(nullptr){
        for (int i=0; i<initialSize; i++){
            DbConnection* connection = allocator();
            pool.push(connection);
        }
    }

    std::shared_ptr<DbConnection> getConnection(){
        std::scoped_lock lock(mutex_);
        if (pool.empty()){
            //Try to allocate more instances
            if (currentSize >= maxSize) {
                wtLogError("Maximum number of db connections reached!");
                return nullptr;
            }

            DbConnection* connection = connectionAllocator();
            pool.push(connection);
            currentSize++;
        }
        DbConnection* conn = pool.front();
        pool.pop();
        return std::shared_ptr<DbConnection>(conn, [this](DbConnection* conn){
            wtLogTrace("DB Connection released!");
            return returnToPool(conn, pool, mutex_);
        });
    }

    static void returnToPool(DbConnection* connection, std::queue<DbConnection*>& pool, std::mutex& mutex_){
        std::scoped_lock lock(mutex_);
        pool.push(connection);
    }

private:
    int currentSize;
    int maxSize;
    DbConnection* (*connectionAllocator)();
    std::queue<DbConnection*> pool;
    std::mutex mutex_;
};


#endif //WINTER_DBCONNECTIONPOOL_H
