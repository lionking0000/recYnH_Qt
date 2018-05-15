#include <QtWidgets>

#include "tabdialog.h"
#include "robot.h"
#include "fusionpair.h"

//! [0]
TabDialog::TabDialog(CRobot *robot, QWidget *parent)
    : QDialog(parent)
{
    //QFileInfo fileInfo(fileName);
    m_robot = robot;

    tabWidget = new QTabWidget;
    tabWidget->addTab(new OptionTab(robot), tr("Options"));
    tabWidget->addTab(new AlignTab(robot), tr("Align"));
    tabWidget->addTab(new MergeTab(robot), tr("Merge"));
    tabWidget->addTab(new AverageTab(robot), tr("Average"));


    // Ok and Cencel box
    //buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    //                                 | QDialogButtonBox::Cancel);
    //connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    //connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    //mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    tabWidget->setCurrentIndex(1);

    setWindowTitle(tr("recYnH"));
}


int AlignTab::option1()
{
    if (this->m_bait_prey_fasta_Check.checkState() == Qt::Unchecked){
        this->m_browseButton2->setEnabled( true );
    }
    if (this->m_bait_prey_fasta_Check.checkState() == Qt::Checked){
        this->m_browseButton2->setEnabled( false );
    }
}

int AlignTab::updated(){
    m_progressBar->setValue(0);
    return 0;
}

int AlignTab::align()
{
    int argc = 7;
    //char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/users/lserrano/jyang/work/Mireia/2015-05-28/40_40.upper.fasta",
    //                 "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read1.fastq.gz",
    //                 "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read2.fastq.gz",
    //                 "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-07-07.A2.txt", "125"};

    std::string str_bait_fasta = this->m_bait_fasta_Label.text().toStdString();
    std::string str_bait_fastq = this->m_bait_fastq_Label.text().toStdString();
    std::string str_prey_fastq = this->m_prey_fastq_Label.text().toStdString();
    std::string str_output_file = this->m_output_file_Label.text().toStdString();

    char* cstr_bait_fasta = (char*) str_bait_fasta.c_str();
    char* cstr_bait_fastq = (char*) str_bait_fastq.c_str();
    char* cstr_prey_fastq = (char*) str_prey_fastq.c_str();
    char* cstr_output_file = (char*) str_output_file.c_str();

    //cstr_bait_fasta = "/Volumes/users/lserrano/jyang/work/Mireia/2015-05-28/40_40.upper.fasta";
    //cstr_bait_fastq = "/Volumes//users/smaurer/sequencing_data/Mireia_Garriga/2015-07-07/PCR_TS_A2_10494_GGAGCC_read1.fastq.gz";
    //cstr_prey_fastq = "/Volumes//users/smaurer/sequencing_data/Mireia_Garriga/2015-07-07/PCR_TS_A2_10494_GGAGCC_read2.fastq.gz";
    //cstr_output_file = "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-07-07.A2.txt"
    char* argv[] = { "./kmer", "FusionPairSearch", cstr_bait_fasta,
                     cstr_bait_fastq,
                     cstr_prey_fastq,
                     cstr_output_file, "125"};


    FusionPair* _fp = new FusionPair( argc, argv );
    _fp->bShortRun = m_robot->_run._config.fp_bShortRun;
    _fp->Search_Normal();
    //_run->_pfp = _fp;

    for( int i = 0; i <= 100; i++ ){
        m_progressBar->setValue(i);
        //this->update();
    }
    //progressBar->setValue(0);
    return 0;
}

QString AlignTab::browse1()
{
    //QString filepath =
    //    QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath()));

    //if (!directory.isEmpty()) {
    //    if (directoryComboBox->findText(directory) == -1)
    //        directoryComboBox->addItem(directory);
    //    directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    //}

    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_bait_fasta_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty()){
            m_bait_fasta_Label.setText(fileName);
            if (this->m_bait_prey_fasta_Check.checkState() == Qt::Checked){
                m_prey_fasta_Label.setText(fileName);
            }
        }
    this->updated();

    return fileName;
}

