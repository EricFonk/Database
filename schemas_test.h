//
//  schemas_test.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef schemas_test_h
#define schemas_test_h

#include <QtWidgets>
#include <QtGui>
#include <QString>

class SchemasTest : public QWidget
{
    Q_OBJECT
public:
    SchemasTest();
    void initData(QStringList);
public:
    QStringListModel *model;
    QListView *listView;
    QStringList allDB;
    private slots:
    void createDatabase();
    void dropDatabase();
};

#endif /* schemas_test_h */
