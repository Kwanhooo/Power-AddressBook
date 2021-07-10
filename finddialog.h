#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QTextStream>

class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    QString getFindText();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString findText;

public slots:
    void findClicked();
};

#endif // FINDDIALOG_H
