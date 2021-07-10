#include "addressbook.h"

void AddressBook::initUI()
{
    QPixmap iconPixmap(":images/favicon.ico");
    QIcon mainWindowIcon;
    mainWindowIcon.addPixmap(iconPixmap);
    this->setWindowIcon(mainWindowIcon);

    this->label_name = new QLabel(tr("姓名："));
    this->label_address = new QLabel(tr("地址："));

    this->lineEdit_name = new QLineEdit;
    this->textEdit_address = new QTextEdit;

    this->lineEdit_name->setReadOnly(true);
    this->textEdit_address->setReadOnly(true);

    this->btn_pre = new QPushButton(tr("前一个"));
    this->btn_next = new QPushButton(tr("后一个"));
    this->btn_add = new QPushButton(tr("添加"));
    this->btn_submit = new QPushButton(tr("提交"));
    this->btn_cancel = new QPushButton(tr("取消"));
    this->btn_remove = new QPushButton(tr("删除"));
    this->btn_edit = new QPushButton(tr("编辑"));
    this->btn_find = new QPushButton(tr("查找"));
    this->btn_load = new QPushButton(tr("载入..."));
    this->btn_save = new QPushButton(tr("保存..."));
    this->btn_export = new QPushButton(tr("导出为CSV"));

    this->btn_pre->setToolTip("查看上一个联系人");
    this->btn_next->setToolTip("查看下一个联系人");
    this->btn_add->setToolTip("添加联系人");
    this->btn_submit->setToolTip("保存更改");
    this->btn_cancel->setToolTip("取消添加操作");
    this->btn_remove->setToolTip("删除这个联系人");
    this->btn_edit->setToolTip("编辑这个联系人信息");
    this->btn_find->setToolTip("从保存的地址簿中查找");
    this->btn_load->setToolTip("从配置文件中读取先前的地址簿");
    this->btn_save->setToolTip("将目前的所有信息保存至配置文件");
    this->btn_export->setToolTip("将这个联系人导出为标准的CSV");

    dialog = new FindDialog();

    this->btn_submit->hide();
    this->btn_cancel->hide();

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(label_name,0,0);
    mainLayout->addWidget(lineEdit_name,0,1);
    mainLayout->addWidget(label_address,1,0,Qt::AlignTop);
    mainLayout->addWidget(textEdit_address,1,1);

    QHBoxLayout *HLay_pre_next=new QHBoxLayout;
    HLay_pre_next->addWidget(btn_pre);
    HLay_pre_next->addWidget(btn_next);

    QVBoxLayout *VLay_right = new QVBoxLayout;
    VLay_right->addWidget(btn_add);
    VLay_right->addWidget(btn_remove);
    VLay_right->addWidget(btn_edit);

    VLay_right->addWidget(btn_submit);
    VLay_right->addWidget(btn_cancel);
    VLay_right->addWidget(btn_find);
    VLay_right->addWidget(btn_load);
    VLay_right->addWidget(btn_save);
    VLay_right->addWidget(btn_export);

    mainLayout->addLayout(HLay_pre_next,2,1);
    mainLayout->addLayout(VLay_right,1,2,Qt::AlignTop);

    this->btn_edit->setEnabled(false);
    this->btn_remove->setEnabled(false);
    this->btn_pre->setEnabled(false);
    this->btn_next->setEnabled(false);
    this->btn_export->setEnabled(false);
    this->btn_save->setEnabled(false);
    this->btn_find->setEnabled(false);

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(15);
    this->setFont(font);

    setLayout(mainLayout);
}

void AddressBook::initSignalSlots()
{
    connect(btn_pre,SIGNAL(clicked()),this,SLOT(btn_pre_clicked()));
    connect(btn_next,SIGNAL(clicked()),this,SLOT(btn_next_clicked()));
    connect(btn_add,SIGNAL(clicked()),this,SLOT(btn_add_clicked()));
    connect(btn_cancel,SIGNAL(clicked()),this,SLOT(btn_cancel_clicked()));
    connect(btn_submit,SIGNAL(clicked()),this,SLOT(btn_submit_clicked()));
    connect(btn_edit,SIGNAL(clicked()),this,SLOT(btn_edit_clicked()));
    connect(btn_remove,SIGNAL(clicked()),this,SLOT(btn_remove_clicked()));
    connect(btn_find,SIGNAL(clicked()),this,SLOT(btn_find_clicked()));
    connect(btn_save,SIGNAL(clicked()),this,SLOT(btn_save_clicked()));
    connect(btn_load,SIGNAL(clicked()),this,SLOT(btn_load_clicked()));
    connect(btn_export,SIGNAL(clicked()),this,SLOT(btn_export_clicked()));
}

