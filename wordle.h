/*
 * This file is part of the XXX distribution (https://github.com/aksmat/wordle-helper).
 * Copyright (c) 2025 Akshay Mathur.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WORDLE_H
#define WORDLE_H

#include <QMainWindow>
#include "ui_wordle.h"
#include <unordered_set>
#include <string>
#include <vector>

#define WORDLE_LETTER_LENGTH 5

class Wordle : public QMainWindow, Ui::Wordle {
    Q_OBJECT

public:
    Wordle(QWidget *parent = nullptr);
    ~Wordle();

protected:
    void loadWordleList();

    void fill(const char*start,unsigned int currentIndex, std::string &notAllowedLetters);
    bool wordContainsOccouringCharacters(std::string &tword);
    bool wordDoesNotContainCharachtersAtPosition(std::string &tword);
    void checkWordForAdditionalValidity(std::string &tword);

    virtual void resizeEvent(QResizeEvent *) override;

protected:
    std::unordered_set<std::string,std::hash<std::string_view>, std::equal_to<>> set;
    ClickableLabel *labels[WORDLE_LETTER_LENGTH];
    std::vector<std::string> extantWordList, matchWordList;
    std::string occouring, notInPos[WORDLE_LETTER_LENGTH];
    QLineEdit *lettersAtPositions[WORDLE_LETTER_LENGTH];
    unsigned int defaultAppHeight, defaultListX, defaultListY, defaultListWidth, defaultListHeight;

private slots:
    void on_action_Quit_triggered();
    void on_action_Clear_triggered();
    void on_action_Generate_triggered();
};
#endif // WORDLE_H
