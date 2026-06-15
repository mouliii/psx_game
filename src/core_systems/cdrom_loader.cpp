#include "cdrom_loader.h"

CdRomLoader* CdRomLoader::m_instance;

void CdRomLoader::Initialize()
{
    if (m_instance == nullptr) 
    {
        m_instance = new CdRomLoader();
    }
    CdRomLoader::Instance().m_cdrom.reset();
    CdRomLoader::Instance().m_cdrom.prepare();
    CdRomLoader::Instance().m_isoParser.initialize();
}

psyqo::Coroutine<psyqo::Buffer<uint8_t>> CdRomLoader::LoadFile(eastl::string_view filePath)
{
    // it is safe to call readFile() again from the callback. If preferred, the loader can be cascaded into another `TaskQueue`.
    psyqo::Buffer<uint8_t> data = co_await m_cdRomLoader.readFile(filePath, m_isoParser);

    co_return eastl::move(data);
}

psyqo::TaskQueue::Task CdRomLoader::LoadFileScheduled(eastl::string_view filePath)
{
    return eastl::move(m_cdRomLoader.scheduleReadFile(filePath, m_isoParser));
}
