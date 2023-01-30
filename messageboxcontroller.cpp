#include "messageboxcontroller.hpp"
#include <QQmlContext>

MessageBoxController::MessageBoxController(QObject *parent)
    : QObject{parent}
    , m_messageText("Hello world")
    , m_visible(false)
{

}

void MessageBoxController::DisplayMessage(const QString &messageText)
{
    setMessageText(messageText);
    setVisible(true);
}

void MessageBoxController::registerProperties(QQmlApplicationEngine &engine, const QString &name)
{
  engine.rootContext()->setContextProperty(name, this);
}

QString MessageBoxController::messageText() const
{
    return m_messageText;
}

void MessageBoxController::setMessageText(const QString &newMessageText)
{
    if (m_messageText == newMessageText)
        return;
    m_messageText = newMessageText;
    emit messageTextChanged();
}

bool MessageBoxController::visible() const
{
    return m_visible;
}

void MessageBoxController::setVisible(bool newVisible)
{
    if (m_visible == newVisible)
        return;
    m_visible = newVisible;
    emit visibleChanged();
}
