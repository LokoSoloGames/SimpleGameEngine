Shader {
	Properties {		

	}
	
	Pass {
		// Queue	"Transparent"
		Cull		None

		BlendRGB 	Add One OneMinusSrcAlpha
		BlendAlpha	Add One OneMinusSrcAlpha
		DepthTest	LessEqual
		// DepthTest	Always
		// DepthWrite	false
		
		VsFunc		vs_main
		PsFunc		ps_main
	}
}

float4x4	sge_matrix_model;
float4x4	sge_matrix_view;
float4x4	sge_matrix_proj;
float4x4	sge_matrix_mvp;

struct VertexIn {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelIn {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


PixelIn vs_main(VertexIn i) {
    	PixelIn o;
	o.position = mul(sge_matrix_mvp, i.position);
    	o.color    = i.color;
    	return o;
}

float4 ps_main(PixelIn i) : SV_TARGET
{
    return i.color;
}
