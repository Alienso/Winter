//
// Created by Alienson on 10.2.2024..
//

//#include "nanodbc/nanodbc.h"

//#include <iostream>

/*int main(int argc, char **argv){
    nanodbc::connection connection("postgresql://postgres:abcdef@localhost:5432/postgres");
    nanodbc::result row = nanodbc::execute(connection, "select now()");
    std::cout << row.get<std::string>(0) << '\n';
    return 0;
}*/

#include <iostream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#define OTL_ODBC_ALTERNATE_RPC
#if !defined(_WIN32) && !defined(_WIN64)
#define OTL_ODBC
#else
#define OTL_ODBC_POSTGRESQL // required with PG ODBC on Windows
#endif
#include <otlv4.h> // include the OTL 4.0 header file

using namespace std;

otl_connect db; // connect object

void insert()
// insert rows into table
{
    otl_stream o(50, // PostgreSQL 8.1 and higher, the buffer can be > 1
                 "insert into test_tab values(:f1<int>,:f2<char[31]>)",
            // SQL statement
                 db // connect object
    );
    char tmp[32];

    for(int i=1;i<=100;++i){
        sprintf(tmp,"Name%d",i);
        o<<i<<tmp;
    }
}

void update(const int af1)
// insert rows into table
{
    otl_stream o(1, // PostgreSQL 8.1 and higher, buffer size can be >1
                 "UPDATE test_tab "
                 "   SET f2=:f2<char[31]> "
                 " WHERE f1=:f1<int>",
            // UPDATE statement
                 db // connect object
    );

    o<<"Name changed"<<af1;
    o<<otl_null()<<af1+1; // set f2 to NULL

}

void select(const int af1)
{
    otl_stream i(50, // On SELECT, the buffer size can be > 1
                 "select * from test_tab where f1>=:f11<int> and f1<=:f12<int>*2",
            // SELECT statement
                 db // connect object
    );
    // create select stream

    int f1;
    char f2[31];

#if (defined(_MSC_VER) && _MSC_VER>=1600) || defined(OTL_CPP_11_ON)
// C++11 (or higher) compiler
#if defined(OTL_ANSI_CPP_11_VARIADIC_TEMPLATES)
    otl_write_row(i,af1,af1); // Writing input values into the stream
#else
    // when variadic template functions are not supported by the C++
  // compiler, OTL provides nonvariadic versions of the same template
  // functions in the range of [1..15] parameters
 otl_write_row(i,af1,af1); // Writing input values into the stream
  // the old way (operators >>() / <<()) is available as always:
  // i<<af1<<af1; // Writing input values into the stream
#endif
    for(auto& it : i){
        it>>f1;
        cout<<"f1="<<f1<<", f2=";
        it>>f2;
        if(it.is_null())
            cout<<"NULL";
        else
            cout<<f2;
        cout<<endl;
    }
#else
    // C++98/03 compiler
 i<<af1<<af1; // Writing input values into the stream
 while(!i.eof()){ // while not end-of-data
   i>>f1;
   cout<<"f1="<<f1<<", f2=";
   i>>f2;
   if(i.is_null())
     cout<<"NULL";
   else
     cout<<f2;
   cout<<endl;
 }
#endif

}

int main()
{
    otl_connect::otl_initialize(); // initialize ODBC environment
    try{

        db.rlogon("postgresql://postgres:abcdef@localhost:5432/postgres");

        otl_cursor::direct_exec
                (
                        db,
                        "drop table test_tab",
                        otl_exception::disabled // disable OTL exceptions
                ); // drop table

        db.commit();

        otl_cursor::direct_exec
                (
                        db,
                        "create table test_tab(f1 int, f2 varchar(30))"
                );  // create table

        db.commit();

        insert(); // insert records into the table
        update(10); // update records in the table
        select(8); // select records from the table

    }

    catch(otl_exception& p){ // intercept OTL exceptions
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.sqlstate<<endl; // print out SQLSTATE message
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from ODBC

    return 0;

}