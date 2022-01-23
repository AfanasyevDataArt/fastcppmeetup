#include <iostream>
#include <functional>
#include <getopt.h>
#include "time_tracker.h"

#include <vector>
#include <atomic>
#include <thread>

float cache(uint32_t n){
    const size_t v_size = 25*1024*1024;
    std::vector<int> v(v_size);
    std::generate(v.begin(), v.end(), [](){return std::rand() % v_size;});

    switch (n) {
        case 0:
        {
            const size_t limit = 8;
            auto start = mytt::start();
            for(size_t i=0 ; i<v_size-1 ; i++){
                size_t offset = (i + v[i] % limit) % v_size ;
                int tmp = v[i];
                v[i] = v[offset];
                v[offset] = tmp;
            }
            return mytt::stop(start);
        }
            break;
        case 1:
        {
            auto start = mytt::start();
            for(size_t i=0 ; i<v_size-1 ; i++){
                size_t offset = (i + v[i]) % v_size;
                int tmp = v[i];
                v[i] = v[offset];
                v[offset] = tmp;
            }
            return mytt::stop(start);
        }
            break;
    }
    return 0.0;
}

float cacheline(uint32_t n){
    const size_t v_size = 10*1024*1024;
    std::vector<int> v(v_size);
    std::generate(v.begin(), v.end(), [](){return std::rand();});

    switch (n) {
        case 0:
        {
            std::atomic_int sum; sum = 0;

            auto start = mytt::start();

            std::thread tA([&sum, &v](){
                for(size_t i=0 ; i<v_size/4 ; i++){
                    sum += v[i] % 32;
                }
            });

            std::thread tB([&sum, &v](){
                for(size_t i=v_size/4 ; i<v_size/2 ; i++){
                    sum += v[i] % 32;
                }
            });

            std::thread tC([&sum, &v](){
                for(size_t i=v_size/2 ; i<3*v_size/4 ; i++){
                    sum += v[i] % 32;
                }
            });

            std::thread tD([&sum, &v](){
                for(size_t i=3*v_size/4 ; i<v_size ; i++){
                    sum += v[i] % 32;
                }
            });

            tA.join();
            tB.join();
            tC.join();
            tD.join();

            return mytt::stop(start);
        }
            break;
        case 1:
        {
            struct{
                std::atomic_int A;
                std::atomic_int B;
                std::atomic_int C;
                std::atomic_int D;
            }sum;
            sum.A = 0;
            sum.B = 0;
            sum.C = 0;
            sum.D = 0;

            auto start = mytt::start();

            std::thread tA([&sum, &v](){
                for(size_t i=0 ; i<v_size/4 ; i++){
                    sum.A += v[i] % 32;
                }
            });

            std::thread tB([&sum, &v](){
                for(size_t i=v_size/4 ; i<v_size/2 ; i++){
                    sum.B += v[i] % 32;
                }
            });

            std::thread tC([&sum, &v](){
                for(size_t i=v_size/2 ; i<3*v_size/4 ; i++){
                    sum.C += v[i] % 32;
                }
            });

            std::thread tD([&sum, &v](){
                for(size_t i=3*v_size/4 ; i<v_size ; i++){
                    sum.D += v[i] % 32;
            }
            });

            tA.join();
            tB.join();
            tC.join();
            tD.join();

            return mytt::stop(start);
        }
            break;
        case 2:
        {
            std::atomic_int sumA; sumA = 0;
            int CACHE_LINE_SIZE = 64;
            uint8_t A[CACHE_LINE_SIZE];
            std::atomic_int sumB; sumB = 0;
            uint8_t B[CACHE_LINE_SIZE];
            std::atomic_int sumC; sumC = 0;
            uint8_t C[CACHE_LINE_SIZE];
            std::atomic_int sumD; sumD = 0;
            uint8_t D[CACHE_LINE_SIZE];

            std::cout << A << B << C << D << std::endl;

            auto start = mytt::start();

            std::thread tA([&sumA, &v](){
                for(size_t i=0 ; i<v_size/4 ; i++){
                    sumA += v[i] % 32;
                }
            });

            std::thread tB([&sumB, &v](){
                for(size_t i=v_size/4 ; i<v_size/2 ; i++){
                    sumB += v[i] % 32;
                }
            });

            std::thread tC([&sumC, &v](){
                for(size_t i=v_size/2 ; i<3*v_size/4 ; i++){
                    sumC+= v[i] % 32;
                }
            });

            std::thread tD([&sumD, &v](){
                for(size_t i=3*v_size/4 ; i<v_size ; i++){
                    sumD += v[i] % 32;
                }
            });

            tA.join();
            tB.join();
            tC.join();
            tD.join();

            return mytt::stop(start);
        }
            break;
    }
    return 0.0;
}

