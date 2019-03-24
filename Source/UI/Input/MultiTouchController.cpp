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

#include "Common.h"
#include "MultiTouchController.h"

#if HELIO_DESKTOP
#   define ZOOM_THRESHOLD 0.5
#   define DRAG_THRESHOLD 10
#   define ZOOM_H_SPEED 0.005f
#   define ZOOM_V_SPEED 0.01f
#elif JUCE_IOS
#   define ZOOM_THRESHOLD 0.1f
#   define DRAG_THRESHOLD 25
#   define ZOOM_H_SPEED 0.0095f
#   define ZOOM_V_SPEED 0.02f
#elif JUCE_ANDROID
#   define ZOOM_THRESHOLD 0.15f
#   define DRAG_THRESHOLD 25
#   define ZOOM_H_SPEED 0.0025f
#   define ZOOM_V_SPEED 0.005f
#endif

MultiTouchController::MultiTouchController(MultiTouchListener &parent) :
    listener(parent),
    point1(0.0, 0.0),
    point2(0.0, 0.0),
    center(0.0, 0.0),
    lastPoint1(0.0, 0.0),
    lastPoint2(0.0, 0.0),
    lastCenter(0.0, 0.0),
    finger1On(false),
    finger2On(false) {}

static Point<double> getAbsolutePosition(const MouseEvent &event)
{
    return event.position.toDouble() / event.eventComponent->getLocalBounds().getBottomRight().toDouble();
}

//static Point<int> getPxPosition(const Point<double> &pos, Component *component)
//{
//    return (pos * component->getLocalBounds().getBottomRight()).toInt();
//}

void MultiTouchController::mouseDown(const MouseEvent &event)
{
    if (event.source.getIndex() > 1)
    {
        return;
    }

    if (event.source.getIndex() == 0)
    {
        this->finger1On = true;
        this->point1 = getAbsolutePosition(event);
        this->lastPoint1 = this->point1;
    }
    else if (event.source.getIndex() == 1)
    {
        this->finger2On = true;
        this->point2 = getAbsolutePosition(event);
        this->lastPoint2 = this->point2;
    }

    this->center = (this->point1 + this->point2) / 2.0;
    this->lastCenter = this->center;

    if (this->hasMultitouch())
    {
        this->listener.multiTouchEvent({ 0.0, 0.0 }, this->center, { 1.0, 1.0 });
    }
}

void MultiTouchController::mouseDrag(const MouseEvent &event)
{
    if (!this->hasMultitouch())
    {
        return;
    }
    
    if (event.source.getIndex() == 0)
    {
        this->point1 = getAbsolutePosition(event);
    }
    else if (event.source.getIndex() == 1)
    {
        this->point2 = getAbsolutePosition(event);
    }

    this->center = (this->point1 + this->point2) / 2.0;
    const auto dragOffset = this->center - this->lastCenter;

    const auto newDistance = this->point2 - this->point1;
    const auto lastDistance = this->lastPoint2- this->lastPoint1;
    const auto zoomH = abs(lastDistance.x) / abs(newDistance.x);
    const auto zoomV = abs(lastDistance.y) / abs(newDistance.y);
    this->listener.multiTouchEvent(dragOffset, this->center, { zoomH, zoomV });

    this->lastPoint1 = this->point1;
    this->lastPoint2 = this->point2;
    this->lastCenter = this->center;
}

void MultiTouchController::mouseUp(const MouseEvent &event)
{
    if (this->hasMultitouch())
    {
        this->finger1On = false;
        this->finger2On = false;
    }
}
