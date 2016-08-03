//
//  mylistview.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef mylistview_h
#define mylistview_h

#include <QtGui>
#include <QtWidgets>
#include <QString>
class MyListView : public QWidget
{
    Q_OBJECT
public:
    MyListView();
    void initData(QString);
    
private:
    QStringListModel *model;
    QListView *listView;
    
    private slots:
    void insertData();
    void deleteData();
    void showData();
};

#endif /* mylistview_h */
