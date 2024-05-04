#include "main.h"
#define int long long
using namespace std;



// DEBUG
// gdb cacheSim
// (gdb) set args 256 4 16 write-allocate write-back lru < chk2.txt
// (gdb) run
// (gdb) quit





// HELPER FUNCTIONS

void insertAthead(int tag, std::list<int>& blocks) {

    blocks.push_front(tag); 

}

void deleteAtend(std::list<int> &blocks) {

    if (blocks.size() < 1) {
        return;
    }
    blocks.pop_back();

}


void deletion(std::list<int>& blocks, int tag_key) {

    for (std::list<int>::const_iterator it = blocks.begin(); it != blocks.end(); ++it) {    // Way to iterate over a list.
        if ((*it) == tag_key) {
            blocks.erase(it);
            break;
        }
    }
    return;
}

int end_value( std::list<int>& blocks) {

    if (blocks.size() < 1) {
        return -1;
    }


    std::list<int>::const_iterator it = std::prev(blocks.end(), 1);  // Way to get the last value of the list.
    return (*it);

}











// Set_LRU


Set_LRU::Set_LRU(){

}

Set_LRU:: Set_LRU(int capacity){

        set_cap=capacity;
        for(int i=0;i<set_cap;i++){      
            lru.push_front(-1); 
        }

    }

void Set_LRU:: setting_size(int capacity){

    set_cap=capacity;

    for(int i=0;i<set_cap;i++){       
        lru.push_front(-1);  
    }

    tag_dirty[-1]=0;

}

bool Set_LRU:: find(int tag_to_be_matched){


            if(!present[tag_to_be_matched]){

                return false;
            }

            else{
                                    // Bring it to the top of the Cache.
            
            deletion(lru,tag_to_be_matched);                  // O(1)
            
            insertAthead(tag_to_be_matched,lru);      // O(1)                // HERE IS THE PROBLERM!!!!!!

            return true;             
     
            }
}


void Set_LRU:: insert(int tag_key){

        present[end_value(lru)]=false;              // Free cahche space.......                                          
        deleteAtend(lru);                          // o(1)

        insertAthead(tag_key,lru);      
        present[tag_key]=true;
        return;


}


bool Set_LRU:: end_dirty(){

    return tag_dirty[end_value(lru)];

}


void Set_LRU:: make_end_zero(){

    tag_dirty[end_value(lru)]=0;
    present[end_value(lru)]=false;
    return;

}











// Set_fifo


Set_fifo::Set_fifo(){

}

Set_fifo:: Set_fifo(int capacity){

        set_cap=capacity;
        for(int i=0;i<set_cap;i++){      
            q.push(-1); 
        }

    }

void Set_fifo:: setting_size(int capacity){

    set_cap=capacity;

    for(int i=0;i<set_cap;i++){       
        q.push(-1); 
    }

    tag_dirty[-1]=0;

}

bool Set_fifo:: find(int tag_to_be_matched){
            return present[tag_to_be_matched];
}


void Set_fifo:: insert(int tag_to_be_inserted){

    present[q.front()]=false;
    q.pop();                                       
    present[tag_to_be_inserted]=true;
    q.push(tag_to_be_inserted);

}


bool Set_fifo:: end_dirty(){

    return tag_dirty[q.front()];

}


void Set_fifo:: make_end_zero(){

    tag_dirty[q.front()]=0;
    present[q.front()]=false;
    return;

}
















// Cache



Cache::Cache(){
                                                                // default constructor
}

Cache::Cache(int no_of_sets_in_cache2,int no_of_blocks_in_set2,int block_size2,
        bool write_allocate2,bool write_through2,bool lru2){


no_of_sets_in_cache=no_of_sets_in_cache2;
no_of_blocks_in_set=no_of_blocks_in_set2;
block_size=block_size2;
write_allocate=write_allocate2;
write_no_allocate= !(write_allocate2);
write_through=write_through2;
write_back=!(write_through2);
lru=lru2;
fifo=!(lru2);


if(lru2){

    set_array_lru.resize(no_of_sets_in_cache2);
    for(int i=0;i<no_of_sets_in_cache2;i++){
        set_array_lru[i].setting_size(no_of_blocks_in_set2);
    }


}
else if(fifo){
    

    set_array_fifo.resize(no_of_sets_in_cache2);
    for(int i=0;i<no_of_sets_in_cache2;i++){
        set_array_fifo[i].setting_size(no_of_blocks_in_set2);
    }
}
}


