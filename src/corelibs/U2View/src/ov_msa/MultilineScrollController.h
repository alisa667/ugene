/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2021 UniPro <ugene@unipro.ru>
 * http://ugene.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include <QScrollArea>

#include <U2Core/U2Region.h>

namespace U2 {

class GScrollBar;
class MaEditor;
class MaEditorSelection;
class MaEditorMultilineWgt;

class U2VIEW_EXPORT MultilineScrollController : public QObject {
    Q_OBJECT
public:
    enum Direction {
        None = 0,
        Up = 1 << 0,
        Down = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        SliderMaximum = 1 << 4,
        SliderMinimum = 1 << 5,
        SliderMoved = 1 << 6,
        PageUp = 1 << 7,
        PageDown = 1 << 8
    };
    Q_DECLARE_FLAGS(Directions, Direction)

    MultilineScrollController(MaEditor* maEditor, MaEditorMultilineWgt* ui);

    void init(GScrollBar* _vScrollBar, QScrollArea* childrenArea);

    // enable/disable multiline scroll controller while switching multiline mode
    void setEnable(bool enable = true);
    bool isEnabled() {
        return enabled;
    };

    void scrollToViewRow(QPoint maPoint);
    void scrollToBase(QPoint maPoint);
    void scrollToPoint(const QPoint& maPoint);

    void setMultilineVScrollbarValue(int value);
    void setMultilineVScrollbarBase(int base);

    void setFirstVisibleBase(int firstVisibleBase);
    void setFirstVisibleViewRow(int viewRowIndex);
    void setFirstVisibleMaRow(int maRowIndex);

    void scrollSmoothly(const Directions& directions);
    void stopSmoothScrolling();

    void scrollStep(Direction direction);
    void scrollPage(Direction direction);
    void scrollToEnd(Direction direction);

    int getFirstVisibleBase(bool countClipped = false) const;
    int getLastVisibleBase(int widgetWidth, bool countClipped = false) const;
    int getFirstVisibleMaRowIndex(bool countClipped = false) const;
    int getFirstVisibleViewRowIndex(bool countClipped = false) const;
    int getLastVisibleViewRowIndex(int widgetHeight, bool countClipped = false) const;

    GScrollBar* getVerticalScrollBar() const;

    void vertScroll(const Directions& directions, bool byStep = true);
    int getViewHeight();

    /** Called right after zoom-on/out/reset or any other font change operation to update internal scrollbars scales. */
    void updateScrollBarsOnFontOrZoomChange() {
        Q_ASSERT(false);
    }

signals:
    void si_visibleAreaChanged();
    void si_vScrollValueChanged();

public slots:
    void sl_updateScrollBars();
    void sl_zoomScrollBars();
    void sl_vScrollValueChanged();
    void sl_handleVScrollAction(int action);

private:
    // enabled status
    bool enabled = false;
    // the enableSignals() must be called after init()
    // the function create/remove connections
    void initSignals(bool enable);
    QMetaObject::Connection connAreaChanged;
    QMetaObject::Connection connVValueChanged;
    QMetaObject::Connection connVActionTriggered;

    int getAdditionalXOffset() const;  // in pixels;
    int getAdditionalYOffset() const;  // in pixels;

    U2Region getVerticalRangeToDrawIn(int widgetHeight) const;  // in pixels

    void zoomVerticalScrollBarPrivate();
    void updateVerticalScrollBarPrivate();
    void updateChildrenScrollBarsPeivate();

    bool eventFilter(QObject* object, QEvent* event);
    bool vertEventFilter(QWheelEvent* event);

    MaEditor* maEditor;
    MaEditorMultilineWgt* ui;
    QScrollArea* childrenScrollArea;
    GScrollBar* vScrollBar;
    int hScrollTail;
    int vScrollTail;

    int savedFirstVisibleMaRow;
    int savedFirstVisibleMaRowOffset;

    bool checkBoundary();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MultilineScrollController::Directions)

}  // namespace U2
