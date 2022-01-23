#include "pch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture2D.h"

//////////////////////////////////////////////////////////////////////
//  BATCHING REQUIRES TWO TEXTURES TO HAVE THE SAME MEMORY ADDRESS  //
//////////////////////////////////////////////////////////////////////

namespace Olala {

    Renderer2D::CameraData Renderer2D::s_CameraData;

    struct Renderer2DData
    {
        // Quads
        struct QuadVertexData
        {
            glm::vec3 Position;
            glm::vec4 Color;
            glm::vec2 TexCoord;
            float TexIndex;
        };

        struct QuadData
        {
            glm::vec2 Position;
            glm::vec2 Size;
            float Rotation;
            glm::vec4 Color;
        };

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        uint32_t MaxQuadCount = 1000;
        uint32_t MaxTextureSlots = 16;

        QuadVertexData* QuadVertexBufferBase = nullptr;
        QuadVertexData* QuadVertexBufferPtr = nullptr;


        Ref<Shader> TextureShader;

        QuadData* QuadDataArray = nullptr;
        uint32_t QuadCount = 0;
        std::unordered_map<Ref<Texture2D>, std::vector<uint32_t>> QuadTextures;// int as index


        // Circles
        // NOTE : Circle is rendered by drawing them on quads
        struct CircleVertexData
        {
            glm::vec3 Position;
            glm::vec2 LocalQuadCoord;
            glm::vec4 Color;
            float Thickness;
        };

        struct CircleData
        {
            glm::vec2 Position;
            glm::vec4 Color;
            float Radius;
            float Thickness;
        };

        Ref<VertexArray> CircleVertexArray;
        Ref<VertexBuffer> CircleVertexBuffer;
        uint32_t MaxCircleCount = 1000;
        Ref<Shader> CircleShader;

        CircleVertexData* CircleVertexBufferBase = nullptr;
        CircleVertexData* CircleVertexBufferPtr = nullptr;

        CircleData* CircleDataArray = nullptr;
        uint32_t CircleCount = 0;


        // Uniform Buffer
        Ref<UniformBuffer> CameraUniformBuffer;
    };
    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        // Quads
        {
            s_Data.QuadVertexArray = VertexArray::Create();
            s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadCount * 4 * sizeof(Renderer2DData::QuadVertexData));
            s_Data.QuadVertexBuffer->SetLayout({
                { "a_Position" , ShaderDataType::Float3 },
                { "a_Color"    , ShaderDataType::Float4 },
                { "a_TexCoord" , ShaderDataType::Float2 },
                { "a_TexIndex" , ShaderDataType::Float  }
            });

            int indexCount = s_Data.MaxQuadCount * 6;
            uint32_t* indices = new uint32_t[indexCount];

            int cnt = 0;
            for (int i = 0; i < indexCount; i += 6)
            {
                indices[i + 0] = cnt + 0;
                indices[i + 1] = cnt + 1;
                indices[i + 2] = cnt + 2;

                indices[i + 3] = cnt + 2;
                indices[i + 4] = cnt + 3;
                indices[i + 5] = cnt + 0;

                cnt += 4;
            }

