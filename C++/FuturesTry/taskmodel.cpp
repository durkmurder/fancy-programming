#include "taskmodel.h"

#include <QVariant>

TaskModel::TaskModel()
{

}

void TaskModel::resetModel(TasksVector tasks)
{
    beginResetModel();
    m_tasks = std::move( tasks );
    endResetModel();
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tasks.size();
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if( row >= m_tasks.size() )
        return QVariant();

    if( role != Qt::DisplayRole )
        return QVariant();

    const Columns column = static_cast<Columns>( index.column() );
    const Task *task = &m_tasks[row];
    switch(column)
    {
        case Owner:
            return QVariant( task->getOwner().c_str() ); break;
        case Title:
            return QVariant( task->getTitle().c_str() ); break;
        case Description:
            return QVariant( task->getDescription().c_str() ); break;
        default:
            return QVariant(); break;
    }
}


QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    if( orientation == Qt::Vertical )
        return section + 1;

    const Columns column = static_cast<Columns>( section );
    switch(column)
    {
        case Owner:
            return QVariant( "Owner" ); break;
        case Title:
            return QVariant( "Title" ); break;
        case Description:
            return QVariant( "Description" ); break;
        default:
            return QVariant(); break;
    }
}
