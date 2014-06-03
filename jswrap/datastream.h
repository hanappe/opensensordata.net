#ifndef DATASTREAMV8_H
#define DATASTREAMV8_H

#include <v8.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include <sstream>
#include <iostream>
#include <vector>

#include "utils.h"
//#include "mdb.h"

using namespace v8;


struct Pair {
  Pair() {
  }
  
  Pair(time_t t, double v) : time(t), value(v) {
  
  }

  Pair(const Pair& p) : time(p.time), value(p.value) {
  
  }

  time_t time;
  double value;
};


class Datastream {
public:

        Datastream(int id = 0) : mId(id), current(0x0) {

	}

	int mId;
	
	~Datastream() {
	  clear();
	}

	void clear() {
	  for (std::vector<Datarow*>::iterator it = datarows.begin(); it != datarows.end(); ++it) {
	    delete (*it);
	  };
	  datarows.clear();
	  current = 0x0;
	}
	
	void beginDatarow() {
	  current = new Datarow;
	}

	void append(int date, double value) {
	  
	  if (!current) {
	    beginDatarow();
	  }
	  Pair p(date, value);
	  //std::cout << "Pair date: " << p.date << " v: " << p.value << current << std::endl;
	  current->push_back(p);
	}

	void endDatarow() {
	  if (current) {
	    datarows.push_back(current);
	    current = 0;
	  }
	}

	void print() {
	  /*
	  std::cout << "Datastreqm print begin" << std::endl;
	  std::cout << "data row count: " << datarows.size() << std::endl;
	  for (std::vector<Datarow*>::const_iterator it = datarows.begin(); it != datarows.end(); ++it) {

	    const Datarow * dr = (*it);
	    std::cout << "datapoints count: " << dr->size() << std::endl;
	    for (std::vector<Pair>::const_iterator itb = dr->begin(); itb != dr->end(); ++itb) {
	      const Pair& p = (*itb);
	      //std::cout << "date: " << p.date << " value: " << p.value << std::endl;
	    };
	  };
	  */
	};

	Handle<Array> toV8Array(Isolate * i);

	typedef std::vector<Pair> Datarow;
	std::vector<Datarow*> datarows;

	Datarow * current;
	
        struct JS {
                
                static void Initialize(Handle<ObjectTemplate> global, Isolate* i) {
		  fprintf(stdout, "Datastream Initialize\n");
		  global->Set(String::NewFromUtf8(i, "Datastream"), FunctionTemplate::New(i, Constructor));
                }

