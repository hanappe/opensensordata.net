#ifndef MDBV8_H_
#define MDBV8_H_

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

  Datastream * getDatastream(int id);
 

  bool isConnected() const;
 
  static std::string GetPassword();
  
  MYSQL * mMysql;
  
  static DB * smDB;
};

#endif // MDBV8_H_