float prefetcher(uint32_t n){
    const size_t v_size = 10*1024*1024;
    std::vector<int> v(v_size);
    std::generate(v.begin(), v.end(), [](){return std::rand() % 3 - 1;});

    switch (n) {
        case 0:
        {
            auto start = mytt::start();
            auto sum_pos = std::count_if(v.begin(), v.end(), [](const int& e){return e > 0;});
            auto dur = mytt::stop(start);
            std::cout << sum_pos << std::endl;
            return dur;
        }
            break;
        case 1:
        {
            std::sort(v.begin(), v.end());
            auto start = mytt::start();
            auto sum_pos = std::count_if(v.begin(), v.end(), [](const int& e){return e > 0;});
            auto dur = mytt::stop(start);
            std::cout << sum_pos << std::endl;
            return dur;
        }
            break;
    }
    return 0.0;
}

struct Data{
    short num;
    char letter;
    mutable double value;
};

#pragma pack(push,1)
struct DataAlign{
    short num;
    char letter;
    mutable double value;
};
#pragma pack(pop)

void struct_test(){
    std::cout << "Not aligned:" << std::endl;
    std::cout << "num: " << offsetof(Data, num) << std::endl;
    std::cout << "letter: " << offsetof(Data, letter) << std::endl;
    std::cout << "value: " << offsetof(Data, value) << std::endl;

    std::cout << "Aligned: " << std::endl;
    std::cout << "num: " << offsetof(DataAlign, num) << std::endl;
    std::cout << "letter: " << offsetof(DataAlign, letter) << std::endl;
    std::cout << "value: " << offsetof(DataAlign, value) << std::endl;
}

float memaling(uint32_t n){
    struct_test();

    const size_t v_size = 25*1024*1024;

    switch (n) {
        case 0:
        {
            std::vector<Data> v(v_size);
            std::generate(v.begin(), v.end(), [](){
                return Data{0, 'a', (double)(std::rand() % v_size)/1000};});

            auto start = mytt::start();
            std::for_each(v.begin(), v.end(), [](const Data &e){ e.value += 3.14;});
            return mytt::stop(start);
        }
            break;
        case 1:
        {
            std::vector<DataAlign> v(v_size);
            std::generate(v.begin(), v.end(), [](){
                return DataAlign{0, 'a', (double)(std::rand() % v_size)/1000};});

            auto start = mytt::start();
            std::for_each(v.begin(), v.end(), [&v](const DataAlign &e){ e.value += 3.14;});
            return mytt::stop(start);
        }
            break;
        case 2:
        {
            std::vector<size_t> idx(v_size);
            std::generate(idx.begin(), idx.end(), [](){ return std::rand() % v_size;});
            std::vector<Data> v(v_size);
            std::generate(v.begin(), v.end(), [](){
                return Data{0, 'a', (double)(std::rand() % v_size)/1000};});

            auto start = mytt::start();
            std::for_each(idx.begin(), idx.end(), [&v](const size_t &e){ v[e].value += 3.14;});
            return mytt::stop(start);
        }
            break;
        case 3:
        {
            std::vector<size_t> idx(v_size);
            std::generate(idx.begin(), idx.end(), [](){ return std::rand() % v_size;});
            std::vector<DataAlign> v(v_size);
            std::generate(v.begin(), v.end(), [](){
                return DataAlign{0, 'a', (double)(std::rand() % v_size)/1000};});

            auto start = mytt::start();
            std::for_each(idx.begin(), idx.end(), [&v](const size_t &e){ v[e].value += 3.14;});
            return mytt::stop(start);
        }
            break;
    }
    return 0.0;
}

float test(uint32_t n){
    auto start = mytt::start();
    std::cout << "test " << n  <<  std::endl;
    return mytt::stop(start);
}

struct Settings{
    uint32_t n;
    std::string name;
    std::function<float(uint32_t)> func;
};

Settings parse_opt(int argc, char** argv){
    Settings setting = {0, "test", test};

    try{
        int c;
        while ((c = getopt(argc, argv, "n:c:")) != -1){
            switch(c){
                case 'c':
                    setting.n = atoi(optarg);
                    break;
                case 'n':
                    setting.name = std::string(optarg);
                    if(setting.name == "cache"){
                        setting.func = cache;
                        break;
                    }
                    if(setting.name == "cacheline"){
                        setting.func = cacheline;
                        break;
                    }
                    if(setting.name == "prefetcher"){
                        setting.func = prefetcher;
                        break;
                    }
                    if(setting.name == "memalign"){
                        setting.func = memaling;
                        break;
                    }
                    break;
            }
        }
    }
    catch (const std::exception& e){
        std::cerr<< e.what();
    }

    return  setting;
}

int main(int argc, char** argv) {
    auto s = parse_opt(argc, argv);
    auto dur = s.func(s.n);
    std::cout
    << "{"
    << "\"name\": \"" << s.name << "\""
    << ", \"n\":"<< s.n
    << ", \"time_ms\": " << dur*1000
    << "}"
    << std::endl;
    return 0;
}
