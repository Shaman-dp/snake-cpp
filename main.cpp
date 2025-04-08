#include <QApplication> // для QApplication app(argc, argv);
#include <QWidget> // для GameWindow
#include <QPainter> // для метода paintEvent
#include <QKeyEvent> // для обработки событий (клава, мышь)
#include <QTimer> // для таймера
#include <iostream>
#include <QMessageBox>
#include <QLabel>
#include "src/snake.h"
#include "src/food.h"

class GameWindow : public QWidget {
    private:
        Snake snake_;
        QVector<QRect> walls_;
        Food food_;

        int foodSize = 10;
        int wallSize = 10;

        int score_ = 0; // счетчик очков
        int speed_ = 100; // скорость змейки (чем меньше тем быстрее т.к. это скорость обновления между кадрами)

        QLabel* speedLabel_; // виджет уведомления об увеличении скорости
        QTimer* timer_;
    public:
        GameWindow(QWidget *parent = nullptr) {
            setWindowTitle("Snake");
            setFixedSize(400, 400);

            speedLabel_ = new QLabel("Speed Up!", this);
            speedLabel_->setStyleSheet("QLabel { color: red; font: bold 20px; background: transparent; }");
            // static_cast<int> используется для явного преобразования типа
            speedLabel_->move(static_cast<int>(width() / 2 - speedLabel_->width() / 2), static_cast<int>(height() * 0.05));  // позиционируем по центру сверху
            speedLabel_->hide();  // по-умолчанию скрыто

            // размечаем стены
            walls_.append(QRect(0, 0, width(), wallSize));                       // верхняя
            walls_.append(QRect(0, height() - wallSize, width(), wallSize));     // нижняя
            walls_.append(QRect(0, 0, wallSize, height()));                      // левая
            walls_.append(QRect(width() - wallSize, 0, wallSize, height()));     // правая

            // генерируем еду
            food_.generate(width(), height(), foodSize, snake_.getBody());

            timer_ = new QTimer(this); // таймер для обновления игры
            // подключаем сигнал "timeout" к методу updateGame
            connect(timer_, &QTimer::timeout, this, &GameWindow::updateGame);
            timer_->start(speed_); // устанавливаем и запускаем таймер

            // NOTE: Qt — событийно-ориентированная система (Qt работает через систему событий (event loop))
            // как работает:
            // - внутри event loop создаётся событие timeout
            // - сигнал от события ставится в очередь
            // - когда дойдет очередь - вызывается updateGame
        }

        ~GameWindow() {
            delete speedLabel_;
            delete timer_; // таймер создавали в динамической памяти (через new), поэтому надо удалить вручную
        }

        // NOTE: методы Qt автоматически вызываются если:
        // - окно меняет размер
        // - вызывается update() или repaint()
        // - окно впервые отображается
        // ! все рисуется с нуля

        // метод перезапуска игры
        void restartGame() {
            snake_.reset();  // сброс состояния змейки
            food_.generate(width(), height(), foodSize, snake_.getBody());

            score_ = 0; // сброс очков
            speed_ = 100; // сброс скорости

            // перезапуск таймера
            timer_->start(speed_);

            // перерисовываем окно
            update();
        }

        // переопределяем стандартный метод Qt
        void paintEvent(QPaintEvent *event) override {
            QPainter painter(this);  // Объект для рисования
            // NOTE: this — это указатель на текущий виджет

            // рисуем стены
            painter.setBrush(Qt::darkGray);
            for (const QRect& wall : walls_) {
                painter.drawRect(wall);
            }

            // рисуем еду
            painter.setBrush(Qt::yellow);
            painter.drawRect(food_.getPosition().x(), food_.getPosition().y(), foodSize, foodSize);

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

        // метод обработки событий от пользователя
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

            // проверка на столкновение змейки со стенами
            for (const QRect& wall : walls_) {
                // "contains" проверяет, находится ли точка (голова змейки) внутри прямоугольника
                if (wall.contains(snake_.getBody().first())) {
                    timer_->stop(); // останавливаем игровой таймер и змейка больше не двигается

                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Game Over", QString("You hit a wall!\nScore: %1\nDo you want to restart?").arg(score_),
                                                  QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        restartGame();  // перезапуск игры
                    } else {
                        QApplication::quit();  // выход из игры
                    }
                }
            }

            // проверка на самопересечение
            // начинаем сравнивать с 1, чтобы не сравнивать голову саму с собой
            for (int i = 1; i < snake_.getBody().size(); i++) {
                if (snake_.getBody()[i] == snake_.getBody().first()) {
                    timer_->stop();

                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Game Over", QString("You hit yourself!\nScore: %1\nDo you want to restart?").arg(score_),
                                                  QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        restartGame();  // перезапуск игры
                    } else {
                        QApplication::quit();  // выход из игры
                    }
                }
            }

            // проверка, что змейка съела еду
            if (snake_.getBody().first() == food_.getPosition()) {
                snake_.grow(); // удлиняем змейку
                score_++;
                food_.generate(width(), height(), foodSize, snake_.getBody());   // генерируем новую еду

                // каждые 10 очков увеличиваем скорость (ограничиваем скорость до 40 мс)
                if (score_ % 10 == 0 && speed_ > 40) {
                    speed_ -= 10;
                    timer_->start(speed_);

                    // показываем виджет на 1500 мс
                    speedLabel_->show();
                    QTimer::singleShot(1500, speedLabel_, &QLabel::hide);
                }
            }

            if (!timer_->isActive()) {
                return;
            }

            update(); // перерисовываем окно (вызывается метод paintEvent)
        }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWindow window;  // создаем окно игры
    window.show();

    return QApplication::exec();  // запуск цикла обработки событий
}
