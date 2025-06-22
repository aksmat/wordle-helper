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

#include "wordle.h"
#include <QFile>
#include <QMessageBox>
#include <QRegularExpressionValidator>

Wordle::Wordle(QWidget *parent) : QMainWindow(parent){
    setupUi(this);
    QLineEdit *lettersAtPositions1[WORDLE_LETTER_LENGTH] = {
        lineEdit_NotPos_1,lineEdit_NotPos_2,lineEdit_NotPos_3,lineEdit_NotPos_4,lineEdit_NotPos_5
    };
    loadWordleList();
    labels[0] = label_L1;
    labels[1] = label_L2;
    labels[2] = label_L3;
    labels[3] = label_L4;
    labels[4] = label_L5;
    for (int i = 0; i < (WORDLE_LETTER_LENGTH-1); i++){
        labels[i]->setNext(labels[i+1]);
        lettersAtPositions[i] = lettersAtPositions1[i];
    }

    lettersAtPositions[WORDLE_LETTER_LENGTH-1] = lettersAtPositions1[WORDLE_LETTER_LENGTH-1];
    lineEdit_NonOccour->setValidator(new QRegularExpressionValidator( QRegularExpression("[A-Za-z]{0,30}"), this ));
    listWidget->clear();

    defaultAppHeight = 600;
    defaultListWidth = 250;
    defaultListHeight= 211;
    defaultListX = 1;
    defaultListY = 310;
}

Wordle::~Wordle() {
}

void Wordle::loadWordleList() {
    QFile wordleList(":/wordle-list.txt");
    if (!wordleList.open(QIODevice::ReadOnly)){
        qDebug()<< "error: " << wordleList.errorString();
    }

    QTextStream stream(&wordleList);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine()) {
        set.insert(line.toStdString());
    };
    wordleList.close();

    QMessageBox::information(this, "Word List", "Wordle Word List is Loaded with " + QString::number(set.size()) + " words!");
    //std::cout<<(set.find("three") != set.end())<<std::endl;
}

void Wordle::fill(const char *start, unsigned int currentIndex, std::string &notAllowedLetters) {
    if (currentIndex >= WORDLE_LETTER_LENGTH)
        return;
    char currentLetter = start[currentIndex];
    std::string curWord  = start;
    if (currentLetter != '.'){
        if (currentIndex == (WORDLE_LETTER_LENGTH-1)){
            checkWordForAdditionalValidity(curWord);
        } else {
            fill(start, currentIndex+1, notAllowedLetters);
        }
        return;
    } else {
        for(int i = 'a'; i < ('z'+1); i++){
            if (notAllowedLetters.find((char)i) != std::string::npos){
                // This is a not allowed letter.
                continue;
            }
            std::string tword= curWord;
            tword[currentIndex] = i;
            if (currentIndex  == (WORDLE_LETTER_LENGTH-1) ){
                checkWordForAdditionalValidity(tword);
            } else {
                fill(tword.c_str(),currentIndex+1, notAllowedLetters);
            }
        }
    }
}

bool Wordle::wordContainsOccouringCharacters(std::string &tword) {
    if (lineEdit_Includes->text().isEmpty())
        return true;
    std::string s = lineEdit_Includes->text().toStdString();
    unsigned int len = s.length();
    for (unsigned int i =0; i < len; i++){
        if (tword.find(s.c_str()[i]) == std::string::npos){
            // Letter does not exist in word, return false;
            return false;
        }
    }
    return true;
}

bool Wordle::wordDoesNotContainCharachtersAtPosition(std::string &tword) {
    const char* word_cstr = tword.c_str();
    for (unsigned int j=0; j<WORDLE_LETTER_LENGTH; j++){
        if (lettersAtPositions[j]->text().isEmpty())
            continue;
        std::string s = lettersAtPositions[j]->text().toStdString();
        const char *sctr = s.c_str();
        for (unsigned int i =0; i < s.length(); i++){
            if (word_cstr[j] == sctr[i]){
                // Letter exists at position. Return false;
                return false;
            }
        }
    }
    return true;
}

void Wordle::checkWordForAdditionalValidity(std::string &tword) {
    if (wordDoesNotContainCharachtersAtPosition(tword) &&
        wordContainsOccouringCharacters(tword)){
        extantWordList.push_back(tword);
    }
}

void Wordle::resizeEvent(QResizeEvent *) {
    unsigned int nHeight = this->height();
    unsigned int hList = (nHeight - defaultAppHeight) + defaultListHeight;

    listWidget->setGeometry(defaultListX, defaultListY,defaultListWidth, hList);
}

void Wordle::on_action_Quit_triggered() {
    close();
}

void Wordle::on_action_Clear_triggered() {
    for(int i =0; i < WORDLE_LETTER_LENGTH; i++){
        labels[i]->reset();
        lettersAtPositions[i]->clear();
    }
    lineEdit_Includes->clear();
    lineEdit_NonOccour->clear();
    listWidget->clear();
    extantWordList.clear();
    matchWordList.clear();
}

void Wordle::on_action_Generate_triggered() {
    unsigned int numLetters = 0;
    unsigned int numNonOccouringLetters = 0;
    char letter;
    std::string matchString, nonOccouring;
    const char *cmstr;
    int i;
    unsigned int row =0;

    listWidget->clear();
    for (i = 0; i < (WORDLE_LETTER_LENGTH); i++){
        letter = labels[i]->getKeyLower();
        if (letter == 0){
            matchString += '.';
        } else {
            matchString += letter;
            numLetters++;
        }
    }
    numNonOccouringLetters = lineEdit_NonOccour->text().size();
    if ((numLetters == 0) && (numNonOccouringLetters < WORDLE_LETTER_LENGTH) && (lineEdit_Includes->text().size() == 0)){
        QMessageBox::information(this,"Insufficient Information","Atleast one letter or 5 known not occouring letters are needed.");
        return;
    }
    nonOccouring = lineEdit_NonOccour->text().toStdString();
    cmstr = matchString.c_str();
    extantWordList.clear();
    matchWordList.clear();

    fill(cmstr,0, nonOccouring);

    for (i = 0; (unsigned int)i < extantWordList.size(); i++){
        if (set.find(extantWordList.at(i)) != set.end()){
            // This word exists in the word list.
            //std::cout<<extantWordList.at(i)<<'\n';
            matchWordList.push_back(extantWordList.at(i));
            QString s = QString::fromStdString(extantWordList.at(i));
            QListWidgetItem *item = new QListWidgetItem(s);
            listWidget->insertItem(row, item);
            row++;
        }
    }

    QMessageBox::information(this, "Search Finished", QString("Total ") + QString::number(row) + QString(" generated,") );
}

