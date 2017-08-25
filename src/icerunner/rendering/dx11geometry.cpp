#include "dx11renderer.h"
#include "comptrhack.hpp"

using namespace ice::dx11rendering;

bool Geometry::Describe(Geometry::Description* pDesc)
{
    if (!pDesc) return false;

    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = pDesc->numVertices * pDesc->vertexSize;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = pDesc->pVertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = pDesc->numIndices * sizeof(uint32_t);
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = pDesc->pIndices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    if (FAILED(renderer_.device_->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_)))
    {
        printf("Failed to create vertex buffer\n");
        return false;
    }

    if (FAILED(renderer_.device_->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_)))
    {
        printf("Failed to create index buffer\n");
        return false;
    }

    description_ = *pDesc;
    topology_ = pDesc->topology;
    return true;
}

void Geometry::Bind()
{
    unsigned offset = 0;
    unsigned stride = description_.vertexSize;

    renderer_.context_->IASetVertexBuffers(0, 1, vertexBuffer_.AddressOf(), &stride, &offset);
    renderer_.context_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
    renderer_.context_->IASetPrimitiveTopology(topology_);
}
