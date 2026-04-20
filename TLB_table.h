#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <cmath>
#include "FileConfiguration.h"

using namespace std;

struct Value {
	unsigned long long physical_memory;
	bool valid_bit;
	Value();
};

class TLB {
	unsigned long long cap;
	unordered_map<unsigned long long, Value> table;
	list<unsigned long long> index_for_FIFO;
	unordered_map<unsigned long long, typename list<unsigned long long>::iterator> LRU_map;
	list<unsigned long long> LRU_queue;
	int request;
	int tlb_hits;
	int tlb_misses;
	int count;

public:
	TLB(FileConfiguration& config);

	void View_table();

	void Insert_with_FIFO_algo(unsigned long long virtual_address_key, unsigned long long physical_memory_value, bool valid_bit_value);

	void insert_with_LRU_algo(unsigned long long virtual_address_key, unsigned long long physical_memory_frame, bool valid_bit_value);

	bool find(unsigned long long virtual_address_key, unsigned long long& physical_memory_value);

	void perfomance_metrics(unsigned long long ram_latency, unsigned long long total_ram_reads, unsigned long long total_ram_writes, unsigned long long dirty_evictions);

	int getFileCount();
	bool findFIFOTLB(unsigned long long virtual_address_key, unsigned long long& physical_memory_value);

	void TLB_Eviction(unsigned long long vpn);
};