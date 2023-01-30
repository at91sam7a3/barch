#ifndef MESSAGEBOXCONTROLLER_HPP
#define MESSAGEBOXCONTROLLER_HPP
#include <QQmlApplicationEngine>
#include <QObject>

class MessageBoxController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString messageText READ messageText WRITE setMessageText NOTIFY messageTextChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit MessageBoxController(QObject *parent = nullptr);
    void DisplayMessage(const QString& messageText);
    void registerProperties(QQmlApplicationEngine& engine, const QString& name);

    QString messageText() const;
    void setMessageText(const QString &newMessageText);

    bool visible() const;
    void setVisible(bool newVisible);

signals:

    void messageTextChanged();
    void visibleChanged();

private:
    QString m_messageText;
    bool m_visible;
};

#endif // MESSAGEBOXCONTROLLER_HPP
