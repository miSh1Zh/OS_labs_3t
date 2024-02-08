// #include "alloc_MKK.hpp"
#include "alloc_pow2.hpp"

#include <iostream>
#include <chrono>
#include <queue>
#include <vector>

const size_t MEM = 4000000;

using namespace std;

int main(){

    vector<size_t> test_blocks;
    size_t t;
    while(cin >> t){
        test_blocks.push_back(t);
    }

    // cout << "Test blocks count: " << test_blocks.size() << endl;

    auto begin = chrono::high_resolution_clock::now();

    Allocator All(MEM);

    auto end = chrono::high_resolution_clock::now();

    // cout << "Init test time (microseconds): " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    
    queue<void*> Q;

    begin = chrono::high_resolution_clock::now();

    size_t null_count = 0;
    for(auto elem : test_blocks){
        void* tmp = All.alloc(elem);
        // cout << tmp << endl;
        if(tmp == NULL) null_count++;
        else{
            Q.push(tmp);
        }
    }

    end = chrono::high_resolution_clock::now();

    cout << "Allocation test time (microseconds): " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;

    begin = chrono::high_resolution_clock::now();

    while(!Q.empty()){
        All.free(Q.back());
        Q.pop();
    }

    end = chrono::high_resolution_clock::now();

    cout << "Free test time (microseconds): " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;

    cout << "NULL count: " << null_count << endl;

    return 0;
}



// Исследование 2 аллокаторов памяти: необходимо реализовать два алгоритма аллокации памяти и 
// сравнить их по следующим характеристикам:

// Фактор использования
// Скорость выделения блоков
// Скорость освобождения блоков
// Простота использования аллокатора


// Каждый аллокатор памяти должен иметь функции аналогичные стандартным функциям free и 
// malloc (realloc, опционально).  Перед работой каждый аллокатор инициализируется свободными 
// страницами памяти, выделенными стандартными средствами ядра. Необходимо самостоятельно 
// разработать стратегию тестирования для определения ключевых характеристик аллокаторов 
// памяти. При тестировании нужно свести к минимуму потери точности из-за накладных расходов 
// при измерении ключевых характеристик, описанных выше.
// В отчете необходимо отобразить следующее:

// Подробное описание каждого из исследуемых алгоритмов
// Процесс тестирования
// Обоснование подхода тестирования
// Результаты тестирования
// Заключение по проведенной работе