QString AlignTab::browse2()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_prey_fasta_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty()){
            m_prey_fasta_Label.setText(fileName);
            if (this->m_bait_prey_fasta_Check.checkState() == Qt::Checked){
                m_bait_fasta_Label.setText(fileName);
            }
        }
        this->updated();

    return fileName;
}

QString AlignTab::browse3()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_bait_fastq_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty())
            m_bait_fastq_Label.setText(fileName);

        this->updated();        return fileName;
}


QString AlignTab::browse4()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_prey_fastq_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty())
            m_prey_fastq_Label.setText(fileName);
    this->updated();

    return fileName;
}

QString AlignTab::browse5()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.txt";

    QString fileName = QFileDialog::getSaveFileName(this,
                                    tr("Select PPI Pair Output File)"),
                                    this->m_output_file_Label.text(),
                                    tr("PPI Pair Output File (*.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty())
            m_output_file_Label.setText(fileName);
    this->updated();

    return fileName;
}

OptionTab::OptionTab(CRobot *robot, QWidget *parent)
    : QWidget(parent)
{
    m_robot = robot;
    QLabel *descriptionLabel = new QLabel(tr("General options"));

    QGroupBox *alignOptGroup = new QGroupBox(tr("Options for align"));
    QGroupBox *mergeOptGroup = new QGroupBox(tr("Options for merge"));
    QGroupBox *averageOptGroup = new QGroupBox(tr("Options for average"));


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( alignOptGroup );
    mainLayout->addWidget( mergeOptGroup );
    mainLayout->addWidget( averageOptGroup );
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}


AlignTab::AlignTab(CRobot *robot, QWidget *parent)
    : QWidget(parent)
{
    m_robot = robot;
    QLabel *descriptionLabel = new QLabel(tr("Align the FASTQ sequencing files into bait and prey sequences to generate interaction matrix"));

    //---------------------------------------------------------------------

    QGroupBox *fastaGroup = new QGroupBox(tr("Fasta files"));

    m_bait_fasta_Label.setText("Fasta file for Bait");
    m_bait_fasta_Label.setText("/Volumes/users/lserrano/jyang/work/Mireia/data/roth2016_control_set_plus_control.fa");

    m_browseButton1 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton1, &QAbstractButton::clicked, this, &AlignTab::browse1);

    m_prey_fasta_Label.setText("Fasta file for Prey");
    m_prey_fasta_Label.setText("/Volumes/users/lserrano/jyang/work/Mireia/data/roth2016_control_set_plus_control.fa");

    //QPushButton *browseButton2 = new QPushButton(tr("&Browse..."), this);
    m_browseButton2 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton2, &QAbstractButton::clicked, this, &AlignTab::browse2);

    //connect(browseButton2, &QAbstractButton::clicked, this, &AlignTab::changed);

    m_browseButton2->setEnabled( false );

    QGridLayout *fastaLayout = new QGridLayout;
    fastaLayout->addWidget(&m_bait_fasta_Label, 0, 0, 1 ,4);
    fastaLayout->addWidget(m_browseButton1, 0, 4,1,1);
    fastaLayout->addWidget(&m_prey_fasta_Label, 1, 0, 1 ,4);
    fastaLayout->addWidget(m_browseButton2, 1, 4,1,1);
    fastaGroup->setLayout(fastaLayout);

    //---------------------------------------------------------------------

    QGroupBox *seqGroup = new QGroupBox(tr("Sequencing files"));

    QLabel *seqFile1 = new QLabel("Fastq file1");

    m_bait_fastq_Label.setText("Fastq file for Read 1 (Baits)");
    m_bait_fastq_Label.setText("/Volumes/users/smaurer/sequencing_data/Mireia_Garriga/2017-03-03_MiSeq/18689_S1_L001_R1_001.fastq.gz"); // /Volumes/users/lserrano/jyang/work/Mireia/2017-03-03_MiSeq/S1_R1.fastq.gz


    m_browseButton3 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton3, &QAbstractButton::clicked, this, &AlignTab::browse3);

    m_prey_fastq_Label.setText("Fastq file for Read 2 (Preys)");
    m_prey_fastq_Label.setText("/Volumes/users/smaurer/sequencing_data/Mireia_Garriga/2017-03-03_MiSeq/18689_S1_L001_R2_001.fastq.gz"); // /Volumes/users/lserrano/jyang/work/Mireia/2017-03-03_MiSeq/S1_R2.fastq.gz

    m_browseButton4 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton4, &QAbstractButton::clicked, this, &AlignTab::browse4);

    QGridLayout *seqLayout = new QGridLayout;

    seqLayout->addWidget(&m_bait_fastq_Label, 0, 0, 1 ,4);
    seqLayout->addWidget(m_browseButton3, 0, 4,1,1);
    seqLayout->addWidget(&m_prey_fastq_Label, 1, 0, 1 ,4);
    seqLayout->addWidget(m_browseButton4, 1, 4,1,1);
    seqGroup->setLayout(seqLayout);

    //---------------------------------------------------------------------

    QGroupBox *outputGroup = new QGroupBox(tr("Output File"));

    //QLabel *m_output_file_Label = new QLabel("Output filepath");
    //QLabel *m_output_file_Label = new QLabel("/Users/jyang/2017-03-03_MiSeq.S1.txt");
    m_output_file_Label.setText("/Users/jyang/2017-03-03_MiSeq.S1.txt");

    QPushButton* browseButton5 = new QPushButton(tr("&Browse..."), this);
    connect(browseButton5, &QAbstractButton::clicked, this, &AlignTab::browse5);

    QGridLayout *outputLayout = new QGridLayout;

    outputLayout->addWidget(&m_output_file_Label,0,0,1,4);
    outputLayout->addWidget(browseButton5,0,4,1,1);
    outputGroup->setLayout(outputLayout);

    //---------------------------------------------------------------------

    QGroupBox *optionGroup = new QGroupBox(tr("Options"));

    QLabel *optionLabel1 = new QLabel("Use the bait and prey fasta as same file");

    m_bait_prey_fasta_Check.setChecked( true );
    connect(&m_bait_prey_fasta_Check, &QAbstractButton::clicked, this, &AlignTab::option1);

    QGridLayout *optionLayout = new QGridLayout;

    //optionGroup->addWidget(seqFile1);
    optionLayout->addWidget(optionLabel1,0,0,1,4);
    optionLayout->addWidget(&m_bait_prey_fasta_Check,0,4,1,1);
    optionGroup->setLayout(optionLayout);

    //---------------------------------------------------------------------

    QGroupBox *runGroup = new QGroupBox(tr("Run"));
    m_progressBar = new QProgressBar(this);
    m_browseButton5 = new QPushButton(tr("&Align..."), this);
    connect(m_browseButton5, &QAbstractButton::clicked, this, &AlignTab::align);
    QGridLayout *runLayout = new QGridLayout;
    runLayout->addWidget(m_progressBar,0,0,1,4);
    runLayout->addWidget(m_browseButton5,0,4,1,1);
    runGroup->setLayout(runLayout);

    //---------------------------------------------------------------------

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(fastaGroup);
    mainLayout->addWidget(seqGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(optionGroup);
    mainLayout->addWidget(runGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    /*

    QLabel *fileNameLabel = new QLabel(tr("File Name:"));
    QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());

    QLabel *pathLabel = new QLabel(tr("Path:"));
    QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());
    pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *sizeLabel = new QLabel(tr("Size:"));
    qlonglong size = fileInfo.size()/1024;
    QLabel *sizeValueLabel = new QLabel(tr("%1 K").arg(size));
    sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *lastReadLabel = new QLabel(tr("Last Read:"));
    QLabel *lastReadValueLabel = new QLabel(fileInfo.lastRead().toString());
    lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QLabel *lastModLabel = new QLabel(tr("Last Modified:"));
    QLabel *lastModValueLabel = new QLabel(fileInfo.lastModified().toString());
    lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(fileNameLabel);
    mainLayout->addWidget(fileNameEdit);
    mainLayout->addWidget(pathLabel);
    mainLayout->addWidget(pathValueLabel);
    mainLayout->addWidget(sizeLabel);
    mainLayout->addWidget(sizeValueLabel);
    mainLayout->addWidget(lastReadLabel);
    mainLayout->addWidget(lastReadValueLabel);
    mainLayout->addWidget(lastModLabel);
    mainLayout->addWidget(lastModValueLabel);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
    */
}


int MergeTab::merge()
{
    return 0;


    int argc = 7;
    char* argv[] = { "./kmer", "FusionPairSearch", "/Volumes/users/lserrano/jyang/work/Mireia/2015-05-28/40_40.upper.fasta",
                     "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read1.fastq.gz",
                     "/Volumes/users/lserrano/jyang/work/Mireia/src_backup_20170622/data/2015-07-07/PCR_TS_A2_10494_GGAGCC_read2.fastq.gz",
                     "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-07-07.A2.txt", "125"};
    /*
    std::string str_bait_fasta = this->m_bait_fasta_Label.text().toStdString();
    std::string str_bait_fastq = this->m_bait_fastq_Label.text().toStdString();
    std::string str_prey_fastq = this->m_prey_fastq_Label.text().toStdString();

    char* cstr_bait_fasta = (char*) str_bait_fasta.c_str();
    char* cstr_bait_fastq = (char*) str_bait_fastq.c_str();
    char* cstr_prey_fastq = (char*) str_prey_fastq.c_str();

    cstr_bait_fasta = "/Volumes/users/lserrano/jyang/work/Mireia/2015-05-28/40_40.upper.fasta";
    cstr_bait_fastq = "/Volumes//users/smaurer/sequencing_data/Mireia_Garriga/2015-07-07/PCR_TS_A2_10494_GGAGCC_read1.fastq.gz";
    cstr_prey_fastq = "/Volumes//users/smaurer/sequencing_data/Mireia_Garriga/2015-07-07/PCR_TS_A2_10494_GGAGCC_read2.fastq.gz";

    char* argv[] = { "./kmer", "FusionPairSearch", cstr_bait_fasta,
                     cstr_bait_fastq,
                     cstr_prey_fastq,
                     "/Users/jyang/Dropbox_CRG/Code/temp/heatmap/output/2015-07-07.A2.txt", "125"};

    */
    FusionPair* _fp = new FusionPair( argc, argv );
    _fp->bShortRun = m_robot->_run._config.fp_bShortRun;
    _fp->Search();

    for( int i = 0; i <= 100; i++ ){
        //m_progressBar->setValue(i);
        //this->update();
    }
    return 0;
}

QString MergeTab::browse1()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_non_selection_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty()){
            m_non_selection_Label.setText(fileName);
        }
    //this->updated();

    return fileName;
}

