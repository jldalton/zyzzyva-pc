//---------------------------------------------------------------------------
// MainSettings.cpp
//
// The main settings for the word study application.
//
// Copyright 2005, 2006 Michael W Thelen <mike@pietdepsi.com>.
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

#include "MainSettings.h"
#include "Auxil.h"
#include "Defs.h"
#include <QSettings>

MainSettings* MainSettings::instance = new MainSettings();

const QString SETTINGS_MAIN = "Zyzzyva";
const QString SETTINGS_MAIN_WINDOW_POS = "main_window_pos";
const QString SETTINGS_MAIN_WINDOW_SIZE = "main_window_size";
const QString SETTINGS_IMPORT = "autoimport";
const QString SETTINGS_IMPORT_LEXICON = "autoimport_lexicon";
const QString SETTINGS_IMPORT_FILE = "autoimport_file";
const QString SETTINGS_DISPLAY_WELCOME = "display_welcome";
const QString SETTINGS_FONT_MAIN = "font";
const QString SETTINGS_FONT_WORD_LISTS = "font_word_lists";
const QString SETTINGS_FONT_QUIZ_LABEL = "font_quiz_label";
const QString SETTINGS_FONT_DEFINITIONS = "font_definitions";
const QString SETTINGS_FONT_WORD_INPUT = "font_word_input";
const QString SETTINGS_SORT_BY_LENGTH = "wordlist_sort_by_length";
const QString SETTINGS_GROUP_BY_ALPHAGRAMS = "wordlist_group_by_alphagrams";
const QString SETTINGS_SHOW_HOOKS = "wordlist_show_hooks";
const QString SETTINGS_SHOW_HOOK_PARENTS = "wordlist_show_hook_parents";
const QString SETTINGS_SHOW_DEFINITIONS = "wordlist_show_definitions";
const QString SETTINGS_USE_TILE_THEME = "use_tile_theme";
const QString SETTINGS_TILE_THEME = "tile_theme";
const QString SETTINGS_QUIZ_LETTER_ORDER = "quiz_letter_order";
const QString SETTINGS_QUIZ_BACKGROUND_COLOR = "quiz_background_color";
const QString SETTINGS_QUIZ_USE_FLASHCARD_MODE = "quiz_use_flashcard_mode";
const QString SETTINGS_QUIZ_SHOW_NUM_RESPONSES = "quiz_show_num_responses";
const QString SETTINGS_QUIZ_AUTO_CHECK = "quiz_auto_check";
const QString SETTINGS_QUIZ_AUTO_ADVANCE = "quiz_auto_advance";
const QString SETTINGS_QUIZ_AUTO_END_AFTER_INCORRECT
    = "quiz_auto_end_after_incorrect";
const QString SETTINGS_QUIZ_CYCLE_ANSWERS = "quiz_cycle_answers";
const QString SETTINGS_QUIZ_RECORD_STATS = "quiz_record_stats";
const QString SETTINGS_LETTER_DISTRIBUTION = "letter_distribution";
const QString SETTINGS_JUDGE_SAVE_LOG = "judge_save_log";
const QString SETTINGS_JUDGE_LOG_DIR = "judge_log_dir";
const QString DEFAULT_AUTO_IMPORT_LEXICON = "OWL2+LWL";
const QString DEFAULT_TILE_THEME = "tan-with-border";
const QString DEFAULT_QUIZ_LETTER_ORDER = Defs::QUIZ_LETTERS_ALPHA;
const QRgb    DEFAULT_QUIZ_BACKGROUND_COLOR = qRgb (0, 0, 127);
const QString DEFAULT_LETTER_DISTRIBUTION = "A:9 B:2 C:2 D:4 E:12 F:2 G:3 "
    "H:2 I:9 J:1 K:1 L:4 M:2 N:6 O:8 P:2 Q:1 R:6 S:4 T:6 U:4 V:2 W:2 X:1 "
    "Y:2 Z:1 _:2";

