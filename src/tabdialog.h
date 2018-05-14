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
QT_END_NAMESPACE

//! [0]
class AlignTab : public QWidget
{
    Q_OBJECT

public:
    explicit AlignTab(CRobot *robot, QWidget *parent = 0);
    QString browse1();  // bait fasta
    QString browse2();  // bait fasta
    QString browse3();  // prey fastq
    QString browse4();  // prey fastq

    int align();
    int option1();
    int updated();

    QLabel m_bait_fasta_Label;
    QLabel m_prey_fasta_Label;
    QLabel m_bait_fastq_Label;
    QLabel m_prey_fastq_Label;

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
//! [0]


//! [1]
class MergeTab : public QWidget
{
    Q_OBJECT

public:
    explicit MergeTab(CRobot *robot, QWidget *parent = 0);
    CRobot* m_robot;

    int merge();
};
//! [1]


//! [2]
class AverageTab : public QWidget
{
    Q_OBJECT

public:
    explicit AverageTab(CRobot *robot, QWidget *parent = 0);
    CRobot* m_robot;
};
//! [2]


//! [3]
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
//! [3]

#endif
