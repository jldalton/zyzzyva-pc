//---------------------------------------------------------------------------
// QuizForm.cpp
//
// A form for quizzing the user.
//
// Copyright 2004, 2005 Michael W Thelen <mike@pietdepsi.com>.
//
// This file is part of Zyzzyva.
//
// Zyzzyva is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Zyzzyva is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//---------------------------------------------------------------------------

#include "QuizForm.h"
#include "NewQuizDialog.h"
#include "QuizEngine.h"
#include "WordValidator.h"
#include "ZListViewItem.h"
#include "Defs.h"
#include <qlayout.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qmessagebox.h>

using namespace Defs;

//---------------------------------------------------------------------------
// QuizForm
//
//! Constructor.
//
//! @param e the quiz engine
//! @param parent the parent widget
//! @param name the name of this widget
//! @param f widget flags
//---------------------------------------------------------------------------
QuizForm::QuizForm (QuizEngine* e, QWidget* parent, const char* name,
                    WFlags f)
    : QFrame (parent, name, f), engine (e),
    newQuizDialog (new NewQuizDialog (this, "newQuizDialog", true))
{
    QHBoxLayout* mainHlay = new QHBoxLayout (this, MARGIN, SPACING,
                                             "mainHlay");
    Q_CHECK_PTR (mainHlay);

    QVBoxLayout* mainVlay = new QVBoxLayout (SPACING, "mainVlay");
    Q_CHECK_PTR (mainVlay);
    mainHlay->addLayout (mainVlay);

    QGroupBox* quizGbox = new QGroupBox (this, "quizGbox");
    Q_CHECK_PTR (quizGbox);
    mainVlay->addWidget (quizGbox);

    QHBoxLayout* quizBoxHlay = new QHBoxLayout (quizGbox, MARGIN, SPACING,
                                                "quizBoxHlay");
    Q_CHECK_PTR (quizBoxHlay);
    quizBoxHlay->addStretch (1);
    questionLabel = new QLabel (quizGbox, "label");
    Q_CHECK_PTR (questionLabel);
    quizBoxHlay->addWidget (questionLabel);
    quizBoxHlay->addStretch (1);

    responseList = new QListView (this, "responseList");
    Q_CHECK_PTR (responseList);
    responseList->setResizeMode (QListView::LastColumn);
    responseList->addColumn ("Responses");
    responseList->header()->hide();
    mainVlay->addWidget (responseList);

    // Question status
    QHBoxLayout* qStatusHlay = new QHBoxLayout (SPACING, "questionStatusHlay");
    Q_CHECK_PTR (qStatusHlay);
    mainVlay->addLayout (qStatusHlay);

    questionNumLabel = new QLabel (this, "questionNumLabel");
    Q_CHECK_PTR (questionNumLabel);
    qStatusHlay->addWidget (questionNumLabel);

    responseStatusLabel = new QLabel (this, "responseStatusLabel");
    Q_CHECK_PTR (responseStatusLabel);
    qStatusHlay->addWidget (responseStatusLabel);

    // Stats
    QHBoxLayout* statsHlay = new QHBoxLayout (SPACING, "statsHlay");
    Q_CHECK_PTR (statsHlay);
    mainVlay->addLayout (statsHlay);

    recallLabel = new QLabel (this, "recallLabel");
    Q_CHECK_PTR (recallLabel);
    statsHlay->addWidget (recallLabel);

    precisionLabel = new QLabel (this, "precisionLabel");
    Q_CHECK_PTR (precisionLabel);
    statsHlay->addWidget (precisionLabel);

    QHBoxLayout* totalStatsHlay = new QHBoxLayout (SPACING, "totalStatsHlay");
    Q_CHECK_PTR (totalStatsHlay);
    mainVlay->addLayout (totalStatsHlay);

    totalRecallLabel = new QLabel (this, "totalRecallLabel");
    Q_CHECK_PTR (totalRecallLabel);
    totalStatsHlay->addWidget (totalRecallLabel);

    totalPrecisionLabel = new QLabel (this, "totalPrecisionLabel");
    Q_CHECK_PTR (totalPrecisionLabel);
    totalStatsHlay->addWidget (totalPrecisionLabel);

    // Input line
    inputLine = new QLineEdit (this, "inputLine");
    Q_CHECK_PTR (inputLine);
    WordValidator* validator = new WordValidator (inputLine);
    Q_CHECK_PTR (validator);
    inputLine->setValidator (validator);
    connect (inputLine, SIGNAL (returnPressed()), SLOT (responseEntered()));
    mainVlay->addWidget (inputLine);

    // Buttons
    QHBoxLayout* buttonHlay = new QHBoxLayout (SPACING, "buttonHlay");
    Q_CHECK_PTR (buttonHlay);
    mainVlay->addLayout (buttonHlay);

    QPushButton* newQuizButton = new QPushButton ("New &Quiz...", this,
                                                  "newQuizButton");
    Q_CHECK_PTR (newQuizButton);
    newQuizButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (newQuizButton, SIGNAL (clicked()), SLOT (newQuizClicked()));
    buttonHlay->addWidget (newQuizButton);

    nextQuestionButton = new QPushButton ("&Next", this,
                                          "nextQuestionButton");
    Q_CHECK_PTR (nextQuestionButton);
    nextQuestionButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (nextQuestionButton, SIGNAL (clicked()),
             SLOT (nextQuestionClicked()));
    buttonHlay->addWidget (nextQuestionButton);

    checkResponseButton = new QPushButton ("&Check", this,
                                           "checkResponseButton");
    Q_CHECK_PTR (checkResponseButton);
    checkResponseButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect (checkResponseButton, SIGNAL (clicked()),
             SLOT (checkResponseClicked()));
    buttonHlay->addWidget (checkResponseButton);
}

