#pragma once


#ifndef UI_SPARK_ENGINE_APP
#define UI_SPARK_ENGINE_APP

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>


class QtTitleFrame : public QFrame {
    Q_OBJECT


public:
    QMainWindow* app;
    QtTitleFrame(QWidget* parent);
    ~QtTitleFrame();
    void SetApp(QMainWindow* _app);
private:

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};


class QtApp : public QMainWindow
{
    Q_OBJECT

public:
    QtApp(QWidget* parent = nullptr);
    ~QtApp();

    void retranslateUi();
    void setupUi();

    QWidget* centralwidget;
    QVBoxLayout* drop_shadow_layout;
    QFrame* background;
    QVBoxLayout* verticalLayout;
    QtTitleFrame* title;
    QHBoxLayout* horizontalLayout;
    QFrame* frame_title;
    QVBoxLayout* verticalLayout_2;
    QFrame* frame_btns;
    QHBoxLayout* horizontalLayout_3;
    QPushButton* btn_minimize;
    QPushButton* btn_maximize;
    QPushButton* btn_close;
    QFrame* content;
    QVBoxLayout* verticalLayout_13;
    QFrame* bottomBar;
    QHBoxLayout* horizontalLayout_7;
    QFrame* bottomSpacer;
    QFrame* resizer;
    QLabel* resizer_icon;
    QSizeGrip* resizer_grip;

public Q_SLOTS:
    bool maximizeSlot();
};

#endif
