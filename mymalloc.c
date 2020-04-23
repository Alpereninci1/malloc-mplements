#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <unistd.h>

Block *split(Block *b, size_t size){

 int oldsize= b->info.size;
 b->info.size= size;
  Block *q = ((void*)b+b->info.size+sizeof(Block));
    q->info.size = oldsize-size-sizeof(Block);     
  
   return q;
}

void *mymalloc(size_t size){
    static int firstcall = 1;/*executes only once*/
    size = size - (size % 16) +16;
    if(firstcall){
        firstcall=0;
        heap_start = sbrk(sizeof(Block)*2+1024); /*executes only once*/
        if(heap_start== NULL){

            perror("hata");
            exit(0);
        }
        heap_start->info.size = 1024;
        heap_start->info.isfree= 1;
        heap_start-> next= NULL;
        free_list= (void*)heap_start+sizeof(Block) + heap_start->info.size;
        free_list->info.size = 1024;
        free_list->info.isfree = 1;
        
        /*available free space is 1024*/ 
        printf("%p-%p\n", heap_start, free_list);

        if(strategy == firstFit){
           Block *p= heap_start;
           while(p< free_list && (p->info.isfree || p->info.size< size)){
                p= (void *)p + sizeof(Block) + p->info.size;
           }
           if(p == free_list){
             fprintf(stderr,"alan kullanilabilir degil");
             return NULL;
            }
            else if(size < p-> info.size){
                split(p,size);
            } 
            p->info.isfree=1;
            return (void*)p+sizeof(Block); 
            
        }
        else if(strategy == bestFit){
               Block *x= heap_start;
               Block *best = heap_start;
               while(x!=NULL){
                   if(x->info.isfree==0){
                        if(x->info.size >size && x->info.size < best->info.size){

                           best=x;
                        }
                        
                    } 
                   
                }
                x = x->next;

                return (void*)x+sizeof(Block); 
            }  
            
        
        else if(strategy == worstFit){
            Block *y = heap_start;
            Block *worst = heap_start;
            while(y!=NULL){
                if(y->info.size > worst->info.size){
                    worst = y;

                }
                
            }
            split(worst,size);
            return (void*)y+sizeof(Block); 

        }

             
        
    }
 
}


/*free algorithm
    find-free
    coalesce
    */
void *myfree(void *p){
    Block *b = (Block*)(p-sizeof(Block));/*block header*/
    b->info.isfree = 0;
    Block *next = p+b->info.size; /*next block*/
    if(!next->info.isfree){
        b->info.size += next->info.size+sizeof(Block);
        /*TODO: edit end-boundary tag!!!*/
    }

    Block *prev = (void *)b-sizeof(Block); /*previous block*/
    if(!prev->info.isfree){
        prev->info.size += b->info.size+sizeof(Block);
        /*TODO: edit end-boundary tag!!!*/
        b = prev;
    }

}  



   

