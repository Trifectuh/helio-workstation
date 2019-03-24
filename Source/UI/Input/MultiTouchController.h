/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "MultiTouchListener.h"

class MultiTouchController final : public MouseListener
{
public:

    explicit MultiTouchController(MultiTouchListener &parent);

    inline bool hasMultitouch() const noexcept
    {
        return this->finger1On && this->finger2On;
    }

    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

private:

    MultiTouchListener &listener;

    // absolute coordinates on the canvas:
    Point<double> point1;
    Point<double> point2;
    Point<double> center;

    Point<double> lastPoint1;
    Point<double> lastPoint2;
    Point<double> lastCenter;

    bool finger1On;
    bool finger2On;

};
