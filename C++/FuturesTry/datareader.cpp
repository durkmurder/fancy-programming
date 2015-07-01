#include "datareader.h"

#include <thread>
#include <future>
#include <algorithm>
#include <iterator>
#include <QDebug>

DataReader::DataReader(std::string filePath)
{
    m_filePath = std::move(filePath);
}

void DataReader::getAllTasksAsync(const std::function<void (TasksVector)> &callback)
{
    std::thread([=](){
        std::future<TasksVector> fut = std::async( DataReader::readDataFromFile, this );
        fut.wait();
        callback( std::move( fut.get() ) );
    }).detach();
}

TasksVector DataReader::readDataFromFile()
{
    if( !m_fileStream.is_open() )
        m_fileStream.open( m_filePath );

    TasksVector result;
    if( m_fileStream.is_open() )
        std::copy( std::istream_iterator<Task>(m_fileStream), std::istream_iterator<Task>(), std::back_inserter<TasksVector>(result) );

    m_fileStream.close();
    return std::move( result );
}

