
#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H

#define timestamp_eq(__t1, __t2)         ((__t1) == (__t2)) 
#define timestamp_before(__t1, __t2)     ((__t1) < (__t2)) 
#define timestamp_after(__t1, __t2)      ((__t1) > (__t2)) 

#define timestamp_diff(__t1, __t2)       ((__t2) - (__t1)) 
#define timestamp_nmul(__t1, __n, __t2)  ((__t1) + ((__n) * (__t2))) 

int timestamp_read(timestamp_t* t, int fd);
int timestamp_write(timestamp_t t, int fd);

#endif /* _TIMESTAMP_H */
