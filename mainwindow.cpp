#include "mainwindow.h"
#include "drawingcanvas.h" // Make sure this is included in your .cpp file
#include <QVBoxLayout>
#include <QHBoxLayout>     // Need to include QHBoxLayout
#include <QPushButton>
#include <QLabel>          // Need to include QLabel
#include <QSpinBox>        // Need to include QSpinBox
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    m_canvas = new DrawingCanvas;
    layout->addWidget(m_canvas);

    QHBoxLayout *sizeLayout = new QHBoxLayout;

    QLabel *sizeLabel = new QLabel("Window Size (px):");
    QSpinBox *sizeSpinBox = new QSpinBox;
    sizeSpinBox->setRange(5, 100);
    sizeSpinBox->setValue(m_canvas->getWindowSize());

    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(sizeSpinBox);
    sizeLayout->addStretch(); // Push the control to the left

    layout->addLayout(sizeLayout); // Add size control above the buttons

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Line Drawing (Segment Creation)
    QPushButton *btnPaintLines = new QPushButton("Draw Segments (Lines)");
    buttonLayout->addWidget(btnPaintLines);

    // Detection (Goal 3: Automated Algorithm)
    QPushButton *btnDetect = new QPushButton("Detect Windows");
    buttonLayout->addWidget(btnDetect);

    // Visualization (Verification)
    QPushButton *btnVisualize = new QPushButton("Visualize Detections");
    buttonLayout->addWidget(btnVisualize);

    // Maintenance
    QPushButton *btnClear = new QPushButton("Clear Canvas");
    buttonLayout->addWidget(btnClear);

    layout->addLayout(buttonLayout);

    centralWidget->setLayout(layout);

    connect(sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_canvas, &DrawingCanvas::setWindowSize);

    connect(btnDetect, &QPushButton::clicked, m_canvas, &DrawingCanvas::segmentDetection); // NOTE: Changed to segmentDetection

    connect(btnPaintLines, &QPushButton::clicked, m_canvas, &DrawingCanvas::paintLines);
    connect(btnClear, &QPushButton::clicked, m_canvas, &DrawingCanvas::clearAll);

    connect(btnVisualize, &QPushButton::clicked, m_canvas, &DrawingCanvas::visualizeDetections);
}

MainWindow::~MainWindow()
{
}
