#include "db.h"

#include <iostream>
#include <sstream>

#include "utils.h"

#include "account.h"
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


bool DB::loadAccount(Account * account, int id) {
        

        if (account && isConnected()) {
                std::ostringstream s;

                // Get account info

                s << "SELECT id, username, email FROM `account` WHERE `id`=";
                s << id;
                s << ";";
                
                const std::string& q1 = s.str();

                bool account_loaded = false;
                if(mysql_real_query(mMysql, q1.data(), q1.size()) == 0) { // success    


                        
                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                        
                        if (num_rows != 1) {
                                fprintf(stderr, "DB::loadAccount error no account found, or multiple account found.\n");
                                return false;
                        }

                        account_loaded = true;
       
                        while ((row = mysql_fetch_row(res)) != NULL) {
                                
                                if (row[0]) {
                                        account->id = atoi(row[0]);
                                }
                                
                                char * username = row[1];
                                if (username && strlen(username) < 512) {
                                        strcpy(account->username, username);
                                } else {
                                        fprintf(stderr, "loadAccount error, invalid username.\n");
                                }

                                char * email = row[2];
                                if (email && strlen(email) < 512) {
                                        strcpy(account->email, email);
                                } else {
                                        fprintf(stderr, "loadAccount error, invalid name.\n");
                                }
                                
                        } //end while
                        mysql_free_result(res);
                } // end if

                if (!account_loaded) {
                        fprintf(stderr, "DB::loadAccount account not found\n");
                        return false;
                }

                // Load Group

                std::ostringstream s2;
                s2 << "SELECT `id`,`name`,`description` FROM `group` WHERE `owner`=";
                s2 << id;
                s2 << ";";

                const std::string& q2 = s2.str();

                if(mysql_real_query(mMysql, q2.data(), q2.size()) == 0) { // success    

                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);

                        while ((row = mysql_fetch_row(res)) != NULL) {

                                int group_id = 0;
                                if (row[0]) {
                                      group_id  = atoi(row[0]);
                                      Group * group = new Group;
                                      loadGroup(group, group_id);
                                      account->groups.push_back(group);
                                }
                        } //end while
                        mysql_free_result(res);
                } // end if
                
                //fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                
        } // end account && isConnected
 
        return true;
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

                        if (num_rows != 1) {
                                return false;
                        }

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
                        mysql_free_result(res);
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
                                      //loadDatastreamContiguous(datastream, datastream_id);
                                      group->datastreams.push_back(datastream);
                                }
                        } //end while
                        mysql_free_result(res);
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
                        mysql_free_result(res);
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
                        

                        MYSQL_ROW row;
                        MYSQL_RES * res = 0;
                        my_ulonglong num_rows = 0;
                        
                        res = mysql_store_result(mMysql);
                        num_rows = mysql_num_rows(res);
                       
                        if (num_rows != 1) {
                                mysql_free_result(res);
                                return false;
                        }

                        result = true;

                        while ((row = mysql_fetch_row(res)) != NULL) {
                                // GET DATAPOINTS

                                
                                if (row[0]) {
                                        datastream->id = atoi(row[0]);
                                }
                                if (row[1]) {
                                        datastream->ownerId = atoi(row[1]);
                                }
                                
                                char * name = row[2];
                                if (name && strlen(name) < 512) {
                                        strcpy(datastream->name, name);
                                } else {
                                        fprintf(stderr, "loadDatastream error, invalid name.\n");
                                }
                                
                                char * desc = row[3];
                                if (name && strlen(desc) < 512) {
                                        strcpy(datastream->description, desc);
                                } else {
                                        fprintf(stderr, "loadDatastream error, invalid description.\n");
                                }                                
                                
                        } //end while
                        mysql_free_result(res);
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

                                // Simple Way

                                if (row[0] && row[1]) { // datetime and value
                                        const time_t time = atoi(row[0]);
                                        const double value = atof(row[1]);

                                        datastream->appendDatapoints(row[0], time, value);

                                        // Contiguous way
                                        datastream->append(row[0], time, value);
                                } else {
                                        if (!row[0]) {
                                                fprintf(stderr, "'datetime' doesn't exist in this datapoint row...\n");
                                        } else if (!row[1]) {
                                                // Contiguous Way
                                                datastream->endDatarow();
                                        }
                                }

                                
                        } // end while
                        datastream->endDatarow();
                        mysql_free_result(res);
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
                                
                        } // end while
                        
                        datastream->endDatarow();
                        mysql_free_result(res);
                } // end if
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
                        mysql_free_result(res);
                } // end if

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
                        mysql_free_result(res);
                } // end if
        }
       
        
        return true;
}


  
bool DB::isConnected() const {
        return !!mMysql;
}
