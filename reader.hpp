#ifndef READER_HPP
#define READER_HPP

#include <vector>

using namespace std;

class reader {
public:
    reader(vector<int>& newData);
    
private:
    vector<int>& data;
};

#endif /* READER_HPP */

