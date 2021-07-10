#include "finddialog.h"

//防止中文乱码宏
#define z(s) (QString::fromLocal8Bit(s))

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    //设置窗口图标
    QIcon findDialogIcon;
    QPixmap findPixmap(":/images/favicon.ico");
    findDialogIcon.addPixmap(findPixmap);
    this->setWindowIcon(findDialogIcon);

    QLabel *findLabel = new QLabel(tr("输入完整姓名以查找："));

    lineEdit = new QLineEdit;
    findButton = new QPushButton(tr("搜索"));
    findButton->setToolTip("立即从地址簿中搜索");
    findText = "";

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findButton);
    setLayout(layout);
    setWindowTitle(tr("查找联系人"));

    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(findButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void FindDialog::findClicked()
{
    QString text = lineEdit->text();
    if (text.isEmpty())
    {
        QMessageBox::information(this, tr("输入为空"),tr("请输入您要查找的姓名"));
        return;
    }
    else
    {
        findText = text;
        lineEdit->clear();
        hide();
    }
}

QString FindDialog::getFindText()
{
    return findText;
}
