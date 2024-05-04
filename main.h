#include <iostream>
#include <string>
#include<queue>
#include<unordered_map>
#include<map>
#include<list>
#define int long long


// HELPER

void insertAthead( int tag, std::list<int>& blocks);
void deleteAtend(std::list<int>& blocks);
void deletion(std::list<int>& blocks, int tag);
int end_value( std::list<int>& blocks);




// Set_LRU

class Set_LRU {
public:
    int set_cap;
    std::list<int> lru;
    std::unordered_map<int, bool> present;
    std::map<int, int> tag_dirty;              // May get initialize differently in different c compilers... (so initialize later!!!)


    Set_LRU();
    Set_LRU(int size);
    void setting_size(int size);
    bool find(int tag_to_be_matched);
    void insert(int tag_to_be_inserted);
    bool end_dirty();
    void make_end_zero();
};



// Set_fifo


class Set_fifo {
public:
    int set_cap;
    std::queue<int> q;
    std::unordered_map<int, bool> present;
    std::map<int, int> tag_dirty;              // May get initialize differently in different c compilers... (so initialize later!!!)


    Set_fifo();
    Set_fifo(int size);
    void setting_size(int size);
    bool find(int tag_to_be_matched);
    void insert(int tag_to_be_inserted);
    bool end_dirty();
    void make_end_zero();
};




// Cache


class Cache {
public:
    int no_of_sets_in_cache;
    int no_of_blocks_in_set;
    int block_size;
    bool write_allocate;
    bool write_no_allocate;
    bool write_through;
    bool write_back;
    bool lru;
    bool fifo;
    std::vector<Set_fifo> set_array_fifo;
    std::vector<Set_LRU> set_array_lru;
    

    Cache();
    Cache(int no_of_sets_in_cache2, int no_of_blocks_in_set2, int block_size2,
          bool write_allocate2, bool write_through2, bool lru2);
    bool load_hit_or_miss(int memory_address);
    bool load_to_cache(int memory_address);
    bool store_hit_or_miss(int memory_address);
    bool store_to_cache(int memory_address);

    ~Cache();
};









// Cache_Sim

class Cache_Sim {

private:

    Cache my_cache;
    int total_loads;
    int total_stores;
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cycles;

public:

    Cache_Sim();
    Cache_Sim(int no_of_sets_in_cache, int no_of_blocks_in_set, int block_size, bool write_allocate, bool write_through, bool lru);
    void Read_trace_file_and_Simulate();

};

