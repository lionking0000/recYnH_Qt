#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
//#include <QPushButton>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
class QLabel;
class QCheckBox;
class QPushButton;
class QProgressBar;
class CRobot;
class CReadPairMatrix;
QT_END_NAMESPACE

using namespace std;

class MapViewDialog : public QDialog
{
    Q_OBJECT

public:
    MapViewDialog()
    {
        QDialog *subDialog = new QDialog;
        subDialog->setWindowTitle("Sub Dialog");
        //QPushButton *button = new QPushButton("Push to open new dialog", this);
        //connect(button, SIGNAL(clicked()), subDialog, SLOT(show()));
    }

    explicit MapViewDialog(CRobot *robot, CReadPairMatrix* readPairMatrix, QWidget *parent = 0);
    CRobot* m_robot;
    CReadPairMatrix* m_readPairMatrix;

private:
    QTabWidget *tabWidget;

};


class OptionTab : public QWidget
{
    Q_OBJECT

public:
    explicit OptionTab(CRobot *robot, QWidget *parent = 0);

    CRobot* m_robot;
};


class AlignTab : public QWidget
{
    Q_OBJECT

public:
    explicit AlignTab(CRobot *robot, QWidget *parent = 0);
    QString browse1();  // bait fasta
    QString browse2();  // bait fasta
    QString browse3();  // prey fastq
    QString browse4();  // prey fastq
    QString browse5();  // output file

    int align();
    int option1();
    int updated();

    QLabel m_bait_fasta_Label;
    QLabel m_prey_fasta_Label;
    QLabel m_bait_fastq_Label;
    QLabel m_prey_fastq_Label;
    QLabel m_output_file_Label;

    CRobot* m_robot;
    QPushButton* m_browseButton1;
    QPushButton* m_browseButton2;
    QPushButton* m_browseButton3;
    QPushButton* m_browseButton4;
    QPushButton* m_browseButton5;
    QProgressBar* m_progressBar;

    //-------- options
    QCheckBox m_bait_prey_fasta_Check;



};




class MergeTab : public QWidget
{
    Q_OBJECT

public:
    explicit MergeTab(CRobot *robot, QWidget *parent = 0);
    CRobot* m_robot;

    QLabel m_non_selection_Label;
    QLabel m_selection_Label;
    QLabel m_output_file_Label;

    int merge();
    QString browse1();  // bait fasta
    QString browse2();  // bait fasta
    QString browse3();  // bait fasta

    void createFusionPair( vector<string>& colNames, vector<string>& rowNames, vector<vector<float>>& matrixValue);
};




class AverageTab : public QWidget
{
    Q_OBJECT

public:
    explicit AverageTab(CRobot *robot, QWidget *parent = 0);
    CRobot* m_robot;

    int average();
};




class TabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TabDialog(CRobot *robot, QWidget *parent = 0);
    CRobot* m_robot;

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;


};

#endif
