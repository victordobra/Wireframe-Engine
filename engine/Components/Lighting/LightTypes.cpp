#include "LightTypes.hpp"
#include "LightingSystem.hpp"

namespace wfe {
    WFE_COMPONENT(AmbientLight)
    WFE_COMPONENT_DEFAULT_SYSTEM(GetLightingSystem)
    WFE_COMPONENT_PROPERTY(ambientColor, PUBLIC)
    WFE_END_COMPONENT(AmbientLight)

    WFE_COMPONENT(DirectionalLight)
    WFE_COMPONENT_DEFAULT_SYSTEM(GetLightingSystem)
    WFE_COMPONENT_PROPERTY(lightColor, PUBLIC)
    WFE_END_COMPONENT(DirectionalLight)

    WFE_COMPONENT(PointLight)
    WFE_COMPONENT_DEFAULT_SYSTEM(GetLightingSystem)
    WFE_COMPONENT_PROPERTY(lightColor, PUBLIC)
    WFE_END_COMPONENT(PointLight)
}