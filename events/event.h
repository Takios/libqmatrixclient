/******************************************************************************
 * Copyright (C) 2015 Felix Rohrbach <kde@fxrh.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef QMATRIXCLIENT_EVENT_H
#define QMATRIXCLIENT_EVENT_H

#include <algorithm>

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>

class QJsonArray;

namespace QMatrixClient
{
    enum class EventType
    {
        RoomMessage, RoomName, RoomAliases, RoomCanonicalAlias,
        RoomMember, RoomTopic, Typing, Receipt, Unknown
    };
    
    class Event
    {
        public:
            Event(EventType type);
            virtual ~Event();
            
            EventType type() const;
            QString id() const;
            QDateTime timestamp() const;
            QString roomId() const;
            // only for debug purposes!
            QString originalJson() const;

            static Event* fromJson(const QJsonObject& obj);
            
        protected:
            bool parseJson(const QJsonObject& obj);
        
        private:
            class Private;
            Private* d;
    };

    QList<Event*> eventListFromJson(const QJsonArray& contents);

    /**
     * Finds a place in the timeline where a new event/message could be inserted.
     * @return an iterator to an item with the latest timestamp before
     * the one of 'item'; or timeline.end(), if all events are later.
     */
    template <class ItemT, class ContT>
    typename ContT::iterator
    findInsertionPos(ContT & timeline, const ItemT *item)
    {
        return std::lower_bound (timeline.begin(), timeline.end(), item,
            [](const typename ContT::value_type a, const ItemT * b) {
                return a->timestamp() > b->timestamp();
            }
        );
    }
}

#endif // QMATRIXCLIENT_EVENT_H
