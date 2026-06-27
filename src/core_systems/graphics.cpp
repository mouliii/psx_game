#include "graphics.h"
#include "trig.h"

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
    m_gpu.chain(m_orderingTables[m_parity]);
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
void Graphics::SetActiveFont(Texture *tex)
{
    fontTexture = tex;
}
void Graphics::DrawText(const psyqo::Vec2 pos, const psyqo::Color color, const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    psyqo::Vec2 charPos = pos;
    int16_t w = 8, h = 16;
    psyqo::Vertex uvBase = GetTextureUV(fontTexture);
    for (size_t i = 0; buffer[i] != '\0'; i++)
    {
        int16_t glyph = buffer[i];
        if (glyph == ' '){ // 32
            charPos.x += w;
            continue;
        }
        else if (glyph == '\n'){
            charPos.y += h;
            charPos.x = pos.x;
            continue;
        }
        glyph -= 32;

        int16_t uvx = uvBase.x + (glyph % 16) * 16;
        int16_t uvy = uvBase.y + (glyph / 16) * 16;
        DrawTexturedQuad(fontTexture, psyqo::Vec2(charPos.x, charPos.y), {w,h}, {{uvx, uvy},{15,15}}, 0, color);
        charPos.x += w;
    }
}
// returns UV topleft offset in pixels
psyqo::Vertex Graphics::GetTextureUV(Texture *texture)
{
    //uint16_t TPageVramX2 = (texture->x / TPAGE_WIDTH) * TPAGE_WIDTH;
    //uint16_t TPageVramY = (texture->y / TPAGE_HEIGHT) * TPAGE_HEIGHT;

    int16_t TPageVramX = texture->x & ~(TPAGE_WIDTH - 1);
    int16_t TPageVramY = texture->y & ~(TPAGE_HEIGHT - 1);
    //printf("non: %d, &: %d\n", TPageVramX2, TPageVramX);
    int16_t uvx = (texture->x - TPageVramX) << (2 - texture->colorMode);
    int16_t uvy = texture->y - TPageVramY;

    return {uvx, uvy};
}

void Graphics::DrawTexturedQuad(const Texture *texture, const psyqo::Vec2 pos, const psyqo::Vertex size, const psyqo::Rect uv, uint16_t otEntry, psyqo::Color color)
{
    auto& texQuad = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::TexturedQuad>();
    if (!texture)
    {
        psyqo::Kernel::assert(texture != nullptr, "Texture is null pointer");
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
    texQuad.primitive.uvB = psyqo::PrimPieces::UVCoords({static_cast<uint8_t>(uv.pos.x + uv.size.x), static_cast<uint8_t>(uv.pos.y)});
    texQuad.primitive.uvC = psyqo::PrimPieces::UVCoordsPadded({static_cast<uint8_t>(uv.pos.x), static_cast<uint8_t>(uv.pos.y + uv.size.y)});
    texQuad.primitive.uvD = psyqo::PrimPieces::UVCoordsPadded({static_cast<uint8_t>(uv.pos.x + uv.size.x), static_cast<uint8_t>(uv.pos.y + uv.size.y)});
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

void Graphics::DrawSprite16x16(const Texture *texture, const psyqo::Vec2 pos, const psyqo::Rect uv, uint16_t otEntry, psyqo::Color color)
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

void Graphics::DrawLine(const psyqo::Vec2 start, const psyqo::Vec2 end, psyqo::Color color, uint16_t otEntry)
{
    auto& line = m_primitiveBuffers[m_parity].allocateFragment<psyqo::Prim::Line>();
    line.primitive.pointA = {(int16_t)start.x.integer(), (int16_t)start.y.integer()};
    line.primitive.pointB = {(int16_t)end.x.integer(),   (int16_t)end.y.integer()};
    line.primitive.setColor(color);

    m_orderingTables[m_parity].insert(line, otEntry);
}

void Graphics::DrawCircle(const psyqo::Vec2 pos, const uint16_t radius, const uint16_t segments, psyqo::Color color, uint16_t otEntry)
{
    using namespace psyqo::trig_literals;
    psyqo::Vec2 prevPos;
    psyqo::Angle step = 2.0_pi / segments;
    //
    prevPos.x = pos.x + (mouli::trig.cos(0.0_pi) * radius);
    prevPos.y = pos.y + (mouli::trig.sin(0.0_pi) * radius);
    //
    for (int i = 1; i <= segments; ++i)
    {
        psyqo::Angle angle = step * i;

        psyqo::Vec2 nextPos;
        nextPos.x = pos.x + (mouli::trig.cos(angle) * radius);
        nextPos.y = pos.y + (mouli::trig.sin(angle) * radius);

        DrawLine(prevPos, nextPos, color, otEntry);

        prevPos = nextPos;
    }
    
}
