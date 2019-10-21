#pragma once

#include <QFrame>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>


namespace ProjectOperation {
class FileManager : public QFrame {
    Q_OBJECT

public:
    explicit FileManager(QWidget* parent = nullptr);

    void clear();

signals:
    void sendFilePath(const QString& ) const;

private slots:
    void recvFilePath(const QString& );

private:
    QLabel* _label;
    QLabel* _fileName;
    QPushButton* _button;

    QFileDialog* _fileDialog;
};


} // namespace ProjectOperation
