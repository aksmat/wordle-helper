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

#include "clickablelabel.h"
#include <QKeyEvent>

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent){
    reset();
    sequence = 0;
    next = nullptr;
}

void ClickableLabel::reset() {
    isKeyPressed = false;
    key = 0;
    letter = ' ';
    this->setText(QString(letter));
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheetLabel();
}

void ClickableLabel::setNext(ClickableLabel *value) {
    next = value;
}

int ClickableLabel::getKeyLower() {
    if (isKeyPressed){
        return ('a' + (key - Qt::Key_A));
    }
    return 0;
}

void ClickableLabel::setStyleSheetLabel() {
    static QString styleSheetsEmpty = "background-color: rgb(254, 255, 213); \
        color: rgb(20, 20, 20);  \
        font: 20pt \"Ubuntu\";";
    static QString styleSheetsLetter = "color: rgb(255, 255, 255);  \
        background-color: rgb(51, 163, 107); \
        font: 20pt \"Ubuntu\";";
    static QString styleSheets = "color: rgb(255, 255, 255);  \
        background-color: rgb(51, 163, 107); \
        font: 20pt \"Ubuntu\";";
    static QString styleSheetInFocus = "border: 2px solid #202020;";
    static QString styleSheetOutOfFocus = "border: 2px solid #4CAF50;";
    if (isKeyPressed){
        this->setStyleSheet(styleSheetsLetter + (hasFocus()?styleSheetInFocus:styleSheetOutOfFocus));
    } else {
        this->setStyleSheet(styleSheetsEmpty + (hasFocus()?styleSheetInFocus:styleSheetOutOfFocus));
    }
}

void ClickableLabel::setFocusToNext() {
    if (next == nullptr)
        return;
    next->setFocus();
}

void ClickableLabel::keyPressEvent(QKeyEvent *event) {
    int pkey = event->key();
    isKeyPressed = false;
    if ((pkey >= Qt::Key_A) && (pkey <= Qt::Key_Z)){
        letter = 'A' + (pkey - Qt::Key_A);
        key = pkey;
        isKeyPressed = true;
        this->setText(QString(letter));
        setFocusToNext();
    } else if ((pkey == Qt::Key_Delete) || (pkey == Qt::Key_Backspace)) {
        reset();
    } else if ((pkey == Qt::Key_Tab) || (pkey == Qt::Key_Space)){
        setFocusToNext();
        reset();
    }
    setStyleSheetLabel();
}

void ClickableLabel::focusInEvent(QFocusEvent *) {
    setStyleSheetLabel();
}

void ClickableLabel::focusOutEvent(QFocusEvent *) {
    setStyleSheetLabel();
}
