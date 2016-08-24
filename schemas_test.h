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
    void InitData(QStringList);
public:
    QStringListModel *model;
    QListView *listView;
    QStringList allDB;
    private slots:
    void CreateDatabase();
    void DropDatabase();
};

#endif /* schemas_test_h */
