#ifndef WRITER_HPP
#define WRITER_HPP

#include <vector>
#include <mutex>

using namespace std;

class writer {
public:
    writer(vector<int>& newData, mutex& newWriteMutex);

private:
    vector<int>& data;
    mutex& writeMutex;
};

#endif /* WRITER_HPP */

