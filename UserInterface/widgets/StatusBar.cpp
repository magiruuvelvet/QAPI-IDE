#include "StatusBar.hpp"

StatusBar::StatusBar(QWidget *parent)
    : QWidget(parent)
{
    this->_rootLayout = std::make_unique<QVBoxLayout>();
    this->_rootLayout->setMargin(0);
    this->_rootLayout->setContentsMargins(0,0,0,0);
    this->_rootLayout->setSpacing(0);

    this->_contentLayout = std::make_unique<QHBoxLayout>();
    this->_contentLayout->setMargin(4);
    this->_contentLayout->setContentsMargins(12, 5, 12, 5);
    this->_contentLayout->setSpacing(5);

    // separator line
    this->_sepLine = std::make_shared<QFrame>(this);
    this->_sepLine->setFrameShape(QFrame::HLine);
    this->_sepLine->setFrameShadow(QFrame::Plain);
    this->_sepLine->setLineWidth(1);
    this->_sepLine->setFixedHeight(1);

    // status info label
    this->_statusText = std::make_shared<QLabel>(this);
    this->_statusText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // create layout
    this->_contentLayout->addWidget(this->_statusText.get());
    this->_contentLayout->addSpacerItem(new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    this->_rootLayout->addWidget(this->_sepLine.get());
    this->_rootLayout->addLayout(this->_contentLayout.get());

    this->setLayout(this->_rootLayout.get());
}

void StatusBar::setStatusText(const QString &text)
{
    this->_statusText->setText(text);
}
