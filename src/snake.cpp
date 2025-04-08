#include "snake.h"

Snake::Snake() {
    snakeBody_.append(QPoint(70, 50));
    snakeBody_.append(QPoint(60, 50));
    snakeBody_.append(QPoint(50, 50));
    direction_ = Qt::Key_Right;
}

void Snake::move() {
    QPoint head = snakeBody_.first(); // копируем первый сегмент змейки

    // обновляем координаты нового сегмента
    if (direction_ == Qt::Key_Up){
        head.ry() -= 10; // "head.ry() -= 10" тоже что и head.setY(head.y() - 10), только короче
    }
    if (direction_ == Qt::Key_Down){
        head.ry() += 10;
    }
    if (direction_ == Qt::Key_Left){
        head.rx() -= 10;
    }
    if (direction_ == Qt::Key_Right){
        head.rx() += 10;
    }

    snakeBody_.prepend(head);  // добавляет элемент в начало списка
    snakeBody_.removeLast();   // удаляем последний элемент списка (змейка остается той же длины)
}

void Snake::grow() {
    snakeBody_.append(snakeBody_.last());
}

void Snake::setDirection(Qt::Key d) {
    // Запрещаем движение в противоположную сторону
    if ((direction_ == Qt::Key_Up && d == Qt::Key_Down) ||
        (direction_ == Qt::Key_Down && d == Qt::Key_Up) ||
        (direction_ == Qt::Key_Left && d == Qt::Key_Right) ||
        (direction_ == Qt::Key_Right && d == Qt::Key_Left)) {
        return;
    }
    direction_ = d;
}

QList<QPoint> Snake::getBody(){
    return snakeBody_;
}

void Snake::reset() {
    snakeBody_.clear();

    snakeBody_.append(QPoint(70, 50));
    snakeBody_.append(QPoint(60, 50));
    snakeBody_.append(QPoint(50, 50));
    direction_ = Qt::Key_Right;
}