	  static void Constructor(const FunctionCallbackInfo<Value>& info);
	  static void GetDatapoints(Local<String> property, const PropertyCallbackInfo<Value> &info);
	  static void GetX(Local<String> property, const PropertyCallbackInfo<Value> &info);
	  /*
                //TEMP 
                static void InitConnection() {

		  MYSQL * mysql = 0;
		  
                        std::cout << "InitConnection" << std::endl;

                        mysql = mysql_init(0);
                        if (mysql == NULL) {
                                fprintf(stderr, "mysql_init error: %s\n", mysql_error(mysql));
                        }  
                        
                        //if (mysql) {
                        //        std::cout << "mysql has been initialize" << std::end;
                        //
                        //MYSQL * rcon = mysql_real_connect(con, "localhost", "user12", "34klq*", "testdb", 0, NULL, 0);
                        //const std::string host = "http://opensensordata.net"; // 2
                        //const std::string host = "opensensordata@ns3002005"; // 2
                        //const std::string host = "opensensordata@opensensordata.net"; // 2
                        //const std::string host = "opensensordata.net"; // 111
                        //const std::string host = "ns3002005.ovh.net"; // 111
                        //const std::string host = "37.59.4.199"; // 111
                        const std::string host = "localhost"; // 111

                        const std::string usr = "opensensordata";
                        const std::string pwd = "YkfFVwg1";
                        const std::string db = "opensensordata";
                        if (!mysql_real_connect(mysql,
                                                host.data(),
                                                usr.data(),
                                                pwd.data(),
						db.data(), 0, NULL, 0)) {
                                fprintf(stderr, "mysql_real_connect error: %s\n", mysql_error(mysql));
			}
                        

			//char query[] = "SELECT 'id', 'owner', 'name', 'description', 'timezone', 'longitude', 'latitude', 'unit' FROM 'datastream' WHERE 'id'='26'";

			//char query[] = "SELECT `id`, `owner`, `name`, `description`, `timezone`, `longitude`, `latitude`, `unit` FROM `datastream` WHERE `id`=26";
			const char query[] = "SELECT id, owner,  name, description FROM `group` WHERE `id`=42";
			if( mysql_real_query(mysql, query, sizeof(query)) == 0) { // sucess
			  

			  // printf( "%ld Record Found\n",(long)
			  // mysql_affected_rows(con);
			  //fprintf(stderr, "query error.");
			  //fprintf(stderr, "query error.");  
		
			  MYSQL_ROW usr_row;
			  MYSQL_RES * usr_res = 0;
			  my_ulonglong usr_num_rows = 0;
			  
			  usr_res = mysql_store_result(mysql);
			  usr_num_rows = mysql_num_rows(usr_res);

			  std::cout << "Res, num_rows: " << usr_res << usr_num_rows << std::endl;
			  

			  while ( (usr_row = mysql_fetch_row(usr_res)) != NULL) {
			    //printf("%s\n", row[0]);
			    std::cout << "FETCH ROW 0: " << usr_row[0] << std::endl;
			    std::cout << "FETCH ROW 1: " << usr_row[1] << std::endl;
			    std::cout << "FETCH ROW 2: " << usr_row[2] << std::endl;
			  }


			  /// LOAD DATASTREAMS

			  my_ulonglong gid = 42;
			  std::ostringstream s;
			  s << "SELECT datastream FROM `group_datastreams` WHERE `gid`=" ;
			  s << gid;
			  s << " ORDER BY `index`;";
			  const std::string& ds_query = s.str();
			  //const std::string str(+ gid + );
			  
			  if( mysql_real_query(mysql, ds_query.data(), ds_query.size()) == 0) { // success    
			    std::cout << "group data stream success " << std::endl;
			  

			    MYSQL_ROW ds_row;
			    MYSQL_RES * ds_res = 0;
			    my_ulonglong ds_num_rows = 0;
			  
			    ds_res = mysql_store_result(mysql);
			    ds_num_rows = mysql_num_rows(ds_res);
			    
			    std::cout << "Res, num_rows: " << ds_res << ds_num_rows << std::endl;
			    
			    Datastream * datastream = new Datastream;
			    
			    
			    while ((ds_row = mysql_fetch_row(ds_res)) != NULL) {
			      //printf("%s\n", row[0]);
			      std::cout << "FETCH 0 datastream id: " << ds_row[0] << std::endl;
			      //std::cout << "FETCH ROW 1: " << row[1] << std::endl;
			      //std::cout << "FETCH ROW 2: " << row[2] << std::endl;
			      
			      // GET DATAPOINTS
			      //Datastream* datastream = new Datastream;
  
			      std::ostringstream s;
			      s << "SELECT `datetime`,`value` FROM `datapoints` WHERE `datastream`=" ;
			      s << ds_row[0];
			      s << " ORDER BY `datetime`;";

			      const std::string& dp_query = s.str();
			      //const std::string str(+ gid + );
			  
			      if( mysql_real_query(mysql, dp_query.data(), dp_query.size()) == 0) { // success    
				std::cout << "datapoints success " << std::endl;
				
				MYSQL_ROW dp_row;
				MYSQL_RES * dp_res = 0;
				my_ulonglong dp_num_rows = 0;
				
				dp_res = mysql_store_result(mysql);
				dp_num_rows = mysql_num_rows(dp_res);
			    


				while ((dp_row = mysql_fetch_row(dp_res)) != NULL) {
				  

				  if (dp_row[0] && dp_row[1]) { // datetime and value

				    const time_t time = ConvertToTime(std::string(dp_row[0]));
				    std::cout << "FETCH 0 dp: " << dp_row[0] << " " << time << std::endl;

				    std::cout << "FETCH 1 dp: " << dp_row[1] << std::endl;
				    
				    const double value = atof(dp_row[1]);
				    //std::cout << "FETCH B dp: " << value << std::endl;
				    //const double value = dp_row[1];
				    datastream->append(time, value);
				  }
				  else {
				    if (!dp_row[0]) {
				      fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
				    } else if (!dp_row[1]) {
				      //fprintf(stderr, "'value' doesn't exist in this datapoint row...\n");
				      datastream->endDatarow();
				    }
				  }
				  
				}

			      }
			      datastream->print();
			      datastream->clear();
			    }
			  }

			} else {
			  fprintf(stderr, "mysql_real_query error: %s\n", mysql_error(mysql));
			}
			
			if (mysql) {
			  mysql_close(mysql);
			  std::cout << "Mysql Closed" << std::endl;
			}

                        std::cout << "END" << std::endl;
                        
                }
	  */

   
                static void Select(const FunctionCallbackInfo<Value>& info ) {


                        
                        v8::Isolate* i = v8::Isolate::GetCurrent();
                        
                        Local<String> property;
                        int sum = 0;
                        
                        // Get the X property and cast this value to an INT then add it by the argument
                        property = String::NewFromUtf8(i, "x");        
                        sum = info.This()->Get(property)->Int32Value() + info[0]->Int32Value();
                        info.This()->Set(property, Number::New(i, sum));
                        
                        property = String::NewFromUtf8(i, "y");
                        sum = info.This()->Get(property)->Int32Value() + info[1]->Int32Value();
                        info.This()->Set(property, Number::New(i, sum));
                        

                        // TODO
                        
                        // Connection to opensensordatabase

                        // Get array data from JSON array 


                }

        };
};

#endif // DATASTREAMV8_H