void AddressBook::updateInterface(Mode mode)
{
    currentMode = mode;
    if(currentMode==EditingMode||currentMode==AddingMode){
        lineEdit_name->setReadOnly(false);
        lineEdit_name->setFocus(Qt::OtherFocusReason);
        textEdit_address->setReadOnly(false);
        btn_add->setEnabled(false);
        btn_edit->setEnabled(false);
        btn_remove->setEnabled(false);
        this->btn_submit->show();
        this->btn_cancel->show();
    }

    if(currentMode==NavigationMode){
        if (info.isEmpty()) {
            lineEdit_name->clear();
            textEdit_address->clear();
        }
        lineEdit_name->setReadOnly(true);
        textEdit_address->setReadOnly(true);
        btn_add->setEnabled(true);
        int number = info.size();
        btn_edit->setEnabled(number >= 1);
        btn_remove->setEnabled(number >= 1);
        btn_next->setEnabled(number >= 1);
        btn_pre->setEnabled(number >=1 );
        btn_submit->hide();
        btn_cancel->hide();
        btn_find->setEnabled(true);
        btn_export->setEnabled(true);
        btn_save->setEnabled(true);
    }
}

void AddressBook::btn_add_clicked()
{
    this->updateInterface(AddingMode);

    this->oldName = this->lineEdit_name->text();
    this->oldAddress = this->textEdit_address->toPlainText();

    this->lineEdit_name->clear();
    this->textEdit_address->clear();

    lineEdit_name->setFocus(Qt::OtherFocusReason);
}

void AddressBook::btn_submit_clicked()
{
    QString newName = this->lineEdit_name->text();
    QString newAddress = this->textEdit_address->toPlainText();
    if(newName == ""|| newAddress == "")
    {
        QMessageBox::information(this, tr("表单部分可能为空！"),tr("请完整键入姓名以及地址！"));
        return;
    }

    if (currentMode == AddingMode)
    {
        if(!this->info.contains(newName))
        {
            info.insert(newName,newAddress);
            QMessageBox::information(this, tr("添加成功啦~"),tr("\"%1\" 已经成功保存！").arg(newName));
        }
        else
        {
            QMessageBox::information(this, tr("添加失败！"),tr("\"%1\" 在你的地址簿中已经存在！").arg(newName));
            return;
        }
    }

    else if(currentMode == EditingMode)
    {
        if (oldName != newName)
        {
            if (!info.contains(newName))
            {
                QMessageBox::information(this, tr("编辑成功~"),
                                         tr("\"%1\" 已经成功编辑至地址簿中！").arg(oldName));
                info.remove(oldName);
                info.insert(newName, newAddress);
            }
            else
            {
                QMessageBox::information(this, tr("编辑失败！"),tr("\"%1\" 在你的地址簿中已存在！").arg(newName));
                return;
            }
        }
        else if (oldAddress != newAddress)
        {
            QMessageBox::information(this, tr("编辑成功~"),tr("\"%1\" 已经成功编辑至地址簿中！").arg(newName));
            info[newName] = newAddress;
        }
    }
    updateInterface(NavigationMode);

    this->lineEdit_name->setText("等待您执行操作...");
    this->textEdit_address->clear();

}

void AddressBook::btn_cancel_clicked()
{
    if(this->info.isEmpty())
    {
        this->lineEdit_name->setReadOnly(true);
        this->textEdit_address->setReadOnly(true);
        this->btn_add->setEnabled(true);
        this->btn_edit->setEnabled(false);
        this->btn_remove->setEnabled(false);
        this->btn_find->setEnabled(false);
        this->btn_export->setEnabled(false);
        this->btn_save->setEnabled(false);
        this->btn_load->setEnabled(true);
        this->btn_submit->hide();
        this->btn_cancel->hide();
        this->lineEdit_name->setText(this->oldName);
        this->textEdit_address->setText(this->oldAddress);
        return;
    }

    updateInterface(NavigationMode);
    this->lineEdit_name->setText(this->oldName);
    this->textEdit_address->setText(this->oldAddress);
}

