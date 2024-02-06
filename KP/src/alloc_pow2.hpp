#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cmath>



const size_t MAX_POW = 10;
const size_t MAX_BLOCK = 1024;

const size_t MIN_POW = 5;
const size_t MIN_BLOCK = 32;

std::vector<size_t> pwrs = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};



struct Allocator{
    std::map<size_t, std::vector<void*>> _free_blocks;

    Allocator(size_t memory_size){
        
        std::vector<size_t>  pwrs(MAX_POW + 1, 0);
        size_t actual_max_power = MAX_POW;
        size_t actual_size = 0;
        size_t power = 0;
        size_t actual_buf = 0;
        while(memory_size > actual_size){

            if(power){
                actual_buf *= 2;
                ++power;
            }else{
                power = MIN_POW;
                actual_buf = MIN_BLOCK;
            }
            
            if(memory_size < actual_size + actual_buf + sizeof(size_t)) actual_max_power = power;
            else{
                ++pwrs[power];
                actual_size += actual_buf + sizeof(size_t);
            }

            if(memory_size < actual_size + MIN_BLOCK + sizeof(size_t)) break;
            
            power %= actual_max_power;
        }

        // // // // // // // // // // 
        // std::cout << "To check number of available 2_pow blocks (from 0 power to 10)\n";
        // for(auto elem : pwrs){
        //     std::cout << elem << ' ';
        // }
        // std::cout << std::endl;
        // // // // // // // // // // 


        size_t cur_block_size = MIN_BLOCK;
        for(size_t i = MIN_POW; i <= MAX_POW; i++){
            if(pwrs[i] == 0) break;

            while(pwrs[i]--){
                size_t* tmp;
                tmp = (size_t*)malloc(cur_block_size + sizeof(size_t));
                if(tmp == NULL){
                    std::cerr << "malloc: error" << std::endl;
                    exit(1);
                }
                tmp[0] = i;
                _free_blocks[i].push_back((void*)tmp);

                // // // // // // // // // //     
                // std::cout << tmp << ' ' << (tmp + 1) << '\n';
                // // // // // // // // // // 
            }
            cur_block_size *= 2;
        }
    }
};



Allocator createMemoryAllocator(size_t memory_size){
    return Allocator(memory_size);
}
void* alloc(Allocator allocator, size_t block_size){
    size_t power = ceil(log2(block_size));
    power = std::max(power, MIN_POW);
    while(power < MAX_POW and allocator._free_blocks.find(power) == allocator._free_blocks.end()){
        power++;
    }

    void* res = NULL;
    if(power <= MAX_POW and allocator._free_blocks.find(power) != allocator._free_blocks.end()){
        if(!allocator._free_blocks[power].empty()){
            res = allocator._free_blocks[power].back() + sizeof(size_t);
            allocator._free_blocks[power].pop_back();   
        }
    }
    // else{
    //     std::cerr << "No block with size of 'block_size'\n";
    // }
    return res;
}
void free(Allocator allocator, void* block){
    if(block != NULL){
        size_t* buf = (size_t*)block - 1;
        size_t key = buf[0];
        if(key >= MIN_POW or key <= MAX_POW or key%2 == 0){
            allocator._free_blocks[key].push_back(buf);
        }
        else{
            std::cerr << "No permission to free the 'block'" << std::endl;
        }
    }
}