bool Cache:: load_hit_or_miss(int memory_address){

    int y=(memory_address/block_size);
    int set_index = (y)%(no_of_sets_in_cache);
    int tag_req= y/(no_of_sets_in_cache);

    bool hit =false;

        if(lru){
            if(set_array_lru[set_index].find(tag_req)){                     // I am here!!!!
                hit =true;
            }
        }

    else if(fifo){
        if(set_array_fifo[set_index].find(tag_req)){
            hit =true;
        }
    }

    return hit;
}


bool Cache:: load_to_cache(int memory_address){

    int y=(memory_address/block_size);
    int set_index = (y)%(no_of_sets_in_cache);
    int tag_req= y/(no_of_sets_in_cache);

    bool block_to_be_replaced_is_dirty;

    if(lru){
            block_to_be_replaced_is_dirty=set_array_lru[set_index].end_dirty();
            set_array_lru[set_index].make_end_zero();
            set_array_lru[set_index].insert(tag_req);   
    }

    else if(fifo){       
            block_to_be_replaced_is_dirty=set_array_fifo[set_index].end_dirty();
            set_array_fifo[set_index].make_end_zero();
            set_array_fifo[set_index].insert(tag_req);     
    }

    return block_to_be_replaced_is_dirty;

}




bool Cache:: store_hit_or_miss(int memory_address){           // Also makes the bolck dirty ,, if found a write back .....


    int y=(memory_address/block_size);
    int set_index = (y)%(no_of_sets_in_cache);

    int tag_req= y/(no_of_sets_in_cache);

    bool hit =false;

    if(lru){
        if(set_array_lru[set_index].find(tag_req)){
            hit =true;
            if(write_back){

                set_array_lru[set_index].tag_dirty[tag_req]=1;
                  
            }
        }
    }

    else if(fifo){
        if(set_array_fifo[set_index].find(tag_req)){
            hit =true;
            if(write_back){

                set_array_fifo[set_index].tag_dirty[tag_req]=1;

            }
        }
    }

    return hit;
}



bool Cache:: store_to_cache(int memory_address){                // Make this!!!!


    int y=(memory_address/block_size);
    int set_index = (y)%(no_of_sets_in_cache);

    int tag_req= y/(no_of_sets_in_cache);

    bool block_to_be_replaced_is_dirty;

    if(lru){  
            block_to_be_replaced_is_dirty=set_array_lru[set_index].end_dirty();
            set_array_lru[set_index].make_end_zero();
            set_array_lru[set_index].insert(tag_req);

            if(write_back){    

                    set_array_lru[set_index].tag_dirty[tag_req]=1;
                        
            }
    }

    else if(fifo){ 
            block_to_be_replaced_is_dirty=set_array_fifo[set_index].end_dirty();
            set_array_fifo[set_index].make_end_zero();
            set_array_fifo[set_index].insert(tag_req);

            if(write_back){

                    set_array_fifo[set_index].tag_dirty[tag_req]=1;

            }
    }

    return block_to_be_replaced_is_dirty;
}


Cache::~Cache(){}  // not needed.











// CACHE_SIM



Cache_Sim::Cache_Sim(){

    // default constructor..
}

Cache_Sim::Cache_Sim(int no_of_sets_in_cache,int no_of_blocks_in_set,int block_size,
        bool write_allocate,bool write_through,bool lru){

my_cache = Cache(no_of_sets_in_cache,no_of_blocks_in_set,block_size,write_allocate,write_through,lru);

total_loads=0;
total_stores=0;
load_hits=0;
load_misses=0;
store_hits=0;
store_misses=0;
total_cycles=0;

}


