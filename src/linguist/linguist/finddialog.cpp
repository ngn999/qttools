/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Linguist of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*  TRANSLATOR FindDialog

    Choose Edit|Find from the menu bar or press Ctrl+F to pop up the
    Find dialog
*/

#include "finddialog.h"

QT_BEGIN_NAMESPACE

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    statusFilter->addItem(tr("All"), -1);
    statusFilter->addItem(tr("Finished"), TranslatorMessage::Finished);
    statusFilter->addItem(tr("Unfinished"), TranslatorMessage::Unfinished);

    findNxt->setEnabled(false);

    connect(findNxt, &QAbstractButton::clicked,
            this, &FindDialog::emitFindNext);
    connect(useRegExp, &QCheckBox::stateChanged,
            this, &FindDialog::verify);
    connect(led, &QLineEdit::textChanged,
            this, &FindDialog::verify);
    connect(statusFilter, &QComboBox::currentIndexChanged,
            this, &FindDialog::statusFilterChanged);

    led->setFocus();
}

void FindDialog::verify()
{
    bool validRegExp = true;
    if (useRegExp->isChecked() && !led->text().isEmpty()) {
        m_regExp.setPattern(led->text());
        validRegExp = m_regExp.isValid();
    }
    if (validRegExp && m_redText)
        led->setStyleSheet(QStringLiteral("color: auto;"));
    else if (!validRegExp && !m_redText)
        led->setStyleSheet(QStringLiteral("color: red;"));
    m_redText = !validRegExp;
    findNxt->setEnabled(!led->text().isEmpty() && validRegExp);
}

void FindDialog::statusFilterChanged()
{
    int newStateFilter = statusFilter->currentData().toInt();
    if (newStateFilter != -1) {
        if (m_lastStateFilter == -1)
            m_storedSkipObsolete = skipObsolete->isChecked();
        skipObsolete->setEnabled(false);
        skipObsolete->setChecked(true);
    } else {
        skipObsolete->setEnabled(true);
        skipObsolete->setChecked(m_storedSkipObsolete);
    }
    m_lastStateFilter = newStateFilter;
}

void FindDialog::emitFindNext()
{
    DataModel::FindLocation where;
    if (sourceText != 0)
        where =
            DataModel::FindLocation(
                (sourceText->isChecked() ? DataModel::SourceText : 0) |
                (translations->isChecked() ? DataModel::Translations : 0) |
                (comments->isChecked() ? DataModel::Comments : 0));
    else
        where = DataModel::Translations;

    FindOptions options((matchCase->isChecked() ? FindOption::MatchCase : 0) |
                        (ignoreAccelerators->isChecked() ? FindDialog::IgnoreAccelerators : 0) |
                        (skipObsolete->isChecked() ? FindDialog::SkipObsolete : 0) |
                        (useRegExp->isChecked() ? FindDialog::UseRegExp : 0));
    emit findNext(led->text(), where, options, statusFilter->currentData().toInt());
    led->selectAll();
}

void FindDialog::find()
{
    led->setFocus();

    show();
    activateWindow();
    raise();
}

QT_END_NAMESPACE
