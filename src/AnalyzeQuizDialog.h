//---------------------------------------------------------------------------
// AnalyzeQuizDialog.h
//
// A dialog for prompting the user for a quiz.
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

#ifndef ANALYZE_QUIZ_DIALOG_H
#define ANALYZE_QUIZ_DIALOG_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>

class QuizEngine;

class AnalyzeQuizDialog : public QDialog
{
  Q_OBJECT
  public:
    AnalyzeQuizDialog (QuizEngine* e, QWidget* parent = 0,
                       const char* name = 0, bool modal = false, WFlags f = 0);

    ~AnalyzeQuizDialog() { }

  public slots:
    void reset();
    void updateStats();
    void addMissed (const QString& word);
    void addIncorrect (const QString& word);
    void clearMissed();
    void clearIncorrect();

  private:
    void setRecall (int correct, int total);
    void setPrecision (int correct, int total);
    QString percentString (int numerator, int denominator) const;

  private:
    QuizEngine*   quizEngine;
    QListView*    missedList;
    QListView*    incorrectList;
    QLabel*       recallLabel;
    QLabel*       precisionLabel;
    QPushButton*  closeButton;
};

#endif // ANALYZE_QUIZ_DIALOG_H

