#include "food.h"

void Food::generate(int width, int height, int foodSize, const QList<QPoint>& snakeBody) {
    QPoint newPosition;
    bool overlapsFlag = true;

    while (overlapsFlag) {
        // предполагается, что размеры еды и стенок равны (возможно дальше поменяю)
        // "QRandomGenerator::global()->bounded(m, n)" — это метод для генерации случайных чисел от m до n-1
        // "1" - убираем возможность появления еды в стенке
        int x = QRandomGenerator::global()->bounded(1, (width - foodSize) / foodSize) * foodSize;
        int y = QRandomGenerator::global()->bounded(1, (height - foodSize) / foodSize) * foodSize;
        newPosition = QPoint(x, y);

        overlapsFlag = false;
        // проверка, что ячейка еды не создается в теле змейки
        for (const QPoint& point : snakeBody) {
            if (point == newPosition) {
                overlapsFlag = true;
                break;
            }
        }
    }
    position_ = newPosition;
}

QPoint Food::getPosition() {
    return position_;
}
