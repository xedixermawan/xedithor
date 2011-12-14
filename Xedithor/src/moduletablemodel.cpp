
#include "moduletablemodel.h"

int RowData::mLast_ID_Module=0;

ModuleTableModel::ModuleTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    numberRow = 1;
    numberCol = 6;
    for(int i=0;i<numberRow;i++)
    {
            RowData* dat = new RowData(numberCol);
            m_gridData2.insert(i, dat);
    }
    m_Headers.push_back("Index ");
    m_Headers.push_back("ID");
    m_Headers.push_back("X");
    m_Headers.push_back("Y");
    m_Headers.push_back("Width");
    m_Headers.push_back("Height");

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()) , this, SLOT(timerHit()));
    timer->start();
}

int ModuleTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_gridData2.count();
}

int ModuleTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return numberCol;
}

QVariant ModuleTableModel::data(const QModelIndex &index, int role) const
{

    //std::cout<<"row: "<<index.row()<<std::endl;
    if (role == Qt::DisplayRole)
    {
        RowData* dat =m_gridData2.at(index.row());
        int col = index.column();
        return dat->getData(col);
        //return m_gridData[index.row()][index.column()];

        //return m_gridData2;//QTime::currentTime().toString();

    }
    else if(role == Qt::EditRole)
    {
        RowData* dat =m_gridData2.at(index.row());
        int col = index.column();
        return dat->getData(col);


    }

    return QVariant();
}

bool ModuleTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        qDebug("test");
        //save value from editor to member m_gridData
        RowData* dat =m_gridData2.at(index.row());
        dat->setData(index.column(), value.toString());
        //m_gridData[index.row()][index.column()] = value.toString();

        //--debug only
        //for presentation purposes only: build and emit a joined string
        QString result;
        for(int row= 0; row < m_gridData2.count(); row++)
        {
            for(int col= 0; col < numberCol; col++)
            {
                dat =m_gridData2.at(row);
                result +=dat->getData(col)+ " ";
            }
            result +="\n";
        }
        qDebug(result.toStdString().c_str());
        //--end debug only
        emit editCompleted( result );
    }

    return true;
}

Qt::ItemFlags ModuleTableModel::flags(const QModelIndex & /*index*/) const
{
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}


bool ModuleTableModel::insertRows ( int row, int count, const QModelIndex & parent )
{
    beginInsertRows(parent, row, row+count-1);
    for (int n = 0; n < count; ++n) {
        RowData* dat = new RowData(numberCol);
        m_gridData2.insert(row, dat);
    }
    endInsertRows();

    return true;
}

bool ModuleTableModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    beginRemoveRows(parent, row,row+count-1);

    if(row <= m_gridData2.count()-1)
    {
        for (int n = 0; n < count; ++n) {
                delete m_gridData2.at(row);
                m_gridData2.removeAt(row);
            }
    }
    endRemoveRows ();
    return true;
}

QVariant ModuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();
    if(orientation == Qt::Vertical)
       return QVariant(section);
    if (m_Headers.size() ==0)
        return QVariant();
    return m_Headers.at(section);

}

bool ModuleTableModel::cloneRow(int row)
{
    RowData* dat= m_gridData2.at(row);
    insertRow(row+1);
    RowData* newRowData =m_gridData2.at(row+1);
    delete newRowData;
    newRowData = new RowData(numberCol);
    for(int i=0;i<numberCol;i++)
    {
        if(i>1) //don't clone idx & ID
        {
            newRowData->setData(i,dat->getData(i));
        }
    }

    return true;
}

bool ModuleTableModel::swapRow(int row1,int row2)
{
   //TODO : swap data

   return true;
}

RowData* ModuleTableModel::getDatainRow(int iRow)
{
    return m_gridData2.at(iRow);
}

void ModuleTableModel::timerHit()
{
/*
    //we identify the top left cell
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex botright = createIndex(m_gridData2.count(),5);
    //emit a signal to make the view reread identified data
    emit dataChanged(topLeft, botright);
    */
}

void ModuleTableModel::refresh()
{
     QModelIndex topLeft  = createIndex(0,0);
     QModelIndex botright = createIndex(m_gridData2.count(),numberCol-1);

     emit dataChanged(topLeft, botright);
}
