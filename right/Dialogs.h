#include <QDialog>

class QPushButton;

class ErrorMessageBox : public QDialog
{
    Q_OBJECT

    private :
        QPushButton *fButton;

    public :
        ErrorMessageBox( const QString& message, QWidget *parent = nullptr );
        ~ErrorMessageBox() override = default;
};
