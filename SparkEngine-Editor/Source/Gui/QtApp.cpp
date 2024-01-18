#include "QtApp.h"

QtApp::QtApp(QWidget* parent)
    : QMainWindow(parent)
{
    this->setupUi();
}

bool QtApp::maximizeSlot()
{
    if (this->isMaximized()) {
        this->showNormal();
        return false;
    }
    else {
        this->showMaximized();
        return true;
    }
}

void QtApp::setupUi()
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    if (this->objectName().isEmpty())
        this->setObjectName("MainWindow");
    this->resize(880, 600);
    this->setMinimumSize(QSize(880, 600));
    QPalette palette;
    QBrush brush(QColor(240, 240, 240, 0));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
    this->setPalette(palette);
    centralwidget = new QWidget(this);
    centralwidget->setObjectName("centralwidget");
    centralwidget->setStyleSheet(QString::fromUtf8("border-radius: 5px;\n"
        "background:transparent;"));
    drop_shadow_layout = new QVBoxLayout(centralwidget);
    drop_shadow_layout->setSpacing(0);
    drop_shadow_layout->setObjectName("drop_shadow_layout");
    drop_shadow_layout->setContentsMargins(0, 0, 0, 0);
    background = new QFrame(centralwidget);
    background->setObjectName("background");
    background->setStyleSheet(QString::fromUtf8("background-color: rgb(45, 45, 45);\n"
        "border-radius: 5px;"));
    background->setFrameShape(QFrame::NoFrame);
    background->setFrameShadow(QFrame::Raised);
    verticalLayout = new QVBoxLayout(background);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName("verticalLayout");
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    title = new QtTitleFrame(background);
    title->SetApp(this);
    title->setObjectName("title");
    title->setMaximumSize(QSize(16777215, 40));
    title->setStyleSheet(QString::fromUtf8("background-color: none;"));
    title->setFrameShape(QFrame::NoFrame);
    title->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(title);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName("horizontalLayout");
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    frame_title = new QFrame(title);
    frame_title->setObjectName("frame_title");
    frame_title->setMinimumSize(QSize(0, 40));
    QFont font;
    font.setFamilies({ QString::fromUtf8("Roboto Condensed Light") });
    font.setPointSize(14);
    frame_title->setFont(font);
    frame_title->setFrameShape(QFrame::StyledPanel);
    frame_title->setFrameShadow(QFrame::Raised);
    verticalLayout_2 = new QVBoxLayout(frame_title);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setObjectName("verticalLayout_2");
    verticalLayout_2->setContentsMargins(15, 0, 0, 0);

    horizontalLayout->addWidget(frame_title);

    frame_btns = new QFrame(title);
    frame_btns->setObjectName("frame_btns");
    frame_btns->setMaximumSize(QSize(100, 16777215));
    frame_btns->setFrameShape(QFrame::StyledPanel);
    frame_btns->setFrameShadow(QFrame::Raised);
    horizontalLayout_3 = new QHBoxLayout(frame_btns);
    horizontalLayout_3->setObjectName("horizontalLayout_3");
    btn_minimize = new QPushButton(frame_btns);
    btn_minimize->setObjectName("btn_minimize");
    btn_minimize->setMinimumSize(QSize(16, 16));
    btn_minimize->setMaximumSize(QSize(30, 30));
    btn_minimize->setStyleSheet(QString::fromUtf8("QPushButton {\n"
        "	border: none;\n"
        "	border-radius: 2px;		\n"
        "	background-color: transparent;\n"
        "}\n"
        "QPushButton:hover {	\n"
        "	background-color: rgb(166, 166, 166);\n"
        "}"));
    QIcon icon;
    icon.addFile(QString::fromUtf8("assets/icons/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_minimize->setIcon(icon);
    QObject::connect(btn_minimize, SIGNAL(clicked()), this, SLOT(showMinimized()));

    horizontalLayout_3->addWidget(btn_minimize);


    btn_maximize = new QPushButton(frame_btns);
    btn_maximize->setObjectName("btn_maximize");
    btn_maximize->setMinimumSize(QSize(16, 16));
    btn_maximize->setMaximumSize(QSize(30, 30));
    btn_maximize->setStyleSheet(QString::fromUtf8("QPushButton {\n"
        "	border: none;\n"
        "	border-radius: 2px;	\n"
        "	background-color: transparent;\n"
        "}\n"
        "QPushButton:hover {	\n"
        "	background-color: rgb(166, 166, 166);\n"
        "}"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8("assets/icons/square.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_maximize->setIcon(icon1);
    QObject::connect(btn_maximize, SIGNAL(clicked()), this, SLOT(maximizeSlot()));
    



    horizontalLayout_3->addWidget(btn_maximize);

    btn_close = new QPushButton(frame_btns);
    btn_close->setObjectName("btn_close");
    btn_close->setMinimumSize(QSize(16, 16));
    btn_close->setMaximumSize(QSize(30, 30));
    btn_close->setStyleSheet(QString::fromUtf8("QPushButton {\n"
        "	border: none;\n"
        "	border-radius: 2px;		\n"
        "	background-color: transparent;\n"
        "}\n"
        "QPushButton:hover {		\n"
        "	background-color: rgb(184, 76, 78);\n"
        "}"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8("assets/icons/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
    btn_close->setIcon(icon2);
    QObject::connect(btn_close, SIGNAL(clicked()), this, SLOT(close()));
    
    horizontalLayout_3->addWidget(btn_close);


    horizontalLayout->addWidget(frame_btns);


    verticalLayout->addWidget(title);

    content = new QFrame(background);
    content->setObjectName("content");
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(content->sizePolicy().hasHeightForWidth());
    content->setSizePolicy(sizePolicy);
    content->setMaximumSize(QSize(16777215, 16777215));
    content->setStyleSheet(QString::fromUtf8("background-color: none;"));
    content->setFrameShape(QFrame::StyledPanel);
    content->setFrameShadow(QFrame::Raised);
    verticalLayout_13 = new QVBoxLayout(content);
    verticalLayout_13->setSpacing(0);
    verticalLayout_13->setObjectName("verticalLayout_13");
    verticalLayout_13->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(content);

    bottomBar = new QFrame(background);
    bottomBar->setObjectName("bottomBar");
    sizePolicy.setHeightForWidth(bottomBar->sizePolicy().hasHeightForWidth());
    bottomBar->setSizePolicy(sizePolicy);
    bottomBar->setMaximumSize(QSize(16777215, 25));
    bottomBar->setStyleSheet(QString::fromUtf8("background-color: none;"));
    bottomBar->setFrameShape(QFrame::StyledPanel);
    bottomBar->setFrameShadow(QFrame::Raised);
    horizontalLayout_7 = new QHBoxLayout(bottomBar);
    horizontalLayout_7->setObjectName("horizontalLayout_7");
    horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
    bottomSpacer = new QFrame(bottomBar);
    bottomSpacer->setObjectName("bottomSpacer");
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(bottomSpacer->sizePolicy().hasHeightForWidth());
    bottomSpacer->setSizePolicy(sizePolicy1);
    bottomSpacer->setMaximumSize(QSize(1677721, 1677721));
    bottomSpacer->setFrameShape(QFrame::StyledPanel);
    bottomSpacer->setFrameShadow(QFrame::Raised);

    horizontalLayout_7->addWidget(bottomSpacer);

    resizer = new QFrame(bottomBar);
    resizer->setObjectName("resizer");
    sizePolicy1.setHeightForWidth(resizer->sizePolicy().hasHeightForWidth());
    resizer->setSizePolicy(sizePolicy1);
    resizer->setMaximumSize(QSize(28, 28));
    resizer->setStyleSheet(QString::fromUtf8(""));
    resizer->setFrameShape(QFrame::StyledPanel);
    resizer->setFrameShadow(QFrame::Raised);
    resizer_icon = new QLabel(resizer);
    resizer_icon->setObjectName("resizer_icon");
    resizer_icon->setGeometry(QRect(4, 0, 21, 21));
    resizer_icon->setPixmap(QPixmap(QString::fromUtf8("assets/icons/resize-dots.png")));
    resizer_icon->setScaledContents(true);
    resizer_grip = new QSizeGrip(resizer);
    resizer_grip->setObjectName("resizer_grip");
    resizer_grip->setGeometry(QRect(6, 4, 21, 21));

    horizontalLayout_7->addWidget(resizer);


    verticalLayout->addWidget(bottomBar);


    drop_shadow_layout->addWidget(background);

    this->setCentralWidget(centralwidget);

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
} // setupUi

void QtApp::retranslateUi()
{
#if QT_CONFIG(tooltip)
    btn_minimize->setToolTip(QCoreApplication::translate("MainWindow", "Minimize", nullptr));
#endif // QT_CONFIG(tooltip)
    btn_minimize->setText(QString());
#if QT_CONFIG(tooltip)
    btn_maximize->setToolTip(QCoreApplication::translate("MainWindow", "Maximize", nullptr));
#endif // QT_CONFIG(tooltip)
    btn_maximize->setText(QString());
#if QT_CONFIG(tooltip)
    btn_close->setToolTip(QCoreApplication::translate("MainWindow", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
    btn_close->setText(QString());
    resizer_icon->setText(QString());
    (void)this;
} // retranslateUi

QtApp::~QtApp()
{}


/////////////////////////////////////////////////////////


QtTitleFrame::QtTitleFrame(QWidget* parent)
{
    new QFrame(parent);
}

void QtTitleFrame::SetApp(QMainWindow* _app)
{
    app = _app;
}

void QtTitleFrame::mousePressEvent(QMouseEvent* event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void QtTitleFrame::mouseMoveEvent(QMouseEvent* event) {
    app->move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

QtTitleFrame::~QtTitleFrame()
{

}