//---------------------------------------------------------------------------
// responseEntered
//
//! Called when a response is entered into the input line.
//---------------------------------------------------------------------------
void
QuizForm::responseEntered()
{
    QString response = inputLine->text();
    if (response.isEmpty())
        return;

    QuizEngine::ResponseStatus status = engine->respond (response);
    QString statusStr = "";

    if (status == QuizEngine::Correct) {
        ZListViewItem* item = new ZListViewItem (responseList, response);
        responseList->setSelected (item, true);
        responseList->ensureItemVisible (item);
        item->setTextColor (Qt::blue);
        statusStr = "<font color=\"blue\">Correct</font>";
    }
    else if (status == QuizEngine::Incorrect)
        statusStr = "<font color=\"red\">Incorrect</font>";
    else if (status == QuizEngine::Duplicate)
        statusStr = "<font color=\"purple\">Duplicate</font>";
    inputLine->clear();

    // Update the stats if the stats are already shown
    if (!recallLabel->text().isEmpty())
        updateStats();

    // Update the response status label
    responseStatusLabel->setText(response + " : " + statusStr);
}

//---------------------------------------------------------------------------
// newQuizClicked
//
//! Called when the New Quiz button is clicked.
//---------------------------------------------------------------------------
void
QuizForm::newQuizClicked()
{
    int code = newQuizDialog->exec();
    if (code != QDialog::Accepted)
        return;

    QString input = newQuizDialog->getQuizString();
    MatchType type = newQuizDialog->getQuizType();
    bool alphagrams = newQuizDialog->getQuizAlphagrams();
    bool random = newQuizDialog->getQuizRandomOrder();
    engine->newQuiz (input, type, alphagrams, random);
    updateForm (false);
}

//---------------------------------------------------------------------------
// nextQuestionClicked
//
//! Called when the New Question button is clicked.
//---------------------------------------------------------------------------
void
QuizForm::nextQuestionClicked()
{
    if (!engine->nextQuestion())
        QMessageBox::warning (this, "Error getting next question",
                              "Error getting next question.");

    updateForm (false);
}

//---------------------------------------------------------------------------
// checkResponseClicked
//
//! Called when the Check Responses button is clicked.
//---------------------------------------------------------------------------
void
QuizForm::checkResponseClicked()
{
    updateStats();
    inputLine->setEnabled (false);
    checkResponseButton->setEnabled (false);

    QStringList unanswered = engine->getMissed();
    QStringList::iterator it;
    for (it = unanswered.begin(); it != unanswered.end(); ++it) {
        ZListViewItem* item = new ZListViewItem (responseList, *it);
        item->setTextColor (Qt::magenta);
    }
}

