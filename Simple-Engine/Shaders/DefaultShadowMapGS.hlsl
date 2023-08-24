#include "ShadersStructs.hlsli"

[instance(4)]
[maxvertexcount(3)]
void main(triangle GS_IN p[3],
    in uint id : SV_GSInstanceID,
    inout TriangleStream<GS_OUT> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_OUT gs = (GS_OUT) 0;
        gs.pos = mul(float4(p[i].pos.xyz, 1.0f), cascade.viewProjection[id]);
        gs.arrInd = id;
        stream.Append(gs);
    }
}