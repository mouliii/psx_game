#include "graphics.h"

Graphics* Graphics::m_instance = nullptr;

Graphics::Graphics(psyqo::GPU& gpu)
    : m_gpu(gpu)
{
}

void Graphics::Initialize(psyqo::GPU& gpu)
{
    if (m_instance == nullptr) 
    {
        m_instance = new Graphics(gpu);
    }
}

void Graphics::BeginFrame() {
    m_parity = m_gpu.getParity();
    m_primitiveBuffers[m_parity].reset();
    // The [OT] table is automatically cleared by the `GPU` class after it has been sent to the GPU

    auto& clear = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::FastFill>();
    //m_gpu.getNextClear(clear.primitive, clearColor); // examples/discord use this, it clears my wrong render buffer ???? 
    m_gpu.getClear(clear.primitive, clearColor);
    m_gpu.chain(clear);
    //m_orderingTables[m_parity].insert(clear, OT_SIZE - 1);
}

void Graphics::EndFrame()
{
    //m_gpu.chain(m_orderingTables[m_parity]);
    m_gpu.sendChain();
}
// TODO: make own text drawing system then put this to EndFrame();
void Graphics::SubmitOT()
{
    m_gpu.chain(m_orderingTables[m_parity]);
}

void Graphics::SetClearColor(psyqo::Color color)
{
    clearColor = color;
}

void Graphics::SetActiveCamera(Camera2D* camera) {
    m_activeCamera = camera;
}

psyqo::Vertex Graphics::GetTextureUV(Texture *texture)
{
    //uint16_t TPageVramX = (texture->x / TPAGE_WIDTH) * TPAGE_WIDTH;
    //uint16_t TPageVramY = (texture->y / TPAGE_HEIGHT) * TPAGE_HEIGHT;

    int16_t TPageVramX = texture->x & ~255;
    int16_t TPageVramY = texture->y & ~255;
    int16_t uvx = (texture->x - TPageVramX) << (2 - texture->colorMode);
    int16_t uvy = texture->y - TPageVramY;

    return {uvx, uvy};
}

void Graphics::DrawTexturedQuad(const Texture *texture, const psyqo::Vec2 pos, const psyqo::Vertex size, const psyqo::Rect &uv, uint16_t otEntry, psyqo::Color color)
{
    auto& texQuad = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::TexturedQuad>();
    if (!texture)
    {
        // TODO: ASSERT !TEX
        syscall_puts("TEXTURE IS NULL !!\n");
        return;
    }
    const int16_t screenX = static_cast<int16_t>((pos.x - m_activeCamera->pos.x).integer() + m_activeCamera->CAMERA_OFFSET.x);
    const int16_t screenY = static_cast<int16_t>((pos.y - m_activeCamera->pos.y).integer() + m_activeCamera->CAMERA_OFFSET.y);

    psyqo::PrimPieces::ClutIndex clutIndex(texture->clutX, texture->clutY);
    psyqo::PrimPieces::TPageAttr tPage;
    tPage.setPageX(texture->x / TPAGE_WIDTH);
    tPage.setPageY(texture->y / TPAGE_HEIGHT);
    tPage.set(texture->colorMode);
    texQuad.primitive.clutIndex = clutIndex;
    texQuad.primitive.tpage = tPage;
    texQuad.primitive.pointA = {screenX, screenY};
    texQuad.primitive.pointB = {static_cast<int16_t>(screenX + size.x), screenY};
    texQuad.primitive.pointC = {screenX, static_cast<int16_t>(screenY + size.y)};
    texQuad.primitive.pointD = {static_cast<int16_t>(screenX + size.x), static_cast<int16_t>(screenY + size.y)};
    texQuad.primitive.uvA = psyqo::PrimPieces::UVCoords({static_cast<uint8_t>(uv.pos.x), static_cast<uint8_t>(uv.pos.y)});
    texQuad.primitive.uvB = psyqo::PrimPieces::UVCoords({static_cast<uint8_t>(uv.pos.x + uv.size.x - 1), static_cast<uint8_t>(uv.pos.y)});
    texQuad.primitive.uvC = psyqo::PrimPieces::UVCoordsPadded({static_cast<uint8_t>(uv.pos.x), static_cast<uint8_t>(uv.pos.y + uv.size.y - 1)});
    texQuad.primitive.uvD = psyqo::PrimPieces::UVCoordsPadded({static_cast<uint8_t>(uv.pos.x + uv.size.x - 1), static_cast<uint8_t>(uv.pos.y + uv.size.y - 1)});
    texQuad.primitive.setColor(color);

    m_orderingTables[m_parity].insert(texQuad, otEntry);
}

void Graphics::SetTpage(const Texture* texture, uint16_t otEntry)
{
    auto& tpage = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::TPage>();
    psyqo::PrimPieces::ClutIndex clutIndex(texture->clutX, texture->clutY);
    psyqo::PrimPieces::TPageAttr tPageA;
    tPageA.setPageX(texture->x / TPAGE_WIDTH);
    tPageA.setPageY(texture->y / TPAGE_HEIGHT);
    tPageA.set(texture->colorMode);
    tpage.primitive.attr = tPageA;

    m_orderingTables[m_parity].insert(tpage, otEntry);
}

void Graphics::DrawSprite16x16(const Texture *texture, const psyqo::Vec2 pos, const psyqo::Rect &uv, uint16_t otEntry, psyqo::Color color)
{
    auto& sprite = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::Sprite16x16>();
    if (!texture)
    {
        // TODO: ASSERT !TEX
        syscall_puts("TEXTURE IS NULL !!\n");
        return;
    }
    const int16_t screenX = static_cast<int16_t>((pos.x - m_activeCamera->pos.x).integer() + m_activeCamera->CAMERA_OFFSET.x);
    const int16_t screenY = static_cast<int16_t>((pos.y - m_activeCamera->pos.y).integer() + m_activeCamera->CAMERA_OFFSET.y);

    psyqo::PrimPieces::ClutIndex clutIndex(texture->clutX, texture->clutY);
    sprite.primitive.texInfo.clut = clutIndex;
    sprite.primitive.texInfo.u = uv.pos.u;
    sprite.primitive.texInfo.v = uv.pos.v;
    sprite.primitive.position = {screenX, screenY};
    sprite.primitive.setColor(color);

    m_orderingTables[m_parity].insert(sprite, otEntry);
}
