#ifndef SNAKE_CPP_SNAKE_H
#define SNAKE_CPP_SNAKE_H

#include <QList>

class Snake {
    private:
        QList<QPoint> snakeBody_; // QPoint - точка на плоскости с координатами x и y
        Qt::Key direction_; // направление движения
    public:
        Snake();

        void move(); // метод движения змейки
        void grow(); // увеличение змейки при поедании еды
        void setDirection(Qt::Key d); // метод смены направления движения
        QList<QPoint> getBody();

        void reset(); // сброс состояния змейки (для перезапуска)
};

#endif
