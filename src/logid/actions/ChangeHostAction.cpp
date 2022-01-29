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
#include <algorithm>
#include "ChangeHostAction.h"
#include "../Device.h"
#include "../backend/hidpp20/features/ReprogControls.h"
#include "../util/task.h"

using namespace logid::actions;
using namespace logid::backend;

const char* ChangeHostAction::interface_name =
        "pizza.pixl.LogiOps.Action.ChangeHost";

ChangeHostAction::ChangeHostAction(Device *device, config::ChangeHost& config)
    : Action(device), _config (config)
{
    if(std::holds_alternative<std::string>(_config.host)) {
        auto& host = std::get<std::string>(_config.host);
        std::transform(host.begin(), host.end(),
                       host.begin(), ::tolower);
    }
    try {
        _change_host = std::make_shared<hidpp20::ChangeHost>(&device->hidpp20());
    } catch (hidpp20::UnsupportedFeature& e) {
        logPrintf(WARN, "%s:%d: ChangeHost feature not supported, "
                        "ChangeHostAction will not work.", device->hidpp20()
                        .devicePath().c_str(), device->hidpp20().deviceIndex());
    }
}

void ChangeHostAction::press()
{
    // Do nothing, wait until release
}

void ChangeHostAction::release()
{
    if(_change_host) {
        spawn_task(
        [this] {
            auto host_info = _change_host->getHostInfo();
            int next_host;
            if(std::holds_alternative<std::string>(_config.host)) {
                const auto& host = std::get<std::string>(_config.host);
                if(host == "next")
                    next_host = host_info.currentHost + 1;
                else if(host == "prev" || host == "previous")
                    next_host = host_info.currentHost - 1;
                else
                    next_host = host_info.currentHost;
            } else {
                next_host = std::get<int>(_config.host)-1;
            }
            next_host %= host_info.hostCount;
            if(next_host != host_info.currentHost)
                _change_host->setHost(next_host-1);
        });
    }
}

uint8_t ChangeHostAction::reprogFlags() const
{
    return hidpp20::ReprogControls::TemporaryDiverted;
}
