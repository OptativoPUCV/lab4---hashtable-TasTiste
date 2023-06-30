#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long indice = hash(key, map->capacity);
    long primero = indice;

    while(1){
        Pair *actual = map->buckets[indice];

        if (actual == NULL || actual->key == NULL){
            Pair *nuevoPar = (Pair*) malloc(sizeof(Pair));
            nuevoPar->key = strdup(key);
            nuevoPar->value = value;
            map->buckets[indice] = nuevoPar;
            map->size++;
            map->current = indice;
            return;
        }

        indice = (indice + 1) % map->capacity;

        if (indice == primero) return;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair** bucketVAntiguo = map->buckets;
    long capacidadAntigua = map->capacity;

    map->capacity *= 2;
    map->buckets = NULL;
    map->buckets = (Pair**) calloc(map->capacity, sizeof(Pair*));
    map->size = 0;

    for (long i = 0; i < capacidadAntigua; i++){
        Pair* actual = bucketVAntiguo[i];
        if (actual != NULL && actual->key != NULL){
            insertMap(map, actual->key, actual->value);
            free(actual->key);
            free(actual);
        }
    }
}


HashMap * createMap(long capacity) {
    HashMap *mapa = (HashMap*) malloc(sizeof(HashMap));
    mapa->size = 0;
    mapa->capacity = capacity;
    mapa->current = -1;
    mapa->buckets = (Pair**) calloc(capacity,sizeof(Pair*));
    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
    long indice = hash(key, map->capacity);
    long primero = indice;

    while(1){
        Pair *actual = map->buckets[indice];
        if (actual == NULL) return;

        if (strcmp(actual->key,key) == 0){
            actual->key = NULL;
            map->size --;
            return;
        }

        indice = (indice + 1) % map->capacity;

        if(indice == primero) return;
    }
    return;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long indice = hash(key, map->capacity);
    long primero = indice;

    while(1){
        Pair *actual = map->buckets[indice];
        if (actual == NULL) return NULL;

        if (strcmp(actual->key,key) == 0){
            map->current = indice;
            return actual;
        }

        indice = (indice + 1) % map->capacity;

        if(indice == primero) return NULL;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    long indice = 0;
    long primero = indice;
    Pair *actual = map->buckets[indice];

    while(1){
        actual = map->buckets[indice];
        
        if (actual != NULL && actual->key != NULL) break;

        indice = (indice + 1) % map->capacity;
        if(indice == primero) return NULL;
    }
    map->current = indice;
    return actual;
}

Pair * nextMap(HashMap * map) {
    long indice = map->current + 1;
    Pair *actual = map->buckets[indice];

    while(1){
        actual = map->buckets[indice];
        
        if (actual != NULL && actual->key != NULL) break;

        indice++;
        if(indice == map->capacity) return NULL;
    }
    map->current = indice;
    return actual;
}