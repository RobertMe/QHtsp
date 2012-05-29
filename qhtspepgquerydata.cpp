/*
 *  Copyright (C) 2012 Robert Meijers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qhtspepgquery.h"

QHtspEpgQueryData::QHtspEpgQueryData(QHtsp *htsp) :
    channel(0), events(new QHtspEventList(0, this)), htsp(htsp), tag(0)
{
}

QHtspEpgQueryData::QHtspEpgQueryData(const QHtspEpgQueryData &other) :
    QObject(0), QSharedData(other), channel(other.channel), events(new QHtspEventList(0, this)),
    htsp(other.htsp), query(other.query), tag(other.tag)
{
}
