#pragma once

#include <vector>
#include <stdint.h>

#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406

struct VertexBufferElement
{
    uint32_t type;
    uint32_t count;
    uint32_t normalized;

    static uint32_t GetSizeOfType(uint32_t type);
};

class VertexBufferLayout
{
    public:
        VertexBufferLayout();
        ~VertexBufferLayout();

        template<typename T>
        void Push(uint32_t count);

        inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; } 
        inline uint32_t GetStride() const { return m_Stride; }

    private:
        std::vector<VertexBufferElement> m_Elements;
        uint32_t m_Stride;
};