//---------------------------------------------------------------------------
//  readSettings
//
//! Read settings.
//---------------------------------------------------------------------------
void
MainSettings::readSettings()
{
    QSettings settings;
    settings.beginGroup (SETTINGS_MAIN);

    instance->mainWindowPos =
        settings.value (SETTINGS_MAIN_WINDOW_POS, QPoint (50, 50)).toPoint();
    instance->mainWindowSize
        = settings.value (SETTINGS_MAIN_WINDOW_SIZE,
                          QSize (640, 480)).toSize();

    instance->useAutoImport = settings.value (SETTINGS_IMPORT, true).toBool();
    instance->autoImportLexicon
        = settings.value (SETTINGS_IMPORT_LEXICON,
                          DEFAULT_AUTO_IMPORT_LEXICON).toString();

    // Kludge to update the names of the OWL and OWL2 lexicons to have +LWL
    if (instance->autoImportLexicon == "OWL")
        instance->autoImportLexicon = "OWL+LWL";
    else if (instance->autoImportLexicon == "OWL2")
        instance->autoImportLexicon = "OWL2+LWL";

    instance->autoImportFile
        = settings.value (SETTINGS_IMPORT_FILE).toString();

    instance->displayWelcome
        = settings.value (SETTINGS_DISPLAY_WELCOME, true).toBool();

    instance->useTileTheme
        = settings.value (SETTINGS_USE_TILE_THEME, true).toBool();
    instance->tileTheme
        = settings.value (SETTINGS_TILE_THEME, DEFAULT_TILE_THEME).toString();

    instance->quizLetterOrder
        = settings.value (SETTINGS_QUIZ_LETTER_ORDER,
                          DEFAULT_QUIZ_LETTER_ORDER).toString();

    instance->quizBackgroundColor.setRgb (
        settings.value (SETTINGS_QUIZ_BACKGROUND_COLOR,
                        QString::number (DEFAULT_QUIZ_BACKGROUND_COLOR)
                        ).toUInt());

    instance->quizUseFlashcardMode
        = settings.value (SETTINGS_QUIZ_USE_FLASHCARD_MODE, false).toBool();
    instance->quizShowNumResponses
        = settings.value (SETTINGS_QUIZ_SHOW_NUM_RESPONSES, true).toBool();
    instance->quizAutoCheck
        = settings.value (SETTINGS_QUIZ_AUTO_CHECK, true).toBool();
    instance->quizAutoAdvance
        = settings.value (SETTINGS_QUIZ_AUTO_ADVANCE, false).toBool();
    instance->quizAutoEndAfterIncorrect
        = settings.value (SETTINGS_QUIZ_AUTO_END_AFTER_INCORRECT,
                          false).toBool();
    instance->quizCycleAnswers
        = settings.value (SETTINGS_QUIZ_CYCLE_ANSWERS, true).toBool();
    instance->quizRecordStats
        = settings.value (SETTINGS_QUIZ_RECORD_STATS, true).toBool();

    instance->mainFont
        = settings.value (SETTINGS_FONT_MAIN).toString();
    instance->wordListFont
        = settings.value (SETTINGS_FONT_WORD_LISTS).toString();
    instance->quizLabelFont
        = settings.value (SETTINGS_FONT_QUIZ_LABEL).toString();
    instance->wordInputFont
        = settings.value (SETTINGS_FONT_WORD_INPUT).toString();
    instance->definitionFont
        = settings.value (SETTINGS_FONT_DEFINITIONS).toString();


    instance->wordListSortByLength
        = settings.value (SETTINGS_SORT_BY_LENGTH, false).toBool();
    instance->wordListGroupByAnagrams
        = settings.value (SETTINGS_GROUP_BY_ALPHAGRAMS, true).toBool();
    instance->wordListShowHooks
        = settings.value (SETTINGS_SHOW_HOOKS, true).toBool();
    instance->wordListShowHookParents
        = settings.value (SETTINGS_SHOW_HOOK_PARENTS, true).toBool();
    instance->wordListShowDefinitions
        = settings.value (SETTINGS_SHOW_DEFINITIONS, true).toBool();

    instance->letterDistribution
        = settings.value (SETTINGS_LETTER_DISTRIBUTION,
                          DEFAULT_LETTER_DISTRIBUTION).toString();

    instance->judgeSaveLog
        = settings.value (SETTINGS_JUDGE_SAVE_LOG, true).toBool();
    instance->judgeLogDir
        = settings.value (SETTINGS_JUDGE_LOG_DIR,
                          Auxil::getRootDir() +
                          "/data/judge/saved").toString();
    settings.endGroup();
}

