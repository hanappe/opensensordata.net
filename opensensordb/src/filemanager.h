
#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

int filemanager_init(const char* dir); 

int filemanager_start_fileops(id_t datastream); 
int filemanager_apply_fileops(id_t datastream); 
int filemanager_forget_fileops(id_t datastream);

int filemanager_datastream_exists(id_t datastream); 
int filemanager_create_datastream(id_t datastream);
int filemanager_open_datastream(id_t datastream); 
int filemanager_lock_datastream(id_t id);
int filemanager_unlock_datastream(id_t datastream, int fd);

int filemanager_open_datastream_description(id_t id);

int filemanager_open_timeseries(id_t datastream, id_t timeseries); 
int filemanager_close_timeseries(id_t datastream, id_t timeseries, int fd); 
int filemanager_unlink_timeseries(id_t datastream, id_t timeseries);

int filemanager_get_script(const char* account, const char* name, char** script);
void filemanager_release_script(char* script);

#endif /* _FILEMANAGER_H */
