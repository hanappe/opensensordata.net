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

  DB();
  DB(const DB& mdb) {}
  ~DB();

  static DB* GetInstance();

  bool loadAccount(Account* account, int id);
  //bool loadAccount(Account* account, char * ch);
  bool loadGroup(Group* group, int id); // DEPRECATED FUNCTION
  bool loadGroupFromRange(Group* group, int id, time_t start, time_t end);

  bool loadDatastream(Datastream* datastream, int id); // DEPRECATED FUNCTION
  
  // Load datapoints from datastream in range [ start - end ] given in unix_timestamp
  // Set 'start' and 'end' to 0 to get all datapoints
  
  bool loadDatastreamFromRange(Datastream* datastream, int id, time_t start, time_t end);
  bool loadDatastreamContiguous(Datastream* datastream, int id); // DEPRECATED FUNCTION

  bool loadPhotostream(Photostream* photostream, int id); // DEPRECATED FUNCTION

  // Load Photos from photostream in range [ start - end ] given in unix_timestamp
  // Set 'start' and 'end' to 0 to get all photos
  // This function has not been enough tested
  
  bool loadPhotostreamFromRange(Photostream* photostream, int id, time_t start, time_t end);

  bool isConnected() const;
  
  MYSQL* mMysql;
  
  static DB* smDB;

};

#endif // DBJS_H_
