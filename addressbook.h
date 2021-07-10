#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "finddialog.h"
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget *parent = nullptr);
    ~AddressBook();

private:
    QLabel *label_name;
    QLabel *label_address;

    QLineEdit *lineEdit_name;

    QTextEdit *textEdit_address;

    QPushButton *btn_pre;
    QPushButton *btn_next;
    QPushButton *btn_add;
    QPushButton *btn_submit;
    QPushButton *btn_cancel;
    QPushButton *btn_remove;
    QPushButton *btn_edit;
    QPushButton *btn_find;
    QPushButton *btn_load;
    QPushButton *btn_save;
    QPushButton *btn_export;

    void    initUI();
    void    initSignalSlots();

    QMap<QString, QString> info;
    QString oldName;
    QString oldAddress;

    enum Mode { NavigationMode, AddingMode, EditingMode };
    void updateInterface(Mode mode);
    Mode currentMode;

    FindDialog *dialog;

public slots:
    void btn_pre_clicked();
    void btn_next_clicked();
    void btn_add_clicked();
    void btn_submit_clicked();
    void btn_cancel_clicked();
    void btn_edit_clicked();
    void btn_remove_clicked();
    void btn_find_clicked();
    void btn_export_clicked();
    void btn_load_clicked();
    void btn_save_clicked();
};

#endif // ADDRESSBOOK_H