//---------------------------------------------------------------------------
// updateStats
//
//! Update the form, including setting the question label, clearing the
//! response list, and enabling and disabling form elements as
//! appropriate.
//---------------------------------------------------------------------------
void
QuizForm::updateForm (bool showStats)
{
    if (showStats)
        updateStats();
    else
        clearStats();
    setQuestionNum (engine->getQuestionIndex() + 1, engine->numQuestions());
    questionLabel->setText (engine->getQuestion());
    responseList->clear();
    responseStatusLabel->setText ("");
    inputLine->setEnabled (true);
    nextQuestionButton->setEnabled ((engine->numQuestions() > 0) &&
                                    !engine->onLastQuestion());
    checkResponseButton->setEnabled (true);

    inputLine->setFocus();
}

//---------------------------------------------------------------------------
// updateStats
//
//! Update the recall and precision statistics.
//---------------------------------------------------------------------------
void
QuizForm::updateStats()
{
    int correct = engine->getQuestionCorrect();
    setRecall (correct, engine->getQuestionTotal());
    setPrecision (correct, correct + engine->getQuestionIncorrect());
    int quizCorrect = engine->getQuizCorrect();
    setTotalRecall (quizCorrect, engine->getQuizTotal());
    setTotalPrecision (quizCorrect, quizCorrect + engine->getQuizIncorrect());
}

//---------------------------------------------------------------------------
// clearStats
//
//! Clear the recall and precision statistics.
//---------------------------------------------------------------------------
void
QuizForm::clearStats()
{
    recallLabel->setText ("");
    precisionLabel->setText ("");
    totalRecallLabel->setText ("");
    totalPrecisionLabel->setText ("");
}

//---------------------------------------------------------------------------
// clearQuestionNum
//
//! Clear the question number label.
//---------------------------------------------------------------------------
void
QuizForm::clearQuestionNum()
{
    questionNumLabel->setText ("");
}

//---------------------------------------------------------------------------
// setQuestionNum
//
//! Set the current question number and the total number of questions.
//
//! @param num the current question number
//! @param total the total number of questions
//---------------------------------------------------------------------------
void
QuizForm::setQuestionNum (int num, int total)
{
    questionNumLabel->setText ("Question " + QString::number (num)
                               + " of " + QString::number (total));
}

//---------------------------------------------------------------------------
// setTotalRecall
//
//! Set the quiz recall numbers (correct user responses divided by total
//! correct responses).
//
//! @param correct the number of correct responses
//! @param total the total number of correct answers
//---------------------------------------------------------------------------
void
QuizForm::setTotalRecall (int correct, int total)
{
    totalRecallLabel->setText ("Total Recall: " +
                               percentString (correct, total));
}

//---------------------------------------------------------------------------
// setTotalPrecision
//
//! Set the quiz precision numbers (correct user responses divided by total
//! user responses).
//
//! @param correct the number of correct responses
//! @param total the total number of user responses
//---------------------------------------------------------------------------
void
QuizForm::setTotalPrecision (int correct, int total)
{
    totalPrecisionLabel->setText ("Total Precision: "
                                  + percentString (correct, total));
}

//---------------------------------------------------------------------------
// setRecall
//
//! Set the recall numbers (correct user responses divided by total correct
//! responses).
//
//! @param correct the number of correct responses
//! @param total the total number of correct answers
//---------------------------------------------------------------------------
void
QuizForm::setRecall (int correct, int total)
{
    recallLabel->setText ("Recall: " + percentString (correct, total));
}

//---------------------------------------------------------------------------
// setPrecision
//
//! Set the precision numbers (correct user responses divided by total user
//! responses).
//
//! @param correct the number of correct responses
//! @param total the total number of user responses
//---------------------------------------------------------------------------
void
QuizForm::setPrecision (int correct, int total)
{
    precisionLabel->setText ("Precision: " + percentString (correct, total));
}

//---------------------------------------------------------------------------
// percentString
//
//! Create a string to represent a percentage to be displayed.
//
//! @param numerator the numerator
//! @param denominator the denominator
//---------------------------------------------------------------------------
QString
QuizForm::percentString (int numerator, int denominator) const
{
    double pct = denominator ? (numerator * 100.0) / denominator : 0;
    return QString::number (numerator) + " / " + QString::number (denominator)
        + " (" + QString::number (pct, 'f', 1) + "%)";
}
