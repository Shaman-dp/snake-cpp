#include "snake.h"

Snake::Snake() {
    snakeBody_.append(QPoint(50, 50));
    direction_ = Qt::Key_Right;
}

void Snake::move() {

}

void Snake::appendBodyPoint(Qt::Key d) {
    QPoint tail = snakeBody_.last(); // копируем последний сегмент змейки

    // обновляем координаты нового сегмента
    if (d == Qt::Key_Up){
        tail.setY(tail.y() - 10);
    }
    if (d == Qt::Key_Down){
        tail.setY(tail.y() + 10);
    }
    if (d == Qt::Key_Left){
        tail.setX(tail.x() - 10);
    }
    if (d == Qt::Key_Right){
        tail.setX(tail.x() + 10);
    }

    snakeBody_.push_back(tail); // добавляем новый сегмент
}

QList<QPoint> Snake::getBody(){
    return snakeBody_;
}