void Cache_Sim:: Read_trace_file_and_Simulate(){


    int x= ((my_cache.block_size)/4);

    char access_type;
    string memory_address;
    string waste;



while (cin >> access_type >> memory_address >> waste) {

    memory_address=memory_address.substr(2);
    int int_mem_address= stoll(memory_address,0,16);
    
    if(access_type=='l'){

        total_loads++;       

        bool hit = my_cache.load_hit_or_miss(int_mem_address);

            if(hit){
                load_hits++;
                total_cycles+=1;                // Load_cache..
            }
            else{ // miss!!!

                load_misses++;
                bool end_is_Dirty= my_cache.load_to_cache(int_mem_address);

                if(my_cache.write_back){
                        if (end_is_Dirty){
                        total_cycles+=(2+(200*x));
                        }

                    else{
                        total_cycles+=(2+(100*x));
                    }
                }
                else{
                    total_cycles+=(2+(100*x));
                }
            }

    }

    else if(access_type=='s'){

        total_stores++;


        bool hit = my_cache.store_hit_or_miss(int_mem_address);


        if(hit){

            store_hits++;
                                                                        /// Start form here!!
            if(my_cache.write_through){
                    total_cycles+=(1+(100*x));
            }
            else if(my_cache.write_back){
                    total_cycles+=2;        
            }

        }

        else{                                                           // miss!!!!!

            store_misses++;

            if(my_cache.write_no_allocate){
                    total_cycles+=(1+(100*x));                      // Don't touch the Cache....
            }
            else{

                    bool end_is_Dirty=my_cache.store_to_cache(int_mem_address);

                    if(my_cache.write_through){   
                        total_cycles+=(2+(200*x));
                    }
                    else{                                      /// write_back!!!!
                        if(end_is_Dirty){
                            total_cycles+=(2+(200*x));
                        }
                        else{
                            total_cycles+=(2+(100*x));
                        }
                    }
            }
        }
    }        
}

if(my_cache.write_back){


        int dirty_blocks=0;

        if(my_cache.lru){
                for(int i=0;i<(my_cache.no_of_sets_in_cache);i++){

                    Set_LRU focus = my_cache.set_array_lru[i];

                    for (std::pair<int, int> block: focus.tag_dirty){
                        if(block.second==1){
                            dirty_blocks++;
                        }
                    }


                }
        }

        else{
                for(int i=0;i<(my_cache.no_of_sets_in_cache);i++){

                    Set_fifo focus = my_cache.set_array_fifo[i];

                    for (std::pair<int, int> block: focus.tag_dirty){
                        if(block.second==1){
                            dirty_blocks++;
                        }
                    }

                }
        }

 
    total_cycles+=(((100*x))*dirty_blocks);

}



std::cout << "Total loads: " << total_loads << endl;
std::cout << "Total stores: " << total_stores << endl;
std::cout << "Load hits: " << load_hits << endl;
std::cout << "Load misses: " << load_misses << endl;
std::cout << "Store hits: " << store_hits << endl;
std::cout << "Store misses: " << store_misses << endl;
std::cout << "Total cycles: " << total_cycles << endl;

}









// MAIN

signed main(signed argc, char* argv[]){

    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <cache_size> <blocks_per_set> <block_size> <write_policy> <replace_policy> <trace_file>\n";
        return 1; 
    }

    int cache_size = std::stoi(argv[1]);
    int blocks_per_set = std::stoi(argv[2]);
    int block_size = std::stoi(argv[3]);
    bool write_allocate = (std::string(argv[4]) == "write-allocate");
    bool write_through = (std::string(argv[5]) != "write-back");
    bool lru = (std::string(argv[6]) == "lru");

    Cache_Sim simul_1 = Cache_Sim(cache_size, blocks_per_set, block_size, write_allocate, write_through, lru);

    simul_1.Read_trace_file_and_Simulate();

    return 0;
    
}






