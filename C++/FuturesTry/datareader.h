#ifndef DATAREADER_H
#define DATAREADER_H

#include <string>
#include <functional>
#include <task.hpp>
#include <fstream>

// TODO: Implement an interface for reading/writing operations. AbstractDataReader.
// Implement derived classes: FileDataReader(for offline work), DbDataReader( for online work )
class DataReader
{
public:
    DataReader(std::string filePath);
    void getAllTasksAsync( const std::function<void(TasksVector)> &callback );

private:
    TasksVector readDataFromFile();

private:
    std::string m_filePath;
    std::ifstream m_fileStream;
};

#endif // DATAREADER_H
