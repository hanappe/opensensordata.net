#include "db.h"

#include <iostream>
#include <sstream>

#include "utils.h"
#include "datastream.h"
#include "config.inc.h"


DB * DB::smDB = 0x0;

DB::DB() : mMysql(0x0) {

    
  //std::cout << "InitConnection" << std::endl;
    
    mMysql = mysql_init(0);
    if (mMysql == NULL) {
      fprintf(stderr, "mysql_init error: %s\n", mysql_error(mMysql));
    }  

    const std::string host = "localhost"; // 111
    
    const std::string usr = "opensensordata";
    const std::string pwd = PWD; // see config.inc.h
    const std::string db = "opensensordata";
    if (!mysql_real_connect(mMysql,
			    host.data(),
			    usr.data(),
			    pwd.data(),
			    db.data(), 0, NULL, 0)) {
      fprintf(stderr, "mysql_real_connect error: %s\n", mysql_error(mMysql));
    }
    
    
}

DB::~DB() {
  if (mMysql) {                                                                                                                                                    
    mysql_close(mMysql);                                                                                                                                           
    //std::cout << "Mysql Closed" << std::endl;                                                                                                                     
  }

}


DB * DB::GetInstance() {
  if (!smDB) {
    smDB = new DB();
    if (!smDB->isConnected()) {
      fprintf(stderr, "Database connection failed.");
      delete smDB;
      smDB = 0;
    }
  }
  
  return smDB;
}

bool DB::loadDatastream(Datastream * datastream, int id) {
        
        bool result =  false;
        
        if (datastream && isConnected()) {
                
                std::ostringstream s;
                s << "SELECT UNIX_TIMESTAMP(`datetime`),`value` FROM `datapoints` WHERE `datastream`=";
                s << id;
                s << " ORDER BY `datetime`;";
                
                const std::string& query = s.str();
                
                if(mysql_real_query(mMysql, query.data(), query.size()) == 0) { // success    
                        
                        result = true;//new Datastream;
                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                        
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                // GET DATAPOINTS
                                if (row[0] && row[1]) { // datetime and value

                                        const time_t time = atoi(row[0]);
                                        const double value = atof(row[1]);
                                        
                                        datastream->appendDatapoints(row[0], time, value);
                                } else {
                                        if (!row[0]) {
                                                fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                                        } else if (!row[1]) {
                                                //fprintf(stderr, "end row A\n");
                                        }
                                }
                                
                        } //end while
                }
        }
        
        return result;
}

bool DB::loadDatastreamContiguous(Datastream * datastream, int id) {
        
        bool result =  false;
        
        if (datastream && isConnected()) {
                
                std::ostringstream s;
                s << "SELECT UNIX_TIMESTAMP(`datetime`),`value` FROM `datapoints` WHERE `datastream`=";
                s << id;
                s << " ORDER BY `datetime`;";
                
                const std::string& query = s.str();
                
                if(mysql_real_query(mMysql, query.data(), query.size()) == 0) { // success    
                        
                        result = true;//new Datastream;
                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                        
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                // GET DATAPOINTS

                                if (row[0] && row[1]) { // datetime and value
                                        
                                        const time_t time = atoi(row[0]);
                                        const double value = atof(row[1]);
                                                                                
                                        datastream->append(row[0], time, value);
                                
                                } else {
                                        if (!row[0]) {
                                                fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                                        } else if (!row[1]) {
                                                datastream->endDatarow();
                                        }
                                }
                                
                        } //end while
                        
                        datastream->endDatarow();
                        
                }
        }
        
        return result;
}

  
bool DB::isConnected() const {
        return !!mMysql;
}

std::string DB::GetPassword() {
        return std::string();
}