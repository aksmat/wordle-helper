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

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    ClickableLabel(QWidget *parent);
    ClickableLabel() = delete;
    void reset();
    inline void setSequence(unsigned int value){ sequence = value;}
    void setNext(ClickableLabel *value);
    int getKeyLower();
signals:
    void keyed(void *);
protected:
    void setStyleSheetLabel();
    void setFocusToNext();
protected:
    virtual void keyPressEvent(class QKeyEvent *key);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
protected:
    bool isKeyPressed;
    int key;
    char letter;
    unsigned int sequence;
    class ClickableLabel *next;
};

#endif // CLICKABLELABEL_H
