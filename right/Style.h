#pragma once

#include <QString>
#include <QPushButton>

namespace style
{
    static const QString white          =      "#ffffff";
    static const QString black          =      "#191919";
    static const QString gray           =      "#808080";
    static const QString darkGray       =      "#353535";
    static const QString violet         =      "#8888ff";
    static const QString red            =      "#ff8888";
    static const QString green          =      "#88ff88";
    static const QString yellow         =      "#c3ff88";
    static const QString pink           =      "#ff88c3";
    static const QString blue           =      "#88c3ff";
}

inline void ColorButton( QPushButton* button, QColor color )
{
    QPalette palette = button->palette();
    palette.setColor( QPalette::Active, QPalette::ButtonText, color );
    button->setPalette( palette );
    button->update();
}
