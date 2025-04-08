#ifndef SNAKE_CPP_FOOD_H
#define SNAKE_CPP_FOOD_H

#include <QList>
#include <QRandomGenerator>

class Food {
    private:
        QPoint position_;

    public:
        void generate(int width, int height, int foodSize, const QList<QPoint>& snakeBody);
        QPoint getPosition();
};

#endif //SNAKE_CPP_FOOD_H
