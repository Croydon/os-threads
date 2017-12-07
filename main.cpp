#include <cstdlib>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h> 
#include <functional> // std::ref


using namespace std;

mutex writeMutex;
sem_t writeSemaphore;
mutex readMutex;
sem_t readSemaphore;


void reader(vector<int>& data, vector<bool>& readingDataState, const int& AMOUNT_DATA)
{
    // FIXME: Implement better routine when it's time for reader and writer to be executed
    sem_wait(&readSemaphore);
    
    // Do not allow write operations right now
    writeMutex.lock();
    
    cout << "Reader is active!" << endl;
        
    // We are going to search for a readable element now, lock everything in this time
    readMutex.lock();
    
    bool foundReadable = false;
    int i = 0;
    int randomNum;
    while(foundReadable == false && i < data.size())
    {
        randomNum = rand() % AMOUNT_DATA;

        if(readingDataState[randomNum] == false)
        {
            readingDataState[randomNum] = true;
            
            // We just found an element and reserved it for us, unlock other readers
            readMutex.unlock();
            
            foundReadable = true;
        }
        i++;
    }
    
    if(foundReadable == true)
    {
        // Output the element we got
        cout << "A Reader found at data index: " << randomNum << " the value " << data[randomNum] << endl << endl;

        // We have done the reading, not lets free the element for other readers
        readingDataState[randomNum] = false;
    }
    
    // Allow write operations now again
    writeMutex.unlock();
    
}

void writer(vector<int>& data)
{
    sem_wait(&writeSemaphore);
    writeMutex.lock();

    cout << "writer is active!" << endl;
    for (int i = 0; i < data.size(); i++)
    {
        data[i] = rand() * rand();
    }

    writeMutex.unlock();
}

/*
 * Main starting point, initalize important parts, define consts
 */
int main(int argc, char** argv) {
    const int AMOUNT_READERS = 5;
    const int AMOUNT_WRITERS = 3;
    const int AMOUNT_DATA = 10;

    vector<int> data(AMOUNT_DATA, 0);
    vector<bool> readingDataState(AMOUNT_DATA, false);

    srand(time(NULL)); // seed for the random generator

    sem_init(&writeSemaphore, 0, 0);
    
    // Set the max amount of parallel readers to the max amount of existing data elements
    sem_init(&readSemaphore, 0, AMOUNT_DATA);

    
    // Initializing of readers
    for (int i = 0; i < AMOUNT_READERS; i++)
    {
        thread readerThread(reader, ref(data), ref(readingDataState), ref(AMOUNT_DATA));
        readerThread.detach();
    }

    // Initializing of writers
    for (int i = 0; i < AMOUNT_WRITERS; i++)
    {
        thread writerThread(writer, ref(data));
        writerThread.detach();
    }

    
    while(true)
    {
        sem_post(&writeSemaphore);
        sem_post(&readSemaphore);
        sem_post(&readSemaphore);
        sem_post(&readSemaphore);
        sleep(2);
    }
    

    return 0;
}

