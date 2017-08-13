#include "mainwindow.h"
#include "pcapstruct.h"
#include<Winsock2.h>
//#include <arpa/inet.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget = new QWidget();
    this->setCentralWidget(widget);
    this->setWindowTitle(tr("Read pcap"));
    this->setFixedSize(600,200);
    data_source_label = new QLabel(tr("bxq2011hust@qq.com 欢迎反馈 v0.0.2"));
    data_source_label->setStyleSheet("color:rgb(32,69,73);font:14px");

    video_path_label = new QLabel(tr("pcap文件路径:"));
    video_path = new QLineEdit();
    select_video_path = new QPushButton(tr("..."));
    import_video_btn = new QPushButton(tr("开始读取"));
    import_video_btn->setEnabled(false);
    video_path->setEnabled(false);
    video_path->setFocusPolicy(Qt::NoFocus);

    audio_path_label = new QLabel(tr("txt输出路径: "));
    audio_path = new QLineEdit();
    select_audio_path = new QPushButton(tr("..."));
    audio_path->setEnabled(false);
    audio_path->setFocusPolicy(Qt::NoFocus);

    setImportDialogLayout();
    connect(select_video_path, SIGNAL(clicked()), this, SLOT(SelectVideoPath()));
    connect(select_audio_path, SIGNAL(clicked()), this, SLOT(SelectAudioPath()));
    connect(import_video_btn, SIGNAL(clicked()), this, SLOT(readPcapList()));

}

void MainWindow::setImportDialogLayout()
{
    QHBoxLayout *top_layout = new QHBoxLayout();
    top_layout->addStretch();
    top_layout->addWidget(import_video_btn);
    top_layout->addStretch();
    top_layout->addWidget(data_source_label);


    QHBoxLayout *video_path_layout = new QHBoxLayout();
    video_path_layout->addWidget(video_path_label);
    video_path_layout->addWidget(video_path);
    video_path_layout->addWidget(select_video_path);

    QHBoxLayout *audio_path_layout = new QHBoxLayout();
    audio_path_layout->addWidget(audio_path_label);
    audio_path_layout->addWidget(audio_path);
    audio_path_layout->addWidget(select_audio_path);

    QVBoxLayout *import_main_layout = new QVBoxLayout();
    import_main_layout->addStretch();
    import_main_layout->addLayout(video_path_layout);
    import_main_layout->addLayout(audio_path_layout);
    import_main_layout->addLayout(top_layout);
    import_main_layout->addStretch();

    widget->setLayout(import_main_layout);
}

void MainWindow::SelectVideoPath()
{
#ifdef _WIN32
    video_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "d:/",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#else
    video_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#endif
    video_path->setText(video_dir);
    video_import_path=video_path->text();
    import_video_btn->setEnabled(true);
}

void MainWindow::SelectAudioPath()
{
#ifdef _WIN32
    audio_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "d:",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#else
    audio_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#endif
    audio_path->setText(audio_dir);
    audio_import_path=audio_path->text();
}

void MainWindow::readPcapList()
{
    if(audio_import_path.length()==0)
    {
        audio_import_path = video_import_path + QString("/Pcap-Output/");
        QDir outputPath;
        outputPath.mkdir(audio_import_path);
        audio_path->setText(audio_import_path);
    }

    QDir *dir=new QDir(video_import_path);
    QStringList filter;
    filter<<"*.pcap";
    dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    int count = fileInfo->count();
    qDebug()<<"count"<<count<<audio_import_path;
    for(int i = 0;i<count;i++)
    {
        readPcap(fileInfo->at(i).filePath());
    }
    QMessageBox::about(NULL, "提示", "读取完成");
}

void MainWindow::readPcap(QString fileName)
{
    unsigned long long fileseek = 0;
    unsigned long long number = 0;
//    QString name = fileName.left(fileName.length()-4);
    int nameLength = fileName.length()-4-video_import_path.length();
    QString name = fileName.mid(video_import_path.length(), nameLength);
    QFile pcapFile(fileName);
    if (!pcapFile.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("错误"),tr("文件打开失败"));
        return;
    }

    QFile outPutFile(audio_import_path+name+"txt");
    if (!outPutFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("错误"),tr("文件创建失败"));
        return;
    }
    QDateTime dt;
    QString strDate;
    //FramHeader_t frameHeader;//数据帧头
    //IPHeader_t ipHeader;//IP数据报头
    TCPHeader_t tcpHeader;//TCP数据报头
    char m_ckind;
    char m_cLength;
    unsigned int TSval;
    unsigned int TSecr;
    pcapFile.read((char*)&fileHeader,24);
    fileseek+=24;

    while(pcapFile.read((char*)&pkthdr,16))
    {
        fileseek+=16;
        number++;
        pcapFile.seek(fileseek+22);
        //        pcapFile.read((char*)&frameHeader,sizeof(FramHeader_t));
        //        pcapFile.read((char*)&ipHeader,sizeof(IPHeader_t));//20
        pcapFile.read((char*)&tcpHeader,sizeof(TCPHeader_t));//20

        dt = QDateTime::fromTime_t(pkthdr.ts.tv_sec);
        qDebug()<<"编号"<<number<<"caplen"<<pkthdr.caplen;//<<"total"<<ipHeader.TotalLen
        //qDebug()<<"version"<<(ipHeader.Ver_HLen>>4)<<((ipHeader.Ver_HLen<<4)>>4)<<ipHeader.TotalLen;
        strDate = dt.toString("yyyy-MM-dd hh:mm:ss")+QString(" length:%1 \tmicroseconds:%2\t").arg(pkthdr.caplen).arg(pkthdr.ts.tv_usec);

        int test = tcpHeader.HeaderLen>>4;
        //        qDebug()<<"tcpHeaderLen "<<(test)<<"flag"<<tcpHeader.Flags<<"\n";
        test = test*4-20;
        if(test>0)
        {
            pcapFile.read((char*)&m_ckind,1);
            pcapFile.read((char*)&m_cLength,1);
            qDebug()<<(int)m_ckind;
            unsigned long long tcpSeek = fileseek+22+20;
            while((int)m_cLength<test&&(int)m_cLength!=10)
            {
                tcpSeek+=(unsigned int)m_cLength;
                pcapFile.seek(tcpSeek);
                pcapFile.read((char*)&m_ckind,1);
                pcapFile.read((char*)&m_cLength,1);
            }
            if((int)m_ckind == 8)
            {
                pcapFile.read((char*)&TSval,sizeof(TSval));
                pcapFile.read((char*)&TSecr,sizeof(TSecr));
                strDate = strDate + "TSval:"+QString::number(ntohl(TSval),10)+ "\tTSecr:"+QString::number(ntohl(TSecr),10);
            }
        }

        strDate = strDate + "\n";
        outPutFile.write(strDate.toStdString().c_str());
        fileseek+=pkthdr.caplen;
        pcapFile.seek(fileseek);

    }
    pcapFile.close();
    outPutFile.close();
}

MainWindow::~MainWindow()
{

}
