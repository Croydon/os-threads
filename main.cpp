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


void reader(vector<int>& data, vector<bool>& readingDataState, int& counterReader, int& counterWriter, const int& FORCE_CHANGE_AFTER_ROUNDS)
{
    
    while(true)
    {
        readMutex.lock();
        
        
        if(counterReader >= FORCE_CHANGE_AFTER_ROUNDS)
        {
            readMutex.unlock();
            sleep(1);
            continue;
            //return;
        }
        counterReader++;
        readMutex.unlock();

        // FIXME: Implement better routine when it's time for reader and writer to be executed
        sem_wait(&readSemaphore);

        // Do not allow write operations right now
        sem_wait(&writeSemaphore);
        // writeMutex.lock();

        counterWriter = 0;
        cout << "Reader " << this_thread::get_id() << " is active!" << endl;
        
        
        sleep(1);

        // We are going to search for a readable element now, lock everything in this time
        //readMutex.lock();

        bool foundReadable = false;
        int i = 0;
        int randomNum;
        while(foundReadable == false && i < data.size())
        {
            randomNum = rand() % data.size();
            readMutex.lock();
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
            readMutex.lock();
            // Output the element we got
            cout << "A Reader found at data index: " << randomNum << " the value " << data[randomNum] << endl << endl;
            
            // We have done the reading, not lets free the element for other readers
            readingDataState[randomNum] = false;
             readMutex.unlock();
        }
        
        sem_post(&writeSemaphore);
        sem_post(&readSemaphore);
        sleep(1);
        //return;

        // Allow write operations now again
        //writeMutex.unlock();
    }
}

void writer(vector<int>& data, int& counterReader, int& counterWriter, const int& FORCE_CHANGE_AFTER_ROUNDS)
{
   
    while(true)
    {
        writeMutex.lock();
         
        if(counterWriter >= FORCE_CHANGE_AFTER_ROUNDS)
        {
            writeMutex.unlock();
            sleep(1);
            continue;
            //return;
            
        }
        counterWriter++;
         writeMutex.unlock();
        

        sem_wait(&writeSemaphore);
        
        
        counterReader = 0;

        cout << "Writer " << this_thread::get_id() << " is active!" << endl;
        sleep(1);
          cout<<"point of writethred"<<endl;
        
        writeMutex.lock();
        for (int i = 0; i < data.size(); i++)
        {
            data[i] = rand() * rand();
        }
        
        writeMutex.unlock();
        sem_post(&writeSemaphore);
        sleep(1);
       
        //return;
    }
}

/*
 * Main starting point, initalize important parts, define consts
 */
int main(int argc, char** argv) {
    const int AMOUNT_READERS = 5;
    const int AMOUNT_WRITERS = 23;
    const int AMOUNT_DATA = 20;
    const int AMOUNT_FORCE_CHANGE_AFTER_ROUNDS = 10;
    
    int counterReader = 0;
    int counterWriter = 0;
    
    vector<int> data(AMOUNT_DATA, 0);
    vector<bool> readingDataState(AMOUNT_DATA, false);

    srand(time(NULL)); // seed for the random generator

    sem_init(&writeSemaphore, 0, 1);
    
    // Set the max amount of parallel readers to the max amount of existing data elements
    sem_init(&readSemaphore, 0, AMOUNT_READERS);

    
    // Initializing of readers
    for (int i = 0; i < AMOUNT_READERS; i++)
    {
        thread readerThread(reader, ref(data), ref(readingDataState), ref(counterReader), ref(counterWriter), ref(AMOUNT_FORCE_CHANGE_AFTER_ROUNDS));
        readerThread.detach();
    }

    // Initializing of writers
    for (int i = 0; i < AMOUNT_WRITERS; i++)
    {
        thread writerThread(writer, ref(data), ref(counterReader), ref(counterWriter), ref(AMOUNT_FORCE_CHANGE_AFTER_ROUNDS));
        writerThread.detach();
    }

//      sem_post(&writeSemaphore);
//        
//        // All readers are allowed to read at the same time
//        for(int i = 0; i < AMOUNT_READERS; i++)
//        {
//            sem_post(&readSemaphore);
//        }
      
    while(true)
    {
//        sem_post(&writeSemaphore);
//        
//        // All readers are allowed to read at the same time
//        for(int i = 0; i < AMOUNT_READERS; i++)
//        {
//            sem_post(&readSemaphore);
//        }
        
        //sleep(1);
    }
    

    return 0;
}

