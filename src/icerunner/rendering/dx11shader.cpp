#include <d3dcompiler.h>
#include <cstdio>
#include <cerrno>
#include "dx11renderer.h"

using namespace ice::dx11rendering;

//! Hackishly read a file to Blob to support MinGW (old version of d3dcompiler.h with no D3DCompileFromFile or D3DWriteFileToBlob).
static HRESULT FileToBlob(char const* pFileName, ID3DBlob** blob)
{
    std::FILE* file = std::fopen(pFileName, "rb");
    if (!file) return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

    if(fseek(file, 0, SEEK_END) != 0) { std::fclose(file); return HRESULT_FROM_WIN32(ERROR_SEEK); }

    long fileSize = std::ftell(file);
    if (fileSize == -1) return HRESULT_FROM_WIN32(_doserrno);

    HRESULT res = NO_ERROR;
    if (FAILED(res = D3DCreateBlob((size_t)fileSize, blob))) { std::fclose(file); return res; }

    std::rewind(file);

    long readResult = (long)std::fread((*blob)->GetBufferPointer(), 1, (*blob)->GetBufferSize(), file);
    if (readResult != fileSize) { std::fclose(file); return HRESULT_FROM_WIN32(ERROR_READ_FAULT); }

    return true;
}

static void PrintError(HRESULT result)
{
    wchar_t* buffer;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, result, 0, (LPWSTR)&buffer, 0, NULL);
    printf("%S\n", buffer);
    LocalFree(buffer);
}

bool ShaderProgram::Compile(const CompileDescription& compileDescription, void** ppErrorBuffer, size_t* pErrorSize)
{
    auto& vsDesc = compileDescription.vsDescription;
    auto& psDesc = compileDescription.psDescription;
    auto inputDesc = compileDescription.inputDescription;

    HRESULT res = NO_ERROR;

    // Read vertex shader source into a blob.
    ComPtr<ID3DBlob> pVertexShaderSourceCode;
    if (FAILED(res = FileToBlob(vsFileName_.c_str(), &pVertexShaderSourceCode)))
    {
        printf("Couldn't read vertex shader source file\n");
        return false;
    }

    // Compile it.
    ComPtr<ID3DBlob> pVertexShaderByteCode;
    if (FAILED(res = D3DCompile(pVertexShaderSourceCode->GetBufferPointer(), pVertexShaderSourceCode->GetBufferSize(), nullptr, nullptr, nullptr,
                                vsDesc.pEntryPoint, vsDesc.pShaderModel, D3D10_SHADER_ENABLE_STRICTNESS, 0,  &pVertexShaderByteCode, &errorBuffer_)))
    {
        printf("Problem compiling %s: ", vsFileName_.c_str());
        PrintError(res);
        *ppErrorBuffer = errorBuffer_->GetBufferPointer();
        *pErrorSize = errorBuffer_->GetBufferSize();
        return false;
    }

    // Read pixel shader source into a blob.
    ComPtr<ID3DBlob> pPixelShaderSourceCode;
    if (FAILED(res = FileToBlob(psFileName_.c_str(), &pPixelShaderSourceCode)))
    {
        printf("Couldn't read pixel shader source file\n");
        return false;
    }

    // Compile it.
    ComPtr<ID3DBlob> pPixelShaderByteCode;
    if (FAILED(res = D3DCompile(pPixelShaderSourceCode->GetBufferPointer(), pPixelShaderSourceCode->GetBufferSize(), nullptr, nullptr, nullptr,
                                psDesc.pEntryPoint, psDesc.pShaderModel, D3D10_SHADER_ENABLE_STRICTNESS, 0,  &pPixelShaderByteCode, &errorBuffer_)))
    {
        printf("Problem compiling %s: ", psFileName_.c_str());
        PrintError(res);
        *ppErrorBuffer = errorBuffer_->GetBufferPointer();
        *pErrorSize = errorBuffer_->GetBufferSize();
        return false;
    }

    // Create a vertex shader object from its bytecode.
    if (FAILED(renderer_.device_->CreateVertexShader(pVertexShaderByteCode->GetBufferPointer(), pVertexShaderByteCode->GetBufferSize(), nullptr, &vertexShader_)))
    {
        printf("Failed to create vertex shader\n");
        *ppErrorBuffer = nullptr;
        *pErrorSize = 0;
        return false;
    }

    // Create a pixel shader object from its bytecode.
    if (FAILED(renderer_.device_->CreatePixelShader(pPixelShaderByteCode->GetBufferPointer(), pPixelShaderByteCode->GetBufferSize(), nullptr, &pixelShader_)))
    {
        printf("Failed to create pixel shader\n");
        *ppErrorBuffer = nullptr;
        *pErrorSize = 0;
        return false;
    }

    // Create the input layout for this shader program
    if (FAILED(res = renderer_.device_->CreateInputLayout(inputDesc.pInputElementDescriptions, inputDesc.numInputElementDescriptions,
                                          pVertexShaderByteCode->GetBufferPointer(), pVertexShaderByteCode->GetBufferSize(), &inputLayout_)))
    {
        printf("Failed to create input layout: ");
        PrintError(res);
        *ppErrorBuffer = nullptr;
        *pErrorSize = 0;
        return false;
    }

    return true;
}

void ShaderProgram::Bind()
{
    renderer_.context_->IASetInputLayout(inputLayout_);
    renderer_.context_->VSSetShader(vertexShader_, nullptr, 0);
    renderer_.context_->PSSetShader(pixelShader_, nullptr, 0);
}