//---------------------------------------------------------------------------
//  writeSettings
//
//! Write settings.
//---------------------------------------------------------------------------
void
MainSettings::writeSettings()
{
    QSettings settings;
    settings.beginGroup (SETTINGS_MAIN);
    settings.setValue (SETTINGS_MAIN_WINDOW_POS, instance->mainWindowPos);
    settings.setValue (SETTINGS_MAIN_WINDOW_SIZE, instance->mainWindowSize);

    settings.setValue (SETTINGS_IMPORT, instance->useAutoImport);
    settings.setValue (SETTINGS_IMPORT_LEXICON, instance->autoImportLexicon);
    settings.setValue (SETTINGS_IMPORT_FILE, instance->autoImportFile);
    settings.setValue (SETTINGS_DISPLAY_WELCOME, instance->displayWelcome);
    settings.setValue (SETTINGS_USE_TILE_THEME, instance->useTileTheme);
    settings.setValue (SETTINGS_TILE_THEME, instance->tileTheme);
    settings.setValue (SETTINGS_QUIZ_LETTER_ORDER,
                       instance->quizLetterOrder);
    settings.setValue (SETTINGS_QUIZ_BACKGROUND_COLOR,
                       QString::number (instance->quizBackgroundColor.rgb()));
    settings.setValue (SETTINGS_QUIZ_USE_FLASHCARD_MODE,
                       instance->quizUseFlashcardMode);
    settings.setValue (SETTINGS_QUIZ_SHOW_NUM_RESPONSES,
                       instance->quizShowNumResponses);
    settings.setValue (SETTINGS_QUIZ_AUTO_CHECK, instance->quizAutoCheck);
    settings.setValue (SETTINGS_QUIZ_AUTO_ADVANCE,
                       instance->quizAutoAdvance);
    settings.setValue (SETTINGS_QUIZ_AUTO_END_AFTER_INCORRECT,
                       instance->quizAutoEndAfterIncorrect);
    settings.setValue (SETTINGS_QUIZ_CYCLE_ANSWERS,
                       instance->quizCycleAnswers);
    settings.setValue (SETTINGS_QUIZ_RECORD_STATS,
                       instance->quizRecordStats);
    settings.setValue (SETTINGS_FONT_MAIN, instance->mainFont);
    settings.setValue (SETTINGS_FONT_WORD_LISTS, instance->wordListFont);
    settings.setValue (SETTINGS_FONT_QUIZ_LABEL, instance->quizLabelFont);
    settings.setValue (SETTINGS_FONT_WORD_INPUT, instance->wordInputFont);
    settings.setValue (SETTINGS_FONT_DEFINITIONS, instance->definitionFont);
    settings.setValue (SETTINGS_SORT_BY_LENGTH,
                       instance->wordListSortByLength);
    settings.setValue (SETTINGS_GROUP_BY_ALPHAGRAMS,
                       instance->wordListGroupByAnagrams);
    settings.setValue (SETTINGS_SHOW_HOOKS, instance->wordListShowHooks);
    settings.setValue (SETTINGS_SHOW_HOOK_PARENTS,
                       instance->wordListShowHookParents);
    settings.setValue (SETTINGS_SHOW_DEFINITIONS,
                       instance->wordListShowDefinitions);
    settings.setValue (SETTINGS_LETTER_DISTRIBUTION,
                       instance->letterDistribution);
    settings.setValue (SETTINGS_JUDGE_SAVE_LOG, instance->judgeSaveLog);
    settings.setValue (SETTINGS_JUDGE_LOG_DIR, instance->judgeLogDir);
    settings.endGroup();
}
