/*
 * Copyright 2019-2020 PixlOne
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef LOGID_ACTION_NULL_H
#define LOGID_ACTION_NULL_H

#include "Action.h"

namespace logid {
namespace actions
{
    class NullAction : public Action
    {
    public:
        static const char* interface_name;

        explicit NullAction(Device* device);
        NullAction(Device* device, [[maybe_unused]] config::NoAction& config) :
            NullAction(device) { }

        virtual void press();
        virtual void release();

        virtual uint8_t reprogFlags() const;
    };
}}


#endif //LOGID_ACTION_NULL_H
