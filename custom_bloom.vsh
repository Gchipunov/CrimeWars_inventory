// Vertex Shader (custom_bloom.vsh)
#include "common_vs_fxc.h"

struct VertexInput {
    float4 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct VertexOutput {
    float4 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
};

VertexOutput main(VertexInput i) {
    VertexOutput o;
    o.vPosition = mul(i.vPosition, cModelViewProj);
    o.vTexCoord = i.vTexCoord;
    return o;
}