QString MergeTab::browse2()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_selection_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty()){
            m_selection_Label.setText(fileName);
        }
    //this->updated();

    return fileName;
}

QString MergeTab::browse3()
{
    QFileDialog::Options options;
    QString selectedFilter = "*.fa";

    QString fileName = QFileDialog::getSaveFileName(this,
                                    tr("Select a Blaster File)"),
                                    this->m_output_file_Label.text(),
                                    tr("Fasta Files (*.fa, *.fasta, *.txt)"),
                                    &selectedFilter,
                                    options);
        if (!fileName.isEmpty()){
            m_output_file_Label.setText(fileName);
        }
    //this->updated();

    return fileName;
}

MergeTab::MergeTab(CRobot *robot, QWidget *parent)
    : QWidget(parent)
{
    m_robot = robot;
    QLabel *descriptionLabel = new QLabel(tr("Merge two interaction matries to generate an interaction score matrix"));

    QGroupBox *non_selectionGroup = new QGroupBox(tr("Non-Selection"));
    m_non_selection_Label.setText("/Users/jyang/2017-03-03_MiSeq.S1.txt");
    QPushButton* m_browseButton1 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton1, &QAbstractButton::clicked, this, &MergeTab::browse1);
    QGridLayout *non_selectionLayout = new QGridLayout;
    non_selectionLayout->addWidget(&m_non_selection_Label, 0, 0, 1 ,4);
    non_selectionLayout->addWidget(m_browseButton1, 0, 4,1,1);
    non_selectionGroup->setLayout(non_selectionLayout);

    //---------------------------------------------------------------------

    QGroupBox *selectionGroup = new QGroupBox(tr("Selection"));
    m_selection_Label.setText("/Users/jyang/2017-03-03_MiSeq.S2.txt");
    QPushButton* m_browseButton2 = new QPushButton(tr("&Browse..."), this);
    connect(m_browseButton2, &QAbstractButton::clicked, this, &MergeTab::browse2);
    QGridLayout *selectionLayout = new QGridLayout;
    selectionLayout->addWidget(&m_selection_Label, 0, 0, 1 ,4);
    selectionLayout->addWidget(m_browseButton2, 0, 4,1,1);
    selectionGroup->setLayout(selectionLayout);

    //---------------------------------------------------------------------

    QGroupBox *runGroup = new QGroupBox(tr("Run"));
    QProgressBar* m_progressBar = new QProgressBar(this);
    QPushButton* m_browseButton5 = new QPushButton(tr("&Merge..."), this);
    connect(m_browseButton5, &QAbstractButton::clicked, this, &MergeTab::merge);
    QGridLayout *runLayout = new QGridLayout;
    runLayout->addWidget(m_progressBar,0,0,1,4);
    runLayout->addWidget(m_browseButton5,0,4,1,1);
    runGroup->setLayout(runLayout);

    //---------------------------------------------------------------------

    QGroupBox *outputGroup = new QGroupBox(tr("Output File"));

    //QLabel *m_output_file_Label = new QLabel("Output filepath");
    //QLabel *m_output_file_Label = new QLabel("/Users/jyang/2017-03-03_MiSeq.S1.txt");
    m_output_file_Label.setText("/Users/jyang/2017-03-03_MiSeq.IS1.txt");

    QPushButton* browseButton5 = new QPushButton(tr("&Browse..."), this);
    connect(browseButton5, &QAbstractButton::clicked, this, &MergeTab::browse3);

    QGridLayout *outputLayout = new QGridLayout;

    outputLayout->addWidget(&m_output_file_Label,0,0,1,4);
    outputLayout->addWidget(browseButton5,0,4,1,1);
    outputGroup->setLayout(outputLayout);

    //---------------------------------------------------------------------

    QGroupBox *optionGroup = new QGroupBox(tr("Options"));

    QLabel *optionLabel1 = new QLabel("Use the bait and prey fasta as same file");

    //m_bait_prey_fasta_Check.setChecked( true );
    //connect(&m_bait_prey_fasta_Check, &QAbstractButton::clicked, this, &AlignTab::option1);

    QGridLayout *optionLayout = new QGridLayout;

    //optionGroup->addWidget(seqFile1);
    optionLayout->addWidget(optionLabel1,0,0,1,4);
    //optionLayout->addWidget(&m_bait_prey_fasta_Check,0,4,1,1);
    optionGroup->setLayout(optionLayout);

    //---------------------------------------------------------------------
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(non_selectionGroup);
    mainLayout->addWidget(selectionGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(optionGroup);
    mainLayout->addWidget(runGroup);
    setLayout(mainLayout);
}



AverageTab::AverageTab(CRobot *robot, QWidget *parent)
    : QWidget(parent)
{
    m_robot = robot;
    QLabel *descriptionLabel = new QLabel(tr("Average several interaction score matrices"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(descriptionLabel);
    setLayout(layout);
}


