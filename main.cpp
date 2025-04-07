#include <QApplication> // для QApplication app(argc, argv);
#include <QWidget> // для GameWindow
#include <QPainter> // для метода paintEvent
#include <QKeyEvent> // для обработки событий (клава, мышь)
#include <QTimer> // для таймера
#include <iostream>
#include "src/snake.h"

class GameWindow : public QWidget {
    private:
        Snake snake_;
        QTimer* timer_;
    public:
        GameWindow(QWidget *parent = nullptr) {
            setWindowTitle("Snake");
            setFixedSize(400, 400);

            timer_ = new QTimer(this); // таймер для обновления игры
            // подключаем сигнал "timeout" к методу updateGame
            connect(timer_, &QTimer::timeout, this, &GameWindow::updateGame);
            timer_->start(100); // обновление каждые 100 мс

            // NOTE: Qt — событийно-ориентированная система (Qt работает через систему событий (event loop))
            // как работает:
            // - внутри event loop создаётся событие timeout
            // - сигнал от события ставится в очередь
            // - когда дойдет очередь - вызывается updateGame
        }

        ~GameWindow() {
            delete timer_; // таймер создавали в динамической памяти (через new), поэтому надо удалить вручную
        }

        // NOTE: методы Qt автоматически вызываются если:
        // - окно меняет размер
        // - вызывается update() или repaint()
        // - окно впервые отображается
        // ! все рисуется с нуля

        // переопределяем стандартный метод Qt
        void paintEvent(QPaintEvent *event) override {
            QPainter painter(this);  // Объект для рисования
            // NOTE: this — это указатель на текущий виджет

            painter.setBrush(Qt::green);  // Устанавливаем цвет для тела змейки

            // отрисовка всех сегментов тела змейки
            // NOTE: используем "QPoint&", чтобы избежать копирования данных в point
            //       используем "const", чтобы случайно не изменить данные
            // такая конструкция цикла for позволяет перебрать все элементы списка, который
            // получаем методом getBody()
            for (const QPoint& point : snake_.getBody()) {
                painter.drawRect(point.x(), point.y(), 10, 10);
            }
        }

        // в будущем реализация данного метода будет изменена
        // сейчас, при нажатии на стрелки, к змейке добавляется элемент в соответствующем направлении
        void keyPressEvent(QKeyEvent *event) override {
            if (event->key() == Qt::Key_Up) {
                snake_.setDirection(Qt::Key_Up);
            }
            if (event->key() == Qt::Key_Down) {
                snake_.setDirection(Qt::Key_Down);
            }
            if (event->key() == Qt::Key_Left) {
                snake_.setDirection(Qt::Key_Left);
            }
            if (event->key() == Qt::Key_Right) {
                snake_.setDirection(Qt::Key_Right);
            }
        }

        void updateGame() {
            snake_.move();
            update(); // перерисовываем окно (вызывается метод paintEvent)
        }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWindow window;  // создаем окно игры
    window.show();

    return QApplication::exec();  // запуск цикла обработки событий
}