void AddressBook::btn_next_clicked()
{
    QString name = lineEdit_name->text();
    QMap<QString, QString>::iterator i = info.find(name);
    if (i != info.end())
        i++;
    if (i == info.end())
        i = info.begin();
    lineEdit_name->setText(i.key());
    textEdit_address->setText(i.value());
}

void AddressBook::btn_pre_clicked()
{
    QString name = lineEdit_name->text();
    QMap<QString, QString>::iterator i = info.find(name);
    if (i == info.begin())
        i = info.end();
    i--;
    lineEdit_name->setText(i.key());
    textEdit_address->setText(i.value());
}

void AddressBook::btn_edit_clicked()
{
    if(this->lineEdit_name->text()=="等待您执行操作...")
        QMessageBox::information(this,tr("错误"),tr("非联系人页面不可编辑！"));
    oldName = lineEdit_name->text();
    oldAddress = textEdit_address->toPlainText();

    updateInterface(EditingMode);
}

void AddressBook::btn_remove_clicked()
{
    QString name = lineEdit_name->text();
    QString address = textEdit_address->toPlainText();
    if (info.contains(name))
    {
        int button = QMessageBox::question(this,tr("确定删除"),tr("您确定要删除 \"%1\"吗?").arg(name),QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            this->btn_pre_clicked();
            info.remove(name);
            QMessageBox::information(this, tr("已成功删除~"),tr("\"%1\" 已经从您的地址簿中删除！").arg(name));
        }
    }
    updateInterface(NavigationMode);
}

void AddressBook::btn_find_clicked()
{
    dialog->show();
    if (dialog->exec() == QDialog::Accepted) {
        QString contactName = dialog->getFindText();
        if (info.contains(contactName))
        {
            lineEdit_name->setText(contactName);
            textEdit_address->setText(info.value(contactName));
        }
        else
        {
            QMessageBox::information(this, tr("未找到匹配结果"),tr("\"%1\" 不在您的地址簿内！").arg(contactName));
            return;
        }
    }
    updateInterface(NavigationMode);
}


void AddressBook::btn_export_clicked()
{
    QString name = lineEdit_name->text();
    QString address = textEdit_address->toPlainText();
    if(name=="等待您执行操作...")
    {
        QMessageBox::information(this,tr("错误"),tr("非联系人页面不可导出！"));
        return;
    }

    if(name == "")
    {
        QMessageBox::information(this,tr("错误"),tr("空页面不可导出！"));
        return;
    }

    QString firstName;
    QString lastName;
    QStringList nameList;

    int index = name.indexOf(" ");

    if (index != -1)
    {
        nameList = name.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        firstName = nameList.first();
        lastName = nameList.last();
    }
    else
    {
        firstName = name;
        lastName = "";
    }

    QString fileName = QFileDialog::getSaveFileName(this,tr("导出联系人"), "",tr("vCard Files (*.vcf);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("无法打开文件"),file.errorString());
        return;
    }
    QTextStream out(&file);
    out << "BEGIN:VCARD" << "\n";
    out << "VERSION:2.1" << "\n";
    out << "N:" << lastName << ";" << firstName << "\n";
    if (!nameList.isEmpty())
        out << "FN:" << nameList.join(" ") << "\n";
    else
        out << "FN:" << firstName << "\n";
    address.replace(";", "\\;", Qt::CaseInsensitive);
    address.replace("\n", ";", Qt::CaseInsensitive);
    address.replace(",", " ", Qt::CaseInsensitive);
    out << "ADR;HOME:;" << address << "\n";
    out << "END:VCARD" << "\n";
    QMessageBox::information(this, tr("导出成功"),
                             tr("\"%1\" 已成功导出为 vCard.").arg(name));
}

void AddressBook::btn_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存至..."), "",tr("Address Book (*.abk);;All Files (*)"));
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("无法打开该文件！"),file.errorString());
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_12);
    out << this->info;
    QMessageBox::information(this,tr("保存成功"),tr("已保存至 %1").arg(fileName));
    file.close();
}

void AddressBook::btn_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("载入文件..."),"",tr("Address Book (*.abk);;All Files (*)"));
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,tr("无法打开该文件！"),file.errorString());
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_12);
    in>>this->info;
    file.close();
    QMessageBox::information(this,tr("载入成功"),tr("%1 已成功导入！").arg(fileName));
    updateInterface(NavigationMode);
}

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Super AddressBook");
    initUI();//设置UI界面布局
    initSignalSlots();//连接信号槽
}

AddressBook::~AddressBook(){}
