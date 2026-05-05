#ifndef CLICKABLECARD_H
#define CLICKABLECARD_H

#include <QFrame>
#include <QMouseEvent>

class ClickableCard : public QFrame {
    Q_OBJECT

public:
    explicit ClickableCard(QWidget *parent = nullptr);

signals:
    // Your custom signal that the Viewer will listen for
    void clicked(); 

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CLICKABLECARD_H
