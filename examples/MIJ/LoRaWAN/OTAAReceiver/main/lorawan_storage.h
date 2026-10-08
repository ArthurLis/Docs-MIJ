#pragma once

#include "RadioLib.h"

namespace lorawanStorage {

bool initialize();
bool restore(LoRaWANNode &node, const char *nameSpace);
bool save(LoRaWANNode &node, const char *nameSpace);
bool clear(const char *nameSpace);

} // namespace lorawanStorage
