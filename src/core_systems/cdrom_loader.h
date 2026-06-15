#pragma once

#include "psyqo/coroutine.hh"
#include "psyqo/buffer.hh"
#include "psyqo/iso9660-parser.hh"
#include "psyqo/cdrom-device.hh"
#include "psyqo-paths/cdrom-loader.hh"
#include "EASTL/functional.h"
#include "tim_file.h"

class CdRomLoader
{
public:
    static CdRomLoader& Instance() { return *m_instance; }
    static void Initialize();
    psyqo::Coroutine<psyqo::Buffer<uint8_t>> LoadFile(eastl::string_view filePath);
    psyqo::TaskQueue::Task LoadFileScheduled(eastl::string_view filePath);
private:
    CdRomLoader() {}
    static CdRomLoader* m_instance;
    // CD ROM
    psyqo::CDRomDevice m_cdrom;
    psyqo::ISO9660Parser m_isoParser = psyqo::ISO9660Parser(&m_cdrom);
    psyqo::paths::CDRomLoader m_cdRomLoader = psyqo::paths::CDRomLoader();
};