#include "db.h"

#include <iostream>
#include <sstream>

#include "utils.h"

#include "group.h"
#include "datastream.h"
#include "photostream.h"

#include "config.inc.h"


DB * DB::smDB = 0x0;

DB::DB() : mMysql(0x0) {

    
        //std::cout << "InitConnection" << std::endl;
        
        mMysql = mysql_init(0);
        if (mMysql == NULL) {
                fprintf(stderr, "mysql_init error: %s\n", mysql_error(mMysql));
        }  
        
        const std::string host = "localhost"; // 111
        
        const std::string usr = DB_USER;
        const std::string pwd = DB_PASSWORD; // see config.inc.h
        const std::string db = DB_DATABASE;
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


bool DB::loadGroup(Group * group, int id) {
        

        if (group && isConnected()) {
                std::ostringstream s;

                // Get group info

                s << "SELECT id, owner, app, name, description FROM `group` WHERE `id`=";
                s << id;
                s << ";";
                
                const std::string& q1 = s.str();

                bool group_loaded = false;
                if(mysql_real_query(mMysql, q1.data(), q1.size()) == 0) { // success    

                        group_loaded = true;
                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                        
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                
                                if (row[0]) {
                                        group->id = atoi(row[0]);
                                }
                                if (row[1]) {
                                        group->ownerId = atoi(row[1]);
                                }
                                
                                if (row[2]) {
                                        group->ownerId = atoi(row[2]);
                                }
                                
                                char * name = row[3];
                                if (name && strlen(name) < 256) {
                                        strcpy(group->name, name);
                                } else {
                                        fprintf(stderr, "loadGroup error, invalid name.\n");
                                }                                
                        } //end while
                } // end if

                if (!group_loaded) {
                        fprintf(stderr, "loadGroup error, select from 'group' error.\n");
                        return false;
                }

                // Load Datastreams

                std::ostringstream s2;
                s2 << "SELECT datastream FROM `group_datastreams` WHERE `gid`=";
                s2 << id;
                s2 << " ORDER BY `index`;";

                const std::string& q2 = s2.str();

                if(mysql_real_query(mMysql, q2.data(), q2.size()) == 0) { // success    

                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);

                        while ((row = mysql_fetch_row(res)) != NULL) {

                                
                                int datastream_id = 0;
                                if (row[0]) {
                                      datastream_id  = atoi(row[0]);
                                      Datastream * datastream = new Datastream;
                                      loadDatastream(datastream, datastream_id);
                                      group->datastreams.push_back(datastream);
                                }
                        } //end while
                } // end if

                //fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                
                // Load Photostreams
                
                std::ostringstream s3;
                s3 << "SELECT photostream FROM `group_photostreams` WHERE `gid`="
                << id
                << " ORDER BY `index`;";
                
                const std::string& q3 = s3.str();
                
                if(mysql_real_query(mMysql, q3.data(), q3.size()) == 0) { // success    
                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);

                        std::cout << "DB::loadGroup photostream rows: " << num_rows << std::endl;
                
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                
                                int photostream_id = 0;
                                if (row[0]) {
                                        photostream_id  = atoi(row[0]);
                                        Photostream * photostream = new Photostream;
                                        loadPhotostream(photostream, photostream_id);
                                        group->photostreams.push_back(photostream);
                                }
                        } //end while
                } // end if
                
                //fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                
        } // end group && isConnected
 
        return true;
}


bool DB::loadDatastream(Datastream * datastream, int id) {
        
        bool result =  false;
        
        if (datastream && isConnected()) {

                std::ostringstream s;
                
                s << "SELECT `id`,`owner`,`name`,`description` FROM `datastream` WHERE `id`=";
                s << id;
                s << ";";
                
                const std::string& q1 = s.str();
                
                if(mysql_real_query(mMysql, q1.data(), q1.size()) == 0) { // success    
                        
                        result = true;
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                       
                        if (num_rows != 1) {
                                return false;
                        }

                        while ((row = mysql_fetch_row(res)) != NULL) {
                                // GET DATAPOINTS

                                
                                if (row[0]) {
                                        datastream->id = atoi(row[0]);
                                }
                                if (row[1]) {
                                        datastream->ownerId = atoi(row[1]);
                                }
                                
                                char * name = row[2];
                                if (name && strlen(name) < 256) {
                                        strcpy(datastream->name, name);
                                } else {
                                        fprintf(stderr, "loadDatastream error, invalid name.\n");
                                }
                                
                                char * desc = row[3];
                                if (name && strlen(desc) < 256) {
                                        strcpy(datastream->description, desc);
                                } else {
                                        fprintf(stderr, "loadDatastream error, invalid description.\n");
                                }                                
                                
                        } //end while
                }

                //std::cout << "DB::loadDatastream result B " << result << std::endl;

                if (!result)
                        return false;
                
                result = false;



                std::ostringstream s2;
                s2 << "SELECT UNIX_TIMESTAMP(`datetime`),`value` FROM `datapoints` WHERE `datastream`=";
                s2 << id;
                s2 << " ORDER BY `datetime`;";
                
                const std::string& q2 = s2.str();
                
                if(mysql_real_query(mMysql, q2.data(), q2.size()) == 0) { // success    
                        
                        result = true;
                        
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
                                        //std::cout << "DB::loadDatastream value " << value << std::endl;                                        
                                        datastream->appendDatapoints(row[0], time, value);
                                } else {
                                        if (!row[0]) {
                                                fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                                        } else if (!row[1]) {
                                                //fprintf(stderr, "end row A\n");
                                        }
                                }
                                
                        } // end while
                } // end if

                //std::cout << "DB::loadDatastream result B" << result << std::endl;
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
                
                const std::string& q1 = s.str();
                
                if(mysql_real_query(mMysql, q1.data(), q1.size()) == 0) { // success    
                        
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

bool DB::loadPhotostream(Photostream * photostream, int id) {
        
        if (photostream && isConnected()) {

                std::ostringstream s;
                
                s << "SELECT `id`,`owner`,`name`,`description` FROM `photostream` WHERE `id`=";
                s << id;
                s << ";";
                
                const std::string& q1 = s.str();

                bool ps_loaded =  false;                
                if(mysql_real_query(mMysql, q1.data(), q1.size()) == 0) { // success    
                        
                        ps_loaded = true;
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);

                        if (num_rows != 1) {
                                return false;
                        }

                        while ((row = mysql_fetch_row(res)) != NULL) {
                                if (row[0]) {
                                        photostream->id = atoi(row[0]);
                                }
                                if (row[1]) {
                                        photostream->ownerId = atoi(row[1]);
                                }
                                
                                char * name = row[2];
                                if (name && strlen(name) < 512) {
                                        strcpy(photostream->name, name);
                                } else {
                                        fprintf(stderr, "loadPhotostream error, invalid name.\n");
                                }
                                
                                char * desc = row[3];
                                if (name && strlen(desc) < 512) {
                                        strcpy(photostream->description, desc);
                                } else {
                                        fprintf(stderr, "loadPhotostream error, invalid description.\n");
                                }
                                
                                
                        } //end while
                }

                //REMOVE THIS
                //return true;

                if (!ps_loaded) {
                        return false;
                }

                std::ostringstream s2;
                s2 << "SELECT UNIX_TIMESTAMP(`datetime`),`filename` FROM `photos` WHERE `photostream`=";
                s2 << id;
                s2 << " ORDER BY `datetime`;";
                
                const std::string& q2 = s2.str();
                
                if(mysql_real_query(mMysql, q2.data(), q2.size()) == 0) { // success
                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                        
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                // Get PhotoInfo
                                if (row[0] && row[1]) { // datetime and value
                                        PhotoInfo info;
                                        info.datetime = atoi(row[0]);
                                        char * filename = row[1];
                                        if (filename && strlen(filename) < 512) {
                                                strcpy(info.filename, filename);
                                        } else {
                                                fprintf(stderr, "loadPhotostream error, invalid filename.\n");
                                        }
                                         
                                        photostream->append(info);
                                } else {
                                        if (!row[0]) {
                                                fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                                        } else if (!row[1]) {
                                                //fprintf(stderr, "end row A\n");
                                        }
                                }
                                
                        } // end while
                } // end if
        }
       
        
        return true;
}


  
bool DB::isConnected() const {
        return !!mMysql;
}
