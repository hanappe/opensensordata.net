#ifndef DBJS_H_
#define DBJS_H_

#include <string>

#include <mysql/mysql.h>

class Datastream;


class DB {
 public:
 
  //private:

  DB();
  DB(const DB& mdb) {}
  ~DB();

  static DB * GetInstance();

  bool loadDatastream(Datastream * datastream, int id);
  bool loadDatastreamContiguous(Datastream * datastream, int id);

  bool isConnected() const;
 
  static std::string GetPassword();
  
  MYSQL * mMysql;
  
  static DB * smDB;
};

#endif // DBJS_H_