            s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
            s_Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, indexCount));
            delete[] indices;

            s_Data.QuadVertexBufferBase = new Renderer2DData::QuadVertexData[s_Data.MaxQuadCount * 4];
            s_Data.QuadDataArray = new Renderer2DData::QuadData[s_Data.MaxQuadCount];

            s_Data.TextureShader = Shader::Create("../Olala/Asset/Shader/Texture.glsl");

            // Set the texture uniform index
            s_Data.TextureShader->Bind();
            int slotIndices[32] = { 0 };
            for (int j = 0; j < 32; j++)
                slotIndices[j] = j;

            s_Data.TextureShader->SetUniformIntArray("u_Textures", slotIndices, 32);
        }

        // Circles
        {
            s_Data.CircleVertexArray = VertexArray::Create();
            s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxCircleCount * 4 * sizeof(Renderer2DData::CircleVertexData));
            s_Data.CircleVertexBuffer->SetLayout({
                { "a_Position"       , ShaderDataType::Float3 },
                { "a_LocalQuadCoord" , ShaderDataType::Float2 },
                { "a_Color"          , ShaderDataType::Float4 },
                { "a_Thickness"      , ShaderDataType::Float  }
            });

            int indexCount = s_Data.MaxCircleCount * 6;
            uint32_t* indices = new uint32_t[indexCount];

            int cnt = 0;
            for (int i = 0; i < indexCount; i += 6)
            {
                indices[i + 0] = cnt + 0;
                indices[i + 1] = cnt + 1;
                indices[i + 2] = cnt + 2;

                indices[i + 3] = cnt + 2;
                indices[i + 4] = cnt + 3;
                indices[i + 5] = cnt + 0;

                cnt += 4;
            }

            s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
            s_Data.CircleVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, indexCount));
            delete[] indices;

            s_Data.CircleVertexBufferBase = new Renderer2DData::CircleVertexData[s_Data.MaxCircleCount * 4];
            s_Data.CircleDataArray = new Renderer2DData::CircleData[s_Data.MaxCircleCount];

            s_Data.CircleShader = Shader::Create("../Olala/Asset/Shader/Circle.glsl");
        }

        // Uniform buffer
        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
    }

    void Renderer2D::ShutDown()
    {
        delete[] s_Data.QuadVertexBufferBase;
        delete[] s_Data.QuadDataArray;

        delete[] s_Data.CircleVertexBufferBase;
        delete[] s_Data.CircleDataArray;
    }

    void Renderer2D::BeginScene(Camera& camera)
    {
        Reset();

        s_CameraData.m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        s_Data.CameraUniformBuffer->SetData(&s_CameraData, sizeof(CameraData));
    }

    void Renderer2D::EndScene()
    {
        s_Data.TextureShader->Bind();

        while (uint32_t indexCount = BatchQuads())
            FlushQuads(indexCount);


        s_Data.CircleShader->Bind();

        if (uint32_t indexCount = BatchCircles())
            FlushCircles(indexCount);
    }

    uint32_t Renderer2D::BatchQuads()
    {
        if (s_Data.QuadTextures.empty())
            return 0;


        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        uint32_t indexCount = 0;

        int length = std::min(s_Data.MaxTextureSlots, s_Data.QuadTextures.size());
        auto it = s_Data.QuadTextures.begin();

        for (int i = 0; i < length; i++, it++)
        {
            indexCount += WriteQuadDataToBuffer(it->second, it->first == nullptr ? -1 : i);

            if (it->first)
                it->first->Bind(i);
            else if (s_Data.MaxTextureSlots < s_Data.QuadTextures.size())
                i--;
        }

        s_Data.QuadTextures.erase(s_Data.QuadTextures.begin(), it);

        return indexCount;
    }

    void Renderer2D::FlushQuads(uint32_t indexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        s_Data.QuadVertexArray->Bind();
        s_Data.QuadVertexBuffer->Bind();
        s_Data.QuadVertexArray->GetIndexBuffer()->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, indexCount);
    }

    uint32_t Renderer2D::WriteQuadDataToBuffer(std::vector<uint32_t>& quadIndices, uint32_t textureIndex)
    {
        constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                                   {  0.5f,  0.5f, 0.0f, 1.0f  },
                                                   {  0.5f, -0.5f, 0.0f, 1.0f  },
                                                   { -0.5f, -0.5f, 0.0f, 1.0f  } };

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };

        uint32_t indexCount = 0;
        for (const auto& index : quadIndices)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(s_Data.QuadDataArray[index].Position, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), glm::radians(s_Data.QuadDataArray[index].Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(s_Data.QuadDataArray[index].Size, 0.0f));

            for (int i = 0; i < 4; i++)
            {
                s_Data.QuadVertexBufferPtr->Position = transform * vertexPositions[i];
                s_Data.QuadVertexBufferPtr->Color = s_Data.QuadDataArray[index].Color;
                s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
                s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
                s_Data.QuadVertexBufferPtr++;
            }

            indexCount += 6;
        }
        return indexCount;
    }

    uint32_t Renderer2D::BatchCircles()
    {
        if (s_Data.CircleCount == 0)
            return 0;

        s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
        uint32_t indexCount = 0;

        // NOTE : Using 1.2f for extra fading spece
        constexpr glm::vec2 vertexPositions[] = { {-1.2f, 1.2f}, {1.2f, 1.2f}, {1.2f, -1.2f}, {-1.f, -1.2f} };

        for (uint32_t i = 0; i < s_Data.CircleCount; i++)
        {
            auto& circleData = s_Data.CircleDataArray[i];
            for (int j = 0; j < 4; j++)
            {
                s_Data.CircleVertexBufferPtr->Position = glm::vec3(circleData.Position + vertexPositions[j] * circleData.Radius, 0.f);
                s_Data.CircleVertexBufferPtr->LocalQuadCoord = vertexPositions[j];
                s_Data.CircleVertexBufferPtr->Color = circleData.Color;
                s_Data.CircleVertexBufferPtr->Thickness = circleData.Thickness;
                s_Data.CircleVertexBufferPtr++;
            }

            indexCount += 6;
        }

        return indexCount;
    }

    void Renderer2D::FlushCircles(uint32_t indexCount)
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
        s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

        s_Data.CircleVertexArray->Bind();
        s_Data.CircleVertexBuffer->Bind();
        s_Data.CircleVertexArray->GetIndexBuffer()->Bind();
        RenderCommand::DrawIndexed(s_Data.CircleVertexArray, indexCount);
    }

    void Renderer2D::Reset()
    {
        s_Data.QuadCount = 0;
        s_Data.CircleCount = 0;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = 0.f;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTextures[nullptr].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = rotation;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTextures[nullptr].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = 0.f;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTextures[texture].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = rotation;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTextures[texture].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawCircle(glm::vec2 position, float radius, float thickness, glm::vec4 color)
    {
        s_Data.CircleDataArray[s_Data.CircleCount].Position = position;
        s_Data.CircleDataArray[s_Data.CircleCount].Radius = radius;
        s_Data.CircleDataArray[s_Data.CircleCount].Thickness = thickness;
        s_Data.CircleDataArray[s_Data.CircleCount].Color = color;

        s_Data.CircleCount++;
    }

}