#include <cstring>
#include "dx11structs.h"
#include "dx11renderer.h"

using namespace ice::dx11rendering;

bool ShaderConstantBuffer::Describe(const Description& description)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.ByteWidth = (UINT)description.size;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = description.pInitialData;

    HRESULT result = NO_ERROR;
    if (FAILED(result = renderer_.device_->CreateBuffer(&bufferDesc, &resourceData, &buffer_)))
    {
        wchar_t* buffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, result, 0, (LPWSTR)&buffer, 0, NULL);
        printf("Failed to create constant buffer: %S\n", buffer);
        LocalFree(buffer);
        return false;
    }

    size_ = description.size;
    return true;
}

bool ShaderConstantBuffer::Update(const void* pData)
{
    D3D11_MAPPED_SUBRESOURCE subResource;
    if(FAILED(renderer_.context_->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
    {
        printf("Failed to update constant buffer\n");
        return false;
    }

    if(!std::memcpy(subResource.pData, pData, size_))
    {
        printf("Failed to write to constant buffer memory\n");
        return false;
    }

    renderer_.context_->Unmap(buffer_, 0);

    return true;
}

void ShaderConstantBuffer::VSBind()
{
    renderer_.context_->VSSetConstantBuffers(0, 1, buffer_.AddressOf());
}

void ShaderConstantBuffer::PSBind()
{
    renderer_.context_->PSSetConstantBuffers(0, 1, buffer_.AddressOf());
}
