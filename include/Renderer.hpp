#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};