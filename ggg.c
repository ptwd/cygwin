#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
 
static void *infinite_loop(void *arg)
{
        char b[128];
        do {
                printf("wait for scanf()\n");
                scanf("%s", &b[0]);
                printf("done\n");
        } while (1);
        return (void*)(NULL);
}
 
static void control_cleanup(void *arg)
{
        pthread_t *tids = arg;
        printf("try to kill thread #0\n");
        pthread_cancel(tids[0]);
}
 
static void *control(void *arg)
{
        pthread_cleanup_push(control_cleanup, arg)
        do {
                pthread_testcancel();
        } while (1);
        pthread_cleanup_pop(1);
        return (void*)(NULL);
}
 
int main(int argc, char **argv)
{
        pthread_t tids[2];
        int ret;
        ret = pthread_create(&tids[0], NULL, infinite_loop, &tids[0]);
        if (ret) {
                perror("pthread_create()");
                return EXIT_FAILURE;
        }

        ret = pthread_create(&tids[1], NULL, control, &tids[0]);
        if (ret) {
                perror("pthread_create()");
                return EXIT_FAILURE;
        }
 
        sleep(5);
 
        printf("try to kill thread #1\n");
        pthread_cancel(tids[1]);
 
        pthread_join(tids[0], (void**)NULL);
        pthread_join(tids[1], (void**)NULL);
 
        return EXIT_SUCCESS;
}
