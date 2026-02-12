// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 pos : POSITION; // 位置
	float3 nor : NORMAL; // 法線
	float4 col : COLOR0; // 色
	float4 colModel : COLOR1; // 色
	float2 tex : TEXCOORD; // テクスチャ座標
};

//**********************************************************************************
// 色の合成
//**********************************************************************************
float4 Color_Blend(float4 col_A,float4 col_B)
{
	float4 out_col = col_A * col_B;
	out_col.a = col_A.a;
	return out_col;
}

//**********************************************************************************
// 頂点カラーとテクスチャカラーのブレンド計算処理
//**********************************************************************************
float LerpOriginal(float fVtxCol, float fTexCol, float x)
{
	// 頂点とテクスチャの色の差分をどのくらい頂点カラーから引くかという計算
	return fVtxCol + x * (fTexCol - fVtxCol);
}

//**********************************************************************************
// 色をトゥーン調に変える処理
//**********************************************************************************
float4 Color_To_Toon(float4 color, float fDot, float stepCount)
{
    // 輝度を計算
	float luminance = dot(fDot, float3(0.299f, 0.587f, 0.114f));

    // トゥーン段階化
	float toonLum = floor(luminance * stepCount) / stepCount;

    // 元の色を明るさ比率でスケール
	float light = 0.0f;
	
	// 下限値を超えていれば作成
	if (luminance > 0.0001f)
	{
		light = fDot * (toonLum / luminance);
	}

	// 色に光を掛ける
	float4 out_col = color * light;
	out_col.rgb += color.rgb * 0.01f;
	out_col.a = color.a;
	
	// 色を出力
	return out_col;
}

//**********************************************************************************
// 色をトゥーン調に変える処理
//**********************************************************************************
float4 Tex_Color_To_Toon(float4 color, float stepCount)
{
    // トゥーン段階化
	float3 toonLum = (float3) 0;
	toonLum.r = floor(color.r * stepCount) / stepCount;
	toonLum.g = floor(color.g * stepCount) / stepCount;
	toonLum.b = floor(color.b * stepCount) / stepCount;

	// 色に光を掛ける
	float4 out_col = color;
	out_col.rgb *= toonLum;
	out_col.rgb += color.rgb * 0.01f;
	out_col.a = color.a;
	
	// 色を出力
	return out_col;
}