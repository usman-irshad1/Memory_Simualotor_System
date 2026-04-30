#include "Virtual_Memory_manager.h"

void Virtual_Memory_manager::  Runner() {
        ofstream clear_disk("HardDisk.txt", ios::trunc);
        FileConfiguration config;
        config.FileParser();
        config.Display();
        unsigned long long frame_address;
        TLB cache(config);
        Page_table pages(config);

        config.Reading_Addresses();
        int instruction_count = 0;
        size_t index = 0;
        for (auto it = config.actual_adress.begin(); it != config.actual_adress.end(); it++) {
            unsigned long long address = *it;
            unsigned long long vpn = config.getVirtual_page_number(address);
            unsigned long long offset = config.offset(address);
			
            bool is_write = (instruction_count % 5 == 0);
            char x;
            if (is_write) {
                x = 'W';
            }
            else {
                x = 'R';
            }

            bool found = cache.find(vpn, frame_address);
            if (found) {

                pages.getPhysicalAddress(vpn, offset, is_write, cache,found, x);
                //cout << "For the virtual address " << vpn << " the frame is " << frame_address << endl;
            }
            if (!found) {
                cache.insert_with_LRU_algo(vpn, pages.getPhysicalAddress(vpn, offset, is_write, cache, x), true);
                //cache.View_table();
            }

            index++;
            instruction_count++;
        }

        cout << "The frames used are " << pages.getFrame_count();
        cache.perfomance_metrics(config.latency, pages.ram_read_count, pages.ram_write_count, pages.dirty_evictions);

    }
void Virtual_Memory_manager::SPRINTER() {

    ofstream clear_disk("HardDisk.txt", ios::trunc);
    ofstream clear_disk2("OPT_logging", ios::trunc);
    FileConfiguration config;
    config.FileParser();
    config.Display();
    unsigned long long frame_address;
    TLB cache(config);
    Page_table pages(config);
    
    config.Reading_Addresses();
    pages.LOOKAHEADINSERTION(config);

    int instruction_count = 0;
    size_t index = 0;
    for (auto it = config.actual_adress.begin(); it != config.actual_adress.end(); it++) {
        unsigned long long address = *it;
        unsigned long long vpn = config.getVirtual_page_number(address);
        unsigned long long offset = config.offset(address);

        bool is_write = (instruction_count % 5 == 0);
        char x;
        if (is_write) {
            x = 'W';
        }
        else {
            x = 'R';
        }

		bool found = cache.find(vpn, frame_address);  //this is the TLB lookup
        if (found) {
            pages.accessPageTableDirectly(vpn, offset, is_write, x); //this is the page table lookup and the actual read/write operation if the 
            //cout << "For the virtual address " << vpn << " the frame is " << frame_address << endl;
        }
        if (!found) {

            unsigned long long p_frame = pages.accessPageTableDirectly(vpn, offset, is_write, x);
            if (p_frame == (unsigned long long) - 1) {
                p_frame = pages.OPT_algorthim_for_getting_pshycal_number(
                    vpn, offset, is_write, cache, config.actual_adress, index, x
                );
            }
            cache.insert_with_LRU_algo(vpn, p_frame, true);
        }

        index++;
        instruction_count++;
    }

    cout << "The frames used are " << pages.getFrame_count();
    cache.perfomance_metrics(config.latency,  pages.ram_read_count, pages.ram_write_count, pages.dirty_evictions);

}

void Virtual_Memory_manager::Walker() {

    ofstream clear_disk("HardDisk.txt", ios::trunc);
    ofstream clear_disk2("OPT_logging", ios::trunc);
    FileConfiguration config;
    config.FileParser();
    config.Display();
    unsigned long long frame_address;
    TLB cache(config);
    Page_table pages(config);

    config.Reading_Addresses();

    int instruction_count = 0;
    size_t index = 0;
    for (auto it = config.actual_adress.begin(); it != config.actual_adress.end(); it++) {
        unsigned long long address = *it;
        unsigned long long vpn = config.getVirtual_page_number(address);
        unsigned long long offset = config.offset(address);

        bool is_write = (instruction_count % 5 == 0);
        char x;
        if (is_write) {
            x = 'W';
        }
        else {
            x = 'R';
        }

        bool found = cache.findFIFOTLB(vpn, frame_address);  //this is the TLB lookup
        if (found) {
            pages.accessPageTableDirectly(vpn, offset, is_write, x); //this is the page table lookup and the actual read/write operation if the 
           
        }
        if (!found) {
            unsigned long long p_frame = pages.accessPageTableDirectly(vpn, offset, is_write, x);
            if (p_frame == (unsigned long long) - 1) {
                p_frame = pages.acccess_using_FIFO(vpn, offset, is_write, cache,  x);
            }
            cache.Insert_with_FIFO_algo(vpn, p_frame, true);
        }

        index++;
        instruction_count++;
    }

    cout << "The frames used are " << pages.getFrame_count();
    cache.perfomance_metrics(config.latency, pages.ram_read_count, pages.ram_write_count, pages.dirty_evictions);

}


void Virtual_Memory_manager::SPRINTER_OPT_TLB() {

    ofstream clear_disk("HardDisk.txt", ios::trunc);
    ofstream clear_disk2("OPT_logging", ios::trunc);
    FileConfiguration config;
    config.FileParser();
    config.Display();
    unsigned long long frame_address;
    TLB cache(config);
    Page_table pages(config);

    config.Reading_Addresses();

    cache.OPT_LOOKAHEAD_INSERTION(config);   
    pages.LOOKAHEADINSERTION(config);         

    int instruction_count = 0;
    size_t index = 0;
    for (auto it = config.actual_adress.begin(); it != config.actual_adress.end(); it++) {
        unsigned long long address = *it;
        unsigned long long vpn = config.getVirtual_page_number(address);
        unsigned long long offset = config.offset(address);

        bool is_write = (instruction_count % 5 == 0);
        char x;
        if (is_write) {
            x = 'W';
        }
        else {
            x = 'R';
        }

  
        bool found = cache.findOPT_TLB(vpn, frame_address);
        if (found) {
            pages.accessPageTableDirectly(vpn, offset, is_write, x);
        }
        if (!found) {
            unsigned long long p_frame = pages.accessPageTableDirectly(vpn, offset, is_write, x);
            if (p_frame == (unsigned long long)-1) {
                p_frame = pages.OPT_algorthim_for_getting_pshycal_number(
                    vpn, offset, is_write, cache, config.actual_adress, index, x
                );
            }
            // OPT-based TLB insertion – passes current index for future scanning
            cache.insert_with_OPT_algo(vpn, p_frame, true, index);
        }

        index++;
        instruction_count++;
    }

    cout << "The frames used are " << pages.getFrame_count();
    cache.perfomance_metrics(config.latency, pages.ram_read_count, pages.ram_write_count, pages.dirty_evictions);

}
