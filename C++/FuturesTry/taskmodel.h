#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include <vector>
#include "task.hpp"

class TaskModel : public QAbstractListModel
{
public:
    enum Columns { Owner = 0, Title, Description };
    TaskModel();

    void resetModel( TasksVector tasks );

    // QAbstractItemModel interface
public:
    //virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    TasksVector m_tasks;


};

#endif // TASKMODEL_H
