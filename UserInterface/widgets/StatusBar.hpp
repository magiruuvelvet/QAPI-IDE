#ifndef UI_STATUSBAR_HPP
#define UI_STATUSBAR_HPP

#include <memory>

#include <QWidget>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

class StatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);

    void setStatusText(const QString &text);

private:
    std::unique_ptr<QVBoxLayout> _rootLayout;
    std::unique_ptr<QHBoxLayout> _contentLayout;

    std::shared_ptr<QFrame> _sepLine;
    std::shared_ptr<QLabel> _statusText;
};

#endif // UI_STATUSBAR_HPP
