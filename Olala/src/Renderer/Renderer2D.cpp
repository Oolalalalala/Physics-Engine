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
        struct QuadVertex
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

        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t QuadBatchingIndexCount = 0;


        Ref<UniformBuffer> CameraUniformBuffer;
        Ref<Shader> TextureShader;


        QuadData* QuadDataArray = nullptr;
        uint32_t QuadCount = 0;
        std::unordered_map<Ref<Texture2D>, std::vector<int>> QuadTexture;// int as index

    };
    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadCount * 4 * sizeof(Renderer2DData::QuadVertex));
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

        auto indexBuffer = IndexBuffer::Create(indices, indexCount);
        delete[] indices;

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);

        s_Data.QuadVertexBufferBase = new Renderer2DData::QuadVertex[s_Data.MaxQuadCount * 4];
        s_Data.QuadDataArray = new Renderer2DData::QuadData[s_Data.MaxQuadCount];

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

        s_Data.TextureShader = Shader::Create("../Olala/Asset/Texture.glsl");
    }

    void Renderer2D::ShutDown()
    {
        delete[] s_Data.QuadVertexBufferBase;
        delete[] s_Data.QuadDataArray;
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

        while (Batch())
            Flush();
    }

    bool Renderer2D::Batch()
    {
        if (s_Data.QuadTexture.empty())
            return false;


        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.QuadBatchingIndexCount = 0;

        int length = std::min(s_Data.MaxTextureSlots,s_Data.QuadTexture.size());
        auto it = s_Data.QuadTexture.begin();

        for (int i = 0; i < length; i++, it++)
        {
            for (int quadIndex : it->second)
            {
                WriteQuadDataToBuffer(quadIndex, it->first == nullptr ? -1 : i);
            }

            if (it->first)
                it->first->Bind(i);
            else if (s_Data.MaxTextureSlots < s_Data.QuadTexture.size())
                i--;
        }

        s_Data.QuadTexture.erase(s_Data.QuadTexture.begin(), it);

        return true;
    }

    void Renderer2D::Flush()
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        s_Data.QuadVertexArray->Bind();
        s_Data.QuadVertexBuffer->Bind();
        s_Data.QuadVertexArray->GetIndexBuffer()->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadBatchingIndexCount);
    }

    void Renderer2D::WriteQuadDataToBuffer(uint32_t index, uint32_t textureIndex)
    { 
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(s_Data.QuadDataArray[index].Position, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(s_Data.QuadDataArray[index].Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                              glm::scale(glm::mat4(1.0f), glm::vec3(s_Data.QuadDataArray[index].Size, 0.0f));

        constexpr glm::vec4 vertexPositions[4] = { { -0.5f,  0.5f, 0.0f, 1.0f  },
                                                   {  0.5f,  0.5f, 0.0f, 1.0f  },
                                                   {  0.5f, -0.5f, 0.0f, 1.0f  },
                                                   { -0.5f, -0.5f, 0.0f, 1.0f  } };

        constexpr glm::vec2 texCoords[4] = { { 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} };

        for (int i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * vertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = s_Data.QuadDataArray[index].Color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr++;
        }


        s_Data.QuadBatchingIndexCount += 6;
    }

    void Renderer2D::Reset()
    {
        s_Data.QuadCount = 0;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = 0.f;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTexture[nullptr].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = rotation;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTexture[nullptr].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = 0.f;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTexture[texture].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

    void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, glm::vec4 color)
    {
        s_Data.QuadDataArray[s_Data.QuadCount].Position = position;
        s_Data.QuadDataArray[s_Data.QuadCount].Size = size;
        s_Data.QuadDataArray[s_Data.QuadCount].Rotation = rotation;
        s_Data.QuadDataArray[s_Data.QuadCount].Color = color;

        s_Data.QuadTexture[texture].push_back(s_Data.QuadCount);

        s_Data.QuadCount++;
    }

}