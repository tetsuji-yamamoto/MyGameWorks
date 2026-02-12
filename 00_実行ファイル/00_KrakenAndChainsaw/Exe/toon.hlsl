#include "ShaderHeader.hlsli"

// グローバル変数宣言ブロック
float4x4	g_mtxWorld;	// ワールドマトリックス
float4x4	g_View;		// 射影変換マトリックス
float4x4	g_Proj;		// プロェクションマトリックス
float4		g_vecLight;	// ライトの向きベクトル
float4		g_Deffuse;	// 色

// サンプラーを定義 (フィルタリング設定は C++ 側で適用済み)
sampler g_Sampler = sampler_state
{
	AddressU = WRAP;
	AddressV = WRAP;
};

//**********************************************************************************
// 頂点シェーダプログラムブロック
//**********************************************************************************
VS_OUTPUT VS_main(VS_OUTPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
    
	float4 in_pos = input.pos; // 位置
	float3 in_nor = input.nor; // 法線
	float4 in_col = input.col; // 色
	float2 in_tex = input.tex; // テクスチャ座標
    
	// 座標変換(ローカル座標と掛け合わせ)
	output.pos = mul(in_pos, g_mtxWorld);
	output.pos = mul(output.pos, g_View);
	output.pos = mul(output.pos, g_Proj);

    // 法線に回転行列を掛ける
	float3 norLocal;
	norLocal.x = (in_nor.x * g_mtxWorld._11) + (in_nor.y * g_mtxWorld._21) + (in_nor.z * g_mtxWorld._31);
	norLocal.y = (in_nor.x * g_mtxWorld._12) + (in_nor.y * g_mtxWorld._22) + (in_nor.z * g_mtxWorld._32);
	norLocal.z = (in_nor.x * g_mtxWorld._13) + (in_nor.y * g_mtxWorld._23) + (in_nor.z * g_mtxWorld._33);
    
	float fDotMax, fDotNow;
	fDotMax = (norLocal.x * -norLocal.x) + (norLocal.y * -norLocal.y) + (norLocal.z * -norLocal.z);
	fDotNow = (norLocal.x * g_vecLight.x) + (norLocal.y * g_vecLight.y) + (norLocal.z * g_vecLight.z);
    
	float fMugDot;
	fMugDot = fDotNow / fDotMax;
    
    // 法線を出力
	output.nor = norLocal;
    
	// 頂点の色の決定
	output.col = in_col;
	output.colModel = g_Deffuse;
    
    // テクスチャ座標
	output.tex = in_tex;

	return output;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Poligon(VS_OUTPUT input) : COLOR
{
	float4 out_col = (float4) 0;
	float4 in_col = input.col;
	float3 in_nor = input.nor;
    
	float fDotMax, fDotNow;
	fDotMax = (in_nor.x * -in_nor.x) + (in_nor.y * -in_nor.y) + (in_nor.z * -in_nor.z);
	fDotNow = (in_nor.x * g_vecLight.x) + (in_nor.y * g_vecLight.y) + (in_nor.z * g_vecLight.z);
    
	float fMugDot;
	fMugDot = fDotNow / fDotMax;
    
	out_col.r = in_col.r * fMugDot;
	out_col.g = in_col.g * fMugDot;
	out_col.b = in_col.b * fMugDot;
	out_col.a = in_col.a;
    
	// 頂点カラーを使用
	return out_col;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_PoligonTexture(VS_OUTPUT input) : COLOR
{
	float4 out_col = input.col;
	float4 tex_col = tex2D(g_Sampler, input.tex);
	float4 in_col = input.col;
    
    // 頂点カラーとテクスチャカラーをブレンド
	out_col.r = lerp(in_col.r, tex_col.r, 0.5f);
	out_col.g = lerp(in_col.g, tex_col.g, 0.5f);
	out_col.b = lerp(in_col.b, tex_col.b, 0.5f);
	out_col.a = lerp(in_col.a, tex_col.a, 0.5f);
	
	// 法線とライト向きの内積を計算
	float fDot = dot(normalize(input.nor), normalize(-g_vecLight.xyz));
	fDot = saturate(fDot);
	
	// 影をつける
	out_col.rgb *= fDot;
	
	// 色をトゥーン調に調整
	out_col = Color_To_Toon(out_col,fDot,3.0f);
	
	// 頂点カラーを使用
	return out_col;
}

//**********************************************************************************
// ライティング無しポリゴン描画
//**********************************************************************************
float4 PS_PoligonTexture_No_Light(VS_OUTPUT input) : COLOR
{
	float4 out_col = input.col;
	float4 tex_col = tex2D(g_Sampler, input.tex);
	float4 in_col = input.col;
	
    // 頂点カラーとテクスチャカラーをブレンド
	out_col = Color_Blend(in_col, tex_col);
	
	// 色をトゥーン調に調整
	out_col = Tex_Color_To_Toon(tex_col, 3.0f);
	
	// 頂点カラーを使用
	return out_col;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Model(VS_OUTPUT input) : COLOR
{
	// 出力変数
	float4 out_col = g_Deffuse;
	
	// 法線とライト向きの内積を計算
	float fDot = dot(normalize(input.nor), normalize(-g_vecLight.xyz));
	fDot = saturate(fDot);
	
	// 光をトゥーン調に調整
	out_col = Color_To_Toon(out_col, fDot, 3.0f);
	
	// 色を返す
	return out_col;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_Model_Texture(VS_OUTPUT input) : COLOR
{
	// 出力変数
	float4 out_col = g_Deffuse;
	
	// テクスチャカラー取得
	float4 tex_col = tex2D(g_Sampler, input.tex);
	
	// 頂点カラーとテクスチャカラーをブレンド
	out_col = Color_Blend(out_col, tex_col);
	
	// 法線とライト向きの内積を計算
	float fDot = dot(normalize(input.nor), normalize(-g_vecLight.xyz));
	fDot = saturate(fDot);
	
	// 光をトゥーン調に調整
	out_col = Color_To_Toon(out_col, fDot, 3.0f);
	
	// 色を返す
	return out_col;
}

//**********************************************************************************
// テクニック宣言
//**********************************************************************************
technique StandardDraw
{
	pass P0_Poligon
	{
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_Poligon();
	}

	pass P1_PoligonTexture
	{
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_PoligonTexture();
	}

	pass P2_Model
	{
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_Model();
	}

	pass P3_ModelTexture
	{
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_Model_Texture();
	}

	pass P4_Polygon_No_Light
	{
		vertexShader = compile vs_3_0 VS_main();
		pixelShader = compile ps_3_0 PS_PoligonTexture_No_Light();
	}
}