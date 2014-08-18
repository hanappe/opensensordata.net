#ifndef DBJS_H_
#define DBJS_H_

#include <string>

#include <mysql/mysql.h>

class Account;
class Group;
class Datastream;
class Photostream;

class DB {
 public:
 
  //private:

  DB();
  DB(const DB& mdb) {}
  ~DB();

  static DB * GetInstance();

  bool loadAccount(Account * account, int id);
  //bool loadAccount(Account * account, char * ch);
  bool loadGroup(Group * group, int id);

  bool loadDatastream(Datastream * datastream, int id);
  bool loadDatastreamContiguous(Datastream * datastream, int id);

  bool loadPhotostream(Photostream * photostream, int id);

  bool isConnected() const;
  
  MYSQL * mMysql;
  
  static DB * smDB;
};

#endif // DBJS_H_
