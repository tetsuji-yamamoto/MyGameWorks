// グローバル変数宣言ブロック
float4x4 g_mtxWorld; // ワールドマトリックス
float4x4 g_View; // 射影変換マトリックス
float4x4 g_Proj; // プロェクションマトリックス
float4 g_Color; // 色
float4 g_vecLight; // ライトの向きベクトル
float4 g_Deffuse; // 色

// 頂点シェーダ出力構造体
struct VS_OUTPUT
{
	float4 pos : POSITION; // 位置
	float3 nor : NORMAL; // 法線
	float4 col : COLOR0; // 色
	float4 colModel : COLOR1; // 色
	float2 tex : TEXCOORD; // テクスチャ座標
};

// サンプラーを定義 (フィルタリング設定は C++ 側で適用済み)
sampler g_Sampler = sampler_state
{
	AddressU = WRAP;
	AddressV = WRAP;
};

//**********************************************************************************
// 頂点カラーとテクスチャカラーのブレンド計算処理
//**********************************************************************************
float LerpOriginal(float fVtxCol, float fTexCol, float x)
{
    // 頂点とテクスチャの色の差分をどのくらい頂点カラーから引くかという計算
	return fVtxCol + x * (fTexCol - fVtxCol);
}

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

// 頂点シェーダアウトラインプログラムブロック
VS_OUTPUT VS_Outline(VS_OUTPUT input)
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
    
	output.pos.x += norLocal.x * 1.2f;
	output.pos.y += norLocal.y * 1.2f;
	output.pos.z += norLocal.z * 1.2f;
    
    // 法線を出力
	output.nor = norLocal;
    
	// 頂点の色の決定
	output.col = g_Deffuse;
    
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
	float4 out_col = (float4) 0;
	float4 texCol = tex2D(g_Sampler, input.tex);
	float4 in_col = input.col;
	float3 in_nor = input.nor;
    
	float fDotMax, fDotNow;
	fDotMax = (in_nor.x * -in_nor.x) + (in_nor.y * -in_nor.y) + (in_nor.z * -in_nor.z);
	fDotNow = (in_nor.x * g_vecLight.x) + (in_nor.y * g_vecLight.y) + (in_nor.z * g_vecLight.z);
    
	float fMugDot;
	fMugDot = fDotNow / fDotMax;
    
    // 頂点カラーとテクスチャカラーをブレンド
	in_col.r = LerpOriginal(in_col.r, texCol.r, 1.0f);
	in_col.g = LerpOriginal(in_col.g, texCol.g, 1.0f);
	in_col.b = LerpOriginal(in_col.b, texCol.b, 1.0f);
	in_col.a = LerpOriginal(in_col.a, texCol.a, 1.0f);
    
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
float4 PS_Model(VS_OUTPUT input) : COLOR
{
	float4 out_col = (float4) 0;
	float4 in_col = input.col;
	float3 in_nor = input.nor;
    
	float fDotMax, fDotNow;
	fDotMax = (in_nor.x * -in_nor.x) + (in_nor.y * -in_nor.y) + (in_nor.z * -in_nor.z);
	fDotNow = (in_nor.x * g_vecLight.x) + (in_nor.y * g_vecLight.y) + (in_nor.z * g_vecLight.z);
    
	float fMugDot;
	fMugDot = fDotNow / fDotMax;
    
	out_col.r = g_Deffuse.r * fMugDot;
	out_col.g = g_Deffuse.g * fMugDot;
	out_col.b = g_Deffuse.b * fMugDot;
	out_col.a = g_Deffuse.a;
    
	// 頂点カラーを使用
	return out_col;
}

//**********************************************************************************
// ピクセルシェーダプログラムブロック
//**********************************************************************************
float4 PS_ModelTexture(VS_OUTPUT input) : COLOR
{
	float4 out_col = (float4) 0;
	float4 texCol = tex2D(g_Sampler, input.tex);
	float4 in_col = input.col;
	float3 in_nor = input.nor;
    
	float fDotMax, fDotNow;
	fDotMax = (in_nor.x * -in_nor.x) + (in_nor.y * -in_nor.y) + (in_nor.z * -in_nor.z);
	fDotNow = (in_nor.x * g_vecLight.x) + (in_nor.y * g_vecLight.y) + (in_nor.z * g_vecLight.z);
    
	float fMugDot;
	fMugDot = fDotNow / fDotMax;
    
    // 頂点カラーとテクスチャカラーをブレンド
	in_col.r = LerpOriginal(g_Deffuse.r, texCol.r, 1.0f);
	in_col.g = LerpOriginal(g_Deffuse.g, texCol.g, 1.0f);
	in_col.b = LerpOriginal(g_Deffuse.b, texCol.b, 1.0f);
	in_col.a = LerpOriginal(g_Deffuse.a, texCol.a, 1.0f);
    
	out_col.r = in_col.r * fMugDot;
	out_col.g = in_col.g * fMugDot;
	out_col.b = in_col.b * fMugDot;
	out_col.a = in_col.a;
    
	// 頂点カラーを使用
	return out_col;
    
}

// ピクセルシェーダプログラムブロック
float4 PS_Outline(VS_OUTPUT input) : COLOR
{
	// 頂点カラーを使用
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
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
		pixelShader = compile ps_3_0 PS_ModelTexture();
	}

	pass P4_Outline
	{
		vertexShader = compile vs_3_0 VS_Outline();
		pixelShader = compile ps_3_0 PS_Outline();
	}
}