#include "gamerenderstructs.hpp"
#include "rendering/displaymanager.h"
#include "rendering/dx11structs.h"
#include "constants.hpp"

namespace ice
{

namespace game
{

Background::Background()
    : pRenderer_(nullptr), pConstantBuffer_(nullptr), pRenderPass_(nullptr), pGeometry_(nullptr), animationSpeed_(constants::MENU_BACKGROUND_SPEED)
{}

void Background::Render(bool animate)
{
    if (animate) Animate();
    pConstantBuffer_->Update(&constantBufferData_);
    pRenderer_->Draw(*pRenderPass_);
}

void Background::Animate()
{
    if (constantBufferData_.phaseShift.x <= -1250.0)
        constantBufferData_.phaseShift.x += 1250.0;
    else
        constantBufferData_.phaseShift.x -= animationSpeed_;
    pConstantBuffer_->Update(&constantBufferData_);
}

void Background::SetRenderInfo(dx11rendering::DX11Renderer& renderer, dx11rendering::RenderPass& pass)
{
    pRenderer_ = &renderer;
    pGeometry_ = pass.GetGeometry();
    pRenderPass_ = &pass;
    pConstantBuffer_ = pass.GetPipelineDescription().pPSConstantBuffer;
}

} // namespace game

} // namespace ice
