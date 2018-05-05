#include <pthread.h>
#include "apue.h"

struct foo {
    int               f_count; //reference counter
    pthread_mutex_t   f_lock;  //reference counter lock
    int               f_id;

    /* more stuff here*/
};

/*allocate the struct object*/
struct foo *
foo_alloc(int id) {
    struct foo *fp;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        
        if (pthread_mutex_init(&(fp->f_lock), NULL) != 0) {
            /*fail to lock*/
            free(fp);
            return(NULL);
        }
        /*other initializateion*/    
    }
    return(fp);
}

/*add a reference to the counter*/
void
foo_hold(struct foo *fp) {
    pthread_mutex_lock(&(fp->f_lock));
    fp->f_count++;
    pthread_mutex_unlock(&(fp->f_lock));
}

void
foo_rele(struct foo *fp) {

    pthread_mutex_lock(&(fp->f_lock));
    if (--fp->f_count == 0) {
        /*last reference is released*/
        pthread_mutex_unlock(&(fp->f_lock));
        pthread_mutex_destroy(&(fp->f_lock));
        free(fp);
    } else {
      pthread_mutex_unlock(&(fp->f_lock));  
    }
}

void main(void) {
    struct foo *fp1 = foo_alloc(1);
    printf("foo %d is created\n", fp1->f_id);
    exit(0);
}
