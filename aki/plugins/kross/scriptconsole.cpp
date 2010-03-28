/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scriptconsole.h"
#include <KComboBox>
#include <KDebug>
#include <KLocale>
#include <KMessageBox>
#include <Kross/Action>
#include <Kross/ActionCollection>
#include <Kross/Manager>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/View>
#include <QGridLayout>
#include <QLabel>
#include <KPushButton>

class ScriptConsolePrivate
{
public:
    ScriptConsolePrivate(ScriptConsole *qq)
        : q(qq),
        script(0),
        editor(0),
        document(0),
        view(0),
        interpreters(0)
    {
    }

    bool initKate()
    {
        editor = KTextEditor::EditorChooser::editor();
        if (!editor) {
            KMessageBox::error(0, i18n(""), i18n(""));
            return false;
        }

        document = editor->createDocument(0);
        document->setEncoding("UTF-8");

        view = qobject_cast<KTextEditor::View*>(document->createView(q));

        return true;
    }

    void interpretersActivated(const QString &interpreter)
    {
        if (interpreter == "python") {
            document->setHighlightingMode("python");
        } else if (interpreter == "ruby") {
            document->setHighlightingMode("ruby");
        } else if (interpreter == "qtscript" || interpreter == "javascript") {
            document->setHighlightingMode("javascript");
        }
    }

    void executeButtonClicked()
    {
        if (script) {
            return;
        }

        kDebug() << "Executing script from console";
        script = new Kross::Action(0, "Example");
        script->setCode(document->text().toUtf8());
        kDebug() << "Script: " << document->text().toUtf8();
        script->setInterpreter(interpreters->currentText());
        kDebug() << "Interpreter: " << interpreters->currentText();
        Kross::Manager::self().actionCollection()->addAction(script);

        script->trigger();
        q->button(KDialog::User1)->setDisabled(true);
        q->button(KDialog::User2)->setEnabled(true);

        if (script->hadError()) {
            KMessageBox::error(0, i18n("Script has errors:\nLine Number: %1\nMessage: %2\nTrace: %3",
                                       script->errorLineNo(), script->errorMessage(), script->errorTrace()));
            stopButtonClicked();
        }
    }

    void stopButtonClicked()
    {
        kDebug() << "Removing script from console";
        Kross::Manager::self().actionCollection()->removeAction(script);
        kDebug() << "Deleting script";
        delete script;
        script = 0;

        q->button(KDialog::User2)->setDisabled(true);
        q->button(KDialog::User1)->setEnabled(true);
    }

    ScriptConsole *q;
    Kross::Action *script;
    KTextEditor::Editor *editor;
    KTextEditor::Document *document;
    KTextEditor::View *view;
    KComboBox *interpreters;
}; // End of class ScriptConsolePrivate.

ScriptConsole::ScriptConsole(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new ScriptConsolePrivate(this));

    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    d->initKate();
    layout->addWidget(d->view, 0, 0, 1, 2);

    QLabel *label = new QLabel(i18n("Interpreters:"));
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    layout->addWidget(label, 1, 0, 1, 1); 

    d->interpreters = new KComboBox(this);
    d->interpreters->addItems(Kross::Manager::self().interpreters());
    d->interpretersActivated(d->interpreters->currentText());

    connect(d->interpreters, SIGNAL(activated(QString)),
            SLOT(interpretersActivated(QString)));
    layout->addWidget(d->interpreters, 1, 1, 1, 1);

    widget->setLayout(layout);
    setMainWidget(widget);

    setButtons(KDialog::User1 | KDialog::User2 | KDialog::Ok);
    setButtonText(KDialog::User1, i18n("Execute Script"));
    setButtonText(KDialog::User2, i18n("Stop Script"));
    setButtonIcon(KDialog::User1, KIcon("dialog-ok"));
    setButtonIcon(KDialog::User2, KIcon("dialog-cancel"));

    connect(this, SIGNAL(user1Clicked()),
            SLOT(executeButtonClicked()));
    connect(this, SIGNAL(user2Clicked()),
            SLOT(stopButtonClicked()));

    button(KDialog::User2)->setDisabled(true);

    resize(800, 600);
    setWindowTitle("Aki Script Editor");
}

ScriptConsole::~ScriptConsole()
{
    delete d->document;
}

#include "scriptconsole.moc"
