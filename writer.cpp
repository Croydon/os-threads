#include "writer.hpp"


writer::writer(vector<int>& newData, mutex& newWriteMutex) : data(newData), writeMutex(newWriteMutex) {

}


