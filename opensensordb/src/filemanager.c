
#include "osdb_priv.h"

static const char* _dir = "/tmp"; 


enum {
        fileop_start = 0,
        fileop_unlink_ts = 1,
        fileop_modify_ds = 2,
        fileop_modify_ts = 3
};

typedef struct _fileop_t {
        id_t datastream;
        id_t timeseries;
        int op;
        int fd;
} fileop_t;

fileop_t* new_fileop(id_t datastream, id_t timeseries, int op);
void delete_fileop(fileop_t* fop);

static hashtable_t* _fileops = NULL;

int filemanager_init(const char* dir)
{
        _dir = strdup(dir);
        if (_dir == NULL)
                return -1;
        return 0;
}

static int filemanager_create_dirs(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d", _dir, a);
        int err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ((err != 0) && (errno != EEXIST))
                return -1;

        snprintf(path, 512, "%s/%02d/%02d", _dir, a, b);
        err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ((err != 0) && (errno != EEXIST))
                return -1;

        snprintf(path, 512, "%s/%02d/%02d/%02d", _dir, a, b, c);
        err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ((err != 0) && (errno != EEXIST))
                return -1;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d", _dir, a, b, c, id);
        err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ((err != 0) && (errno != EEXIST))
                return -1;

        return 0;  
}

int filemanager_datastream_exists(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd", _dir, a, b, c, id);

        struct stat sb;
        int err = stat(path, &sb);
        return (err == 0);
}

int filemanager_create_datastream(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        if (filemanager_create_dirs(id) == -1)
                return -1;
 
        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd", _dir, a, b, c, id);

        int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
        if (fd == -1) {
                log_err("Failed to open the datastream file (%d): %s", 
                        id, strerror(errno));
                return -1;
        }

        return fd;
}

int filemanager_open_datastream(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;
        void* value;

        if ((_fileops != NULL) && (hashtable_lookup(_fileops, id, &value) != 0)) {
                snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd.tmp%d", 
                         _dir, a, b, c, id, getpid());

                int fd = open(path, O_RDWR);
                if (fd == -1) {
                        log_err("Failed to open the temp datastream file (%d): %s", 
                                id, strerror(errno));
                        return -1;
                }

                list_t* ops = (list_t*) value;

                fileop_t* fop = new_fileop(id, (id_t) -1, fileop_modify_ds);
                if (fop == NULL)
                        return -1;

                ops = list_append(ops, fop);
                if (ops == NULL) 
                        return -1;

                fop->fd = fd;

                return -1;

        } else {
                snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd", _dir, a, b, c, id);

                int fd = open(path, O_RDWR);
                
                if (fd == -1) {
                        log_err("Failed to open the datastream file (%d): %s", 
                                id, strerror(errno));
                        return -1;
                }
                
                return fd;
        }
}

int filemanager_open_datastream_description(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.json", _dir, a, b, c, id);
        
        int fd = open(path, O_RDWR);
        
        if (fd == -1) {
                log_err("Failed to open the datastream description file (%d): %s", 
                        id, strerror(errno));
                return -1;
        }
        
        return fd;
}

int filemanager_lock_datastream(id_t id)
{
        char path[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.lock", _dir, a, b, c, id);
 
        int fd = open(path, O_RDWR);
        if (fd == -1)
                return -1;

        if (flock(fd, LOCK_EX) != 0) {
                log_err("Failed to lock the datastream (%d): %s", 
                        id, strerror(errno));
                close(fd);
                return -1;
        }

        return fd;
}

int filemanager_unlock_datastream(id_t datastream, int fd)
{
        if (flock(fd, LOCK_UN) != 0) {
                log_err("Failed to unlock the datastream (%d): %s", 
                        datastream, strerror(errno));
                close(fd);
                return -1;
        }

        if (close(fd) == -1) {
                log_err("Failed to close the datastream lock file (%d): %s", 
                        datastream, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_apply_dataseries(id_t id)
{
        char path[512];
        char bckpath[512];
        char tmppath[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd", 
                 _dir, a, b, c, id);

        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/DS.osd.bck%d", 
                 _dir, a, b, c, id, getpid());

        snprintf(tmppath, 512, "%s/%02d/%02d/%02d/%012d/DS.osd.tmp%d", 
                 _dir, a, b, c, id, getpid());

        if (rename(path, bckpath) != 0) {
                log_err("Failed to rename datastream file for backup (%d): %s", 
                        id, strerror(errno));
                return -1;
        }

        if (rename(tmppath, path) != 0) {
                log_err("Failed to rename temp datastream (%d): %s", 
                        id, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_restore_backup_dataseries(id_t id)
{
        char path[512];
        char bckpath[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/DS.osd", 
                 _dir, a, b, c, id);

        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/DS.osd.bck%d", 
                 _dir, a, b, c, id, getpid());

        if (rename(bckpath, path) != 0) {
                log_err("Failed to restore datastream file from backup (%d): %s", 
                        id, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_unlink_backup_dataseries(id_t id)
{
        char bckpath[512];
        int a = id % 100;
        int b = (id / 100) % 100;
        int c = (id / 10000) % 100;

        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/DS.osd.bck%d", 
                 _dir, a, b, c, id, getpid());

        if (unlink(bckpath) != 0) {
                log_err("Failed to unlink datastream backup file (%d): %s", 
                        id, strerror(errno));
                return -1;
        }

        return 0;
}

int filemanager_open_timeseries(id_t datastream, id_t timeseries)
{
        char path[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;
        void* value;

        if ((_fileops != NULL) && (hashtable_lookup(_fileops, datastream, &value) != 0)) {

                snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.tmp%d", _dir, 
                         a, b, c, datastream, timeseries, getpid());

                int fd = open(path, O_RDWR);
                if (fd == -1) {
                        log_err("Failed to open the temp timeseries file (%d,%d): %s", 
                                datastream, timeseries, strerror(errno));
                        return -1;
                }

                list_t* ops = (list_t*) value;

                fileop_t* fop = new_fileop(datastream, timeseries, fileop_modify_ts);
                if (fop == NULL)
                        return -1;

                ops = list_append(ops, fop);
                if (ops == NULL) 
                        return -1;

                fop->fd = fd;

                return -1;

        } else {
                snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd", _dir, 
                         a, b, c, datastream, timeseries);

                int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
                
                if (fd == -1) {
                        log_err("Failed to open the timeseries file (%d,%d): %s", 
                                datastream, timeseries, strerror(errno));
                        return -1;
                }
                
                return fd;
        }
}

int filemanager_close_timeseries(id_t datastream, id_t timeseries, int fd)
{
        if (close(fd) != 0) {
                log_err("Failed to close the timeseries file (%d, %d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }

        return 0;
}


static int filemanager_apply_timeseries(id_t datastream, id_t timeseries)
{
        char path[512];
        char bckpath[512];
        char tmppath[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;

        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd", 
                 _dir, a, b, c, datastream, timeseries);

        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.bck%d", 
                 _dir, a, b, c, datastream, timeseries, getpid());

        snprintf(tmppath, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.tmp%d", 
                 _dir, a, b, c, datastream, timeseries, getpid());

        if (rename(path, bckpath) != 0) {
                log_err("Failed to rename timeseries file for backup (%d,%d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }

        if (rename(tmppath, path) != 0) {
                log_err("Failed to rename temp timeseries (%d,%d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_backup_timeseries(id_t datastream, id_t timeseries)
{
        char path[512];
        char bckpath[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;
                
        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd", _dir, 
                 a, b, c, datastream, timeseries);
                
        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.bck%d", _dir, 
                 a, b, c, datastream, timeseries, getpid());
                
        if (rename(path, bckpath) != 0) {
                log_err("Failed to backup the timeseries file (%d,%d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_unlink_backup_timeseries(id_t datastream, id_t timeseries)
{
        char bckpath[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;

        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.bck%d", _dir, 
                 a, b, c, datastream, timeseries, getpid());

        if (unlink(bckpath) != 0) {
                log_err("Failed to unlink timeseries backup file (%d): %s", 
                        datastream, strerror(errno));
                return -1;
        }

        return 0;
}

static int filemanager_restore_backup_timeseries(id_t datastream, id_t timeseries)
{
        char path[512];
        char bckpath[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;
                
        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd", _dir, 
                 a, b, c, datastream, timeseries);
                
        snprintf(bckpath, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd.bck%d", _dir, 
                 a, b, c, datastream, timeseries, getpid());
                
        if (rename(bckpath, path) != 0) {
                log_err("Failed to restore the timeseries backup (%d,%d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }

        return 0;
}

static int _filemanager_unlink_timeseries(id_t datastream, id_t timeseries)
{
        char path[512];
        int a = datastream % 100;
        int b = (datastream / 100) % 100;
        int c = (datastream / 10000) % 100;
                
        snprintf(path, 512, "%s/%02d/%02d/%02d/%012d/TS%08d.osd", _dir, 
                 a, b, c, datastream, timeseries);
                
        if (unlink(path) != 0) {
                log_err("Failed to unlink the timeseries file (%d,%d): %s", 
                        datastream, timeseries, strerror(errno));
                return -1;
        }
        return 0;
}

int filemanager_unlink_timeseries(id_t datastream, id_t timeseries)
{
        void* value;

        if ((_fileops != NULL) && (hashtable_lookup(_fileops, datastream, &value) != 0)) {
                
                list_t* ops = (list_t*) value;

                fileop_t* fop = new_fileop(datastream, timeseries, fileop_unlink_ts);
                if (fop == NULL)
                        return -1;

                ops = list_append(ops, fop);
                if (ops == NULL) 
                        return -1;

                return 0;
                
        } else {
                return _filemanager_unlink_timeseries(datastream, timeseries);
        }
}

fileop_t* new_fileop(id_t datastream, id_t timeseries, int op)
{
        fileop_t* fop = (fileop_t*) malloc(sizeof(fileop_t));
        if (fop == NULL) {
                log_err("new_fileop: out of memory");
                return NULL;
        }
        fop->datastream = datastream;
        fop->timeseries = timeseries;
        fop->op = op;
        fop->fd = -1;
        return fop;
}

void delete_fileop(fileop_t* fop)
{
        free(fop);
}

static int filemanager_revert_fileops(id_t datastream, list_t* l, list_t* last);

static void _fileops_delete_list(void* value)
{
        list_t* l = (list_t*) value;
        while (l) {
                delete_fileop((fileop_t*) l->data);
                l = l->next;
        }
        delete_list((list_t*) value);
}

int filemanager_start_fileops(id_t datastream)
{
        if (_fileops == NULL) {
                _fileops = new_hashtable(_fileops_delete_list);
                if (_fileops == NULL)
                        return -1;
        }

        fileop_t* fop = new_fileop(datastream, (id_t) -1, fileop_start);
        if (fop == NULL)
                return -1;

        list_t* l = new_list(fop);
        if (l == NULL) {
                delete_fileop(fop);
                return -1;
        }

        hashtable_insert(_fileops, datastream, l);

        return 0;
}

int filemanager_apply_fileops(id_t datastream)
{
        void* value;
        int ret;
        list_t *l, *cur;
        fileop_t* fop;

        if (hashtable_lookup(_fileops, datastream, &value) == 0)
                return -1;

        l = cur = (list_t*) value;

        while (cur) {
                fop = (fileop_t*) cur->data;
                if (fop->fd != -1) {
                        close(fop->fd);
                        fop->fd = -1;
                }
                cur = cur->next;
        }
        
        cur = l;

        while (cur) {
                fop = (fileop_t*) cur->data;

                if (fop->op == fileop_unlink_ts) {
                        ret = filemanager_backup_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                break;

                } else if (fop->op == fileop_modify_ds) {
                        ret = filemanager_apply_dataseries(fop->datastream);
                        if (ret != 0)
                                break;

                } else if (fop->op == fileop_modify_ts) {
                        ret = filemanager_apply_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                break;
                }

                cur = cur->next;
        }
        
        if (cur != NULL) {
                log_err("filemanager: Failed to apply the changes. Reverting the operations.");
                return filemanager_revert_fileops(datastream, l, cur);
        }

        while (l != cur) {
                fop = (fileop_t*) cur->data;

                if (fop->op == fileop_unlink_ts) {
                        ret = filemanager_unlink_backup_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                log_warn("Failed to unlink the timeseries backup. Continuing.");

                } else if (fop->op == fileop_modify_ds) {
                        ret = filemanager_unlink_backup_dataseries(fop->datastream);
                        if (ret != 0)
                                log_warn("Failed to unlink the timeseries backup. Continuing.");

                } else if (fop->op == fileop_modify_ts) {
                        ret = filemanager_unlink_backup_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                log_warn("Failed to unlink the timeseries backup. Continuing.");
                }

                cur = cur->next;
        }

        hashtable_remove(_fileops, datastream);

        return 0;
}

int filemanager_forget_fileops(id_t datastream)
{
        void* value;

        if (hashtable_lookup(_fileops, datastream, &value) == 0)
                return -1;

        list_t *l = (list_t*) value;

        while (l) {
                fileop_t* fop = (fileop_t*) l->data;
                if (fop->fd != -1) {
                        close(fop->fd);
                        fop->fd = -1;
                }
                l = l->next;
        }

        hashtable_remove(_fileops, datastream);

        return 0;
}

static int filemanager_revert_fileops(id_t datastream, list_t* l, list_t* last)
{
        int ret;

        while (l != last) {

                fileop_t* fop = (fileop_t*) l->data;

                if (fop->op == fileop_unlink_ts) {
                        ret = filemanager_restore_backup_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                break;

                } else if (fop->op == fileop_modify_ds) {
                        ret = filemanager_restore_backup_dataseries(fop->datastream);
                        if (ret != 0)
                                break;

                } else if (fop->op == fileop_modify_ts) {
                        ret = filemanager_restore_backup_timeseries(fop->datastream, fop->timeseries);
                        if (ret != 0)
                                break;
                }

                l = l->next;
        }
        return 0;
}

static int _is_name_valid(const char* name)
{
        int len = strlen(name);
        if (len < 2) {
                log_err("Name is too short: %s", name);
                return 0;
        }
        if (len > 63) {
                log_err("Name is too long: %s", name);
                return 0;
        }
        char c = name[0];
        if (((c < 'a') || (c > 'z'))
            && ((c < 'A') || (c > 'Z'))
            && ((c < '0') || (c > '9'))) {
                log_err("Invalid name: %s", name);
                return 0;
        }
        for (int i = 1; i < len; i++) {
                char c = name[i];
                if (((c < 'a') || (c > 'z'))
                    && ((c < 'A') || (c > 'Z'))
                    && ((c < '0') || (c > '9'))
                    && (c != '-')) {
                        log_err("Invalid name: %s", name);
                        return 0;
                }
        }
        return 1;
}

int filemanager_get_script(const char* account, const char* name, char** script)
{
        char path[512];
        struct stat sb;

        *script = NULL;
                
        if (!_is_name_valid(account)
            || !_is_name_valid(name)) {
                return -1;
        }

        char a = account[0];
        char b = account[1];

        snprintf(path, 512, "%s/%c/%c/%s/scripts/%s.js", _dir, a, b, account, name);

        if ((stat(path, &sb) != 0)
            || !S_ISREG(sb.st_mode)
            || (sb.st_size == 0)) {
                return -2;
        }

        char* buffer = malloc(sb.st_size + 1);
        FILE* fp = fopen(path, "r");
        if (fp == NULL)
                return -3;

        size_t n = fread(buffer, 1, sb.st_size, fp);
        if (n != sb.st_size) {
                fclose(fp);
                return -3;
        }

        buffer[sb.st_size] = 0;
        *script = buffer;

        fclose(fp);

        return 0;
}

void filemanager_release_script(char* buffer)
{
        free(buffer);
}
