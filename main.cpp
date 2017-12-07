#include <cstdlib>
#include <vector>
#include <mutex>

#include "reader.hpp"
#include "writer.hpp"


using namespace std;

/*
 * Main starting point, initalize important parts, define consts
 */
int main(int argc, char** argv) {
    const int AMOUNT_READERS = 5;
    const int AMOUNT_WRITERS = 3;
    const int AMOUNT_DATA = 10;

    vector<int> data(AMOUNT_DATA);
    vector<reader> readers;
    vector<writer> writers;
    
    mutex writeMutex;

    // Initializing of readers
    for (int i = 0; i < AMOUNT_READERS; i++)
    {
        readers.push_back(reader(data));
    }
    
    // Initializing of writers
    for (int i = 0; i < AMOUNT_READERS; i++)
    {
        writers.push_back(writer(data, writeMutex));
    }
    
    return 0;
   
}

