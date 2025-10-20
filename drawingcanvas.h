#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QRect>
#include <QImage>

class DrawingCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingCanvas(QWidget *parent = nullptr);
    void clearAll();

    int getWindowSize() const { return windowSize; } // Getter
public slots:
    void setWindowSize(int size) { windowSize = size; update(); } // Setter/Slot
    // -----------------------------------------------------------------

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    // NOTE: public slots: should be moved here for best practice,
    // but can remain public slots: above if you prefer.
public slots:
    void segmentDetection();    // Renamed from detectWindows, check your .cpp for consistency
    void visualizeDetections();
    void paintLines();

private:
    QVector<QPointF> points;
    QVector<QRect> detectedWindows;
    int windowSize = 20; // The default experimental size
    bool isPaintLinesClicked = false;

    // Check for segment pattern (red pixels) inside the window
    bool checkWindowForPattern(const QImage& image, int x, int y, int size);
};

#endif // DRAWINGCANVAS_H
