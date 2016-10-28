#ifndef RCEXCEPTIONS_H
#define RCEXCEPTIONS_H

#include <QtCore>

class InvalidOperationException : public QException
{
public:
    InvalidOperationException(QString msg) { message = msg; }
    QString Message() { return message; }
    void raise() const { throw *this; }
    InvalidOperationException *clone() const { return new InvalidOperationException(*this); }

private:
    QString message;
};

class ArgumentNullException : public QException
{
public:
    ArgumentNullException(QString msg) { message = msg; }
    QString Message() { return message; }
    void raise() const { throw *this; }
    ArgumentNullException *clone() const { return new ArgumentNullException(*this); }

private:
    QString message;
};

#endif // RCEXCEPTIONS_H
