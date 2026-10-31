# MinGW-w64 only has an import library for d3dx8d.dll, the DirectX 8 SDK debug DLL that neither Windows nor Wine
# provide. Link the static d3dx8.lib of the DX8 SDK instead, like the MSVC builds do.
# The dxguid.lib of the DX8 SDK is used too, because libdxguid.a of MinGW-w64 also defines the GUIDs of d3dx8.lib.
add_library(deps_d3dx8 STATIC)

target_sources(deps_d3dx8 PRIVATE D3DX8Runtime.cpp)

set_property(TARGET d3d8lib PROPERTY INTERFACE_LINK_LIBRARIES
    d3d8
    dinput8
    ${dx8_SOURCE_DIR}/dxguid.lib
    ${dx8_SOURCE_DIR}/d3dx8.lib
    deps_d3dx8
)
