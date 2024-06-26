//-
// Copyright (c) 2019 The NomadBSD Project. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
 
#pragma once
#include <QDebug>
#include <QWizard>
#include <QWizardPage>
#include <QString>
#include <QProcess>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QProgressBar>

#include "password.h"

#define PATH_ETC_PASSWD "/etc/passwd"

class Wizard : public QWizard
{
	Q_OBJECT

public:
	Wizard(QWidget *parent = 0);
	void accept() override;
	void reject() override;
	QIcon loadIcon(const char *name, ...);
	
	static void errAndOut(QString message)
	{
		QMessageBox msgBox;
		msgBox.setWindowModality(Qt::WindowModal);
		msgBox.setText(tr("Fatal error"));
		msgBox.setWindowTitle(tr("Error"));
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setWindowIcon(msgBox.iconPixmap());
		message.append(tr("\n\nPlease send a bug report to the "    \
				  "NomadBSD mailing list.\n\nWe are sorry " \
				  "for the inconvenience\n"));
		msgBox.setInformativeText(message);
		msgBox.addButton(tr("Exit"), QMessageBox::ActionRole);
		msgBox.exec();
		std::exit(1);
	}

	static void cancelMenu()
	{
		QMessageBox msgBox;
		msgBox.setWindowModality(Qt::WindowModal);
		msgBox.setText(tr("Sure you want to quit?"));
		msgBox.setWindowTitle(tr("Cancel"));
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setWindowIcon(msgBox.iconPixmap());
		QPushButton *leave = msgBox.addButton(tr("Quit"),
		    QMessageBox::ActionRole);
		msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
		msgBox.exec();
		if (msgBox.clickedButton() == leave)
			std::exit(0);
	}
};

//////////////////////////////////////////////////////////////////////////////
//
// Username window
//
//////////////////////////////////////////////////////////////////////////////
class UsernamePage : public QWizardPage
{
	Q_OBJECT

public:
	UsernamePage(QWidget *parent = 0);
	bool isComplete() const;
public slots:
	void nameChanged(const QString &name);
	void usernameChanged(const QString &username);
	bool validateUsername(const QString &username) const;
	void readUsernames(void);
private:
	QLabel	    *status;
	QLineEdit   *namele;
	QLineEdit   *usernamele;
	QList<QString> usernames;
};

//////////////////////////////////////////////////////////////////////////////
//
// Language/locale page
//
//////////////////////////////////////////////////////////////////////////////
class LocalePage : public QWizardPage
{
	Q_OBJECT

public:
	LocalePage(QWidget *parent = 0);
public slots:
	void localeSelected(int row);
private:
	QListWidget *localels;
};

//////////////////////////////////////////////////////////////////////////////
//
// Password page
//
//////////////////////////////////////////////////////////////////////////////
class PasswordPage : public QWizardPage
{
	Q_OBJECT
	
public:
	PasswordPage(QWidget *parent = 0);
	bool isComplete() const;
public slots:
	void passwordChanged();
protected:
	void initializePage();
private:
	QLabel	       *title;
	PasswordWidget *pass;
};

//////////////////////////////////////////////////////////////////////////////
//
// Default application page
//
//////////////////////////////////////////////////////////////////////////////
class ProgramsPage : public QWizardPage
{
	Q_OBJECT

public:
	ProgramsPage(QWidget *parent = 0);
public slots:
	void selectionChanged(int);
private:
	QString getBoxVal(QComboBox *box);
protected:
	void initializePage(void);
private:
	static const int ncats = 4;
	QLabel	  *catLabel[ncats];
	QLabel	  *intro;
	QLabel	  *title;
	QComboBox *browsers;
	QComboBox *shells;
	QComboBox *editors;
	QComboBox *emailClients;
};

//////////////////////////////////////////////////////////////////////////////
//
// Summary page
//
//////////////////////////////////////////////////////////////////////////////
class SummaryPage : public QWizardPage
{
	Q_OBJECT
public:
	SummaryPage(QWidget *parent = 0);
protected:
	void initializePage();
private:
	static const int nkeys = 7;
	QLabel *val[nkeys];
	QLabel *key[nkeys];
	QLabel *title;
};

class CommitPage : public QWizardPage
{
	Q_OBJECT
public:
	CommitPage(QWidget *parent = 0);
	bool isComplete() const;
public slots:
	void readCmdOutput();
	void readError();
	void catchError(QProcess::ProcessError);
	void cleanup(int, QProcess::ExitStatus);
protected:
	void initializePage();
private:
	QLabel	     *commandMsg;
	QLabel	     *statusMsg;
	QString	     commandMsgBuf;
	QString	     errorMsgBuf;
	QProcess     proc;
	QProgressBar *progBar;
};
