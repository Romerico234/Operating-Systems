#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

static int N;                       
static int M;                       

static sem_t sem_customers;         
static sem_t sem_barberReady;       
static sem_t sem_haircutDone;       

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

static int *queue_ids = NULL;
static int headIdx = 0, tailIdx = 0, waiting = 0;

static volatile bool shop_open = true;
static volatile bool barber_sleeping = false;

static int rand_between(int lo, int hi) { 
    return lo + (rand() % (hi - lo + 1));
}

static void enqueue_customer(int id) {
    queue_ids[tailIdx % N] = id;
    tailIdx++;
    waiting++;
}
static int dequeue_customer(void) {
    int id = queue_ids[headIdx % N];
    headIdx++;
    waiting--;
    return id;
}

static void *barber(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&m);
        if (waiting == 0) {
            if (!shop_open) {    
                pthread_mutex_unlock(&m);
                break;
            }
            barber_sleeping = true; 
            pthread_mutex_unlock(&m);

            sem_wait(&sem_customers);

            pthread_mutex_lock(&m);
            barber_sleeping = false;

            if (!shop_open && waiting == 0) {
                pthread_mutex_unlock(&m);
                break;
            }
        }

        int cust_id = dequeue_customer();
        pthread_mutex_unlock(&m);

        sem_post(&sem_barberReady);

        int secs = rand_between(1, 5);
        printf("Barber starts cutting hair of Customer %d for %d seconds.\n", cust_id, secs);
        sleep(secs);
        printf("Barber finishes cutting hair of Customer %d.\n", cust_id);

        sem_post(&sem_haircutDone);
    }

    printf("Barber goes to sleep.\n");
    return NULL;
}

typedef struct { int id; } customer_arg_t;

static void *customer(void *arg) {
    customer_arg_t *c = (customer_arg_t *)arg;
    int id = c->id;

    sleep(rand_between(1, 5));
    printf("Customer %d arrived.\n", id);

    pthread_mutex_lock(&m);

    if (waiting < N) {
        if (waiting == 0 && barber_sleeping) {
            printf("Customer %d wakes up the barber.\n", id);
        }

        int seat_num = (tailIdx % N) + 1; 
        enqueue_customer(id);
        printf("Customer %d sits in waiting area (chair %d).\n", id, seat_num);

        pthread_mutex_unlock(&m);

        sem_post(&sem_customers);

        sem_wait(&sem_barberReady);

        sem_wait(&sem_haircutDone);
        printf("Customer %d leaves after haircut.\n", id);
    } else {
        pthread_mutex_unlock(&m);
        printf("No chairs available, Customer %d leaves the shop.\n", id);
    }

    free(c);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N_chairs> <M_customers>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if (N <= 0 || M <= 0) {
        fprintf(stderr, "N and M must be positive integers.\n");
        return 1;
    }

    srand((unsigned)time(NULL));

    queue_ids = (int *)malloc(sizeof(int) * N);
    if (!queue_ids) {
        perror("malloc");
        return 1;
    }

    sem_init(&sem_customers, 0, 0);
    sem_init(&sem_barberReady, 0, 0);
    sem_init(&sem_haircutDone, 0, 0);

    pthread_t barber_tid;
    pthread_create(&barber_tid, NULL, barber, NULL);

    pthread_t *cust_tids = (pthread_t *)malloc(sizeof(pthread_t) * M);
    for (int i = 0; i < M; i++) {
        customer_arg_t *arg = (customer_arg_t *)malloc(sizeof(customer_arg_t));
        arg->id = i + 1;
        pthread_create(&cust_tids[i], NULL, customer, arg);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(cust_tids[i], NULL);
    }
    free(cust_tids);

    pthread_mutex_lock(&m);
    shop_open = false;
    pthread_mutex_unlock(&m);

    sem_post(&sem_customers);

    pthread_join(barber_tid, NULL);

    sem_destroy(&sem_customers);
    sem_destroy(&sem_barberReady);
    sem_destroy(&sem_haircutDone);
    pthread_mutex_destroy(&m);
    free(queue_ids);

    return 0;
}
