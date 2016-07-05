#pragma once
#ifndef _MYLEVELDB_PORT_H
#define _MYLEVELDB_PORT_H

#include <string.h>
#if defined(LEVELDB_PLATFORM_WINDOWS)
#include "../MyLevelDB/port_win.h"
#include "../MyLevelDB/sha1_portable.h"
#endif

#endif // !_MYLEVELDB_PORT_H