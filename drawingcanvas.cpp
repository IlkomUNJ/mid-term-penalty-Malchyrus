#include "drawingcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>
#include <cmath>

// --- Define Constants for Pattern Check (Goal 2: The justification for 50 goes in report.md) ---
const int PATTERN_RED_PIXEL_THRESHOLD = 50;
const QRgb RED_LINE_COLOR = QColor(Qt::red).rgb();

// --- Helper Function: Check Window for Red Line Pixels ---
bool DrawingCanvas::checkWindowForPattern(const QImage& image, int x, int y, int size) {
    int redPixelCount = 0;

    // Safety check: ensure window is within the image bounds
    if (x < 0 || y < 0 || x + size > image.width() || y + size > image.height()) {
        return false;
    }

    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            // Check if the pixel color is the red line color
            if (image.pixel(i, j) == RED_LINE_COLOR) {
                redPixelCount++;
            }
        }
    }

    // Pattern Criteria Check
    if (redPixelCount >= PATTERN_RED_PIXEL_THRESHOLD) {
        // Goal 2: "DUMP ALL NON EMPTY WINDOWS" logging
        qDebug() << "DUMP: Window at (" << x << "," << y << ") has"
                 << redPixelCount << "red pixels. (Segment piece detected)";
        return true;
    }

    return false;
}

// --- Constructor and Basic Events ---
DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent) {
    // Setting a default size, though MainWindow manages window size
    setMinimumSize(400, 400);
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    points.append(event->pos());
    update();
}

void DrawingCanvas::clearAll() {
    points.clear();
    detectedWindows.clear();
    isPaintLinesClicked = false;
    update();
}

void DrawingCanvas::paintLines(){
    isPaintLinesClicked = true;
    update();
}

// --- Segment Detection (Pixel-Based Sliding Window) ---
void DrawingCanvas::segmentDetection() {
    detectedWindows.clear();

    // Grab the current state of the drawing (including red lines)
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();

    const int W = windowSize;
    const int H = windowSize;
    // Stride equals size for non-overlapping windows (simpler detection)
    const int STRIDE = windowSize;

    qDebug() << "Starting detection with window size:" << W << "x" << H;

    // Slide the Window across the canvas
    for (int y = 0; y <= image.height() - H; y += STRIDE) {
        for (int x = 0; x <= image.width() - W; x += STRIDE) {

            // Check the window content against the pattern criteria (Goal 2)
            if (checkWindowForPattern(image, x, y, W)) {
                // Goal 3: Store the detected window's bounding box
                detectedWindows.append(QRect(x, y, W, H));
            }
        }
    }

    qDebug() << "Total segments detected:" << detectedWindows.size();
    update(); // Redraw to show the purple rectangles (detections)
}

void DrawingCanvas::visualizeDetections() {
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen dotPen(Qt::blue, 5);
    painter.setPen(dotPen);
    painter.setBrush(QBrush(Qt::blue));
    for (const QPointF &pt : points) {
        painter.drawEllipse(pt, 3, 3);
    }

    if (isPaintLinesClicked && points.size() >= 2) {
        QPen linePen(Qt::red, 4); // 4-pixel wide line
        painter.setPen(linePen);

        // Draw lines per even pair (dot 1 to 2, 3 to 4, etc.)
        for(int i = 0; i < points.size() - 1; i += 2) {
            painter.drawLine(points[i], points[i+1]);
        }
    }

    painter.setPen(QPen(Qt::magenta, 2));
    painter.setBrush(Qt::NoBrush);
    for (const QRect &r : detectedWindows) {
        painter.drawRect(r);
    }
}
