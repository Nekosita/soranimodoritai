#pragma pack_matrix(row_major)

#include "dxlib_vs.h"

// [DxLib] MV1���f���̒��_�V�F�[�_�[����
struct VS_INPUT_MV1
{
	float3 position_      : POSITION;		// ���W(���[�J�����)
	float3 normal_        : NORMAL;			// �@��(���[�J�����)
	float4 diffuse_       : COLOR0;			// �f�B�t���[�Y�J���[
	float4 specular_      : COLOR1;			// �X�y�L�����J���[
	float4 uv0_           : TEXCOORD0;		// �e�N�X�`�����W
	float4 uv1_           : TEXCOORD1;		// �T�u�e�N�X�`�����W
};

//---------------------------------------------------------------------------------
//	���_�V�F�[�_�[
//---------------------------------------------------------------------------------
/*
//	�󂯎��e���_�̏��
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	���[�J�����W
	float3 LNormal	: NORMAL;		//	���[�J���@��
	float4 Color	: COLOR;		//	�F
};
*/


//	�s�N�Z���V�F�[�_�[�ɓn�����
struct VS_OUTPUT
{
	float4 SPos		: SV_Position;		//	��ʍ��W
	float4 Color	: COLOR;		//	�F

	//�s�N�Z���V�F�[�_�[�Ŗ��邳�v�Z���������̂�
	float3 WPos		: TEXCOORD1;//���[���h���W
	float3 WNormal	: TEXCOORD2;//���[���h�@��
};
/*
float4x4 World		: register(c94);	//	���[���h�ϊ��s��
float4x4 View		: register(c6);	//	�r���[�ϊ��s��
float4x4 Projection	: register(c2);	//	�v���W�F�N�V�����ϊ��s��
*/
//---------------------------------------------------------------------------------
//�I�e���_�̏���
//---------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT_MV1 In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float3	worldPosition = mul(DxLib_Base.WorldMatrix, float4(In.position_, 1.0));	// �X�L�j���O�v�Z�B���[���h��Ԃ֕ϊ�
	float3	viewPosition = mul(DxLib_Base.ViewMatrix, float4(worldPosition, 1.0));	// �r���[��Ԃ֕ϊ�
	Out.SPos = mul(DxLib_Base.ProjectionMatrix, float4(viewPosition, 1.0));	// �X�N���[����Ԃ֕ϊ�

	//	���[���h�@�����v�Z
//	float3 N = normalize(mul(DxLib_Base.WorldMatrix, float4(In.normal_, 0.0)));
	float3 N = In.normal_;
	
	//�F��n��
	Out.Color = float4(1.0f, 0.5f, 0.5f, 0.8f);
	//Out.Color = In.Color;

	//���[���h���W���s�N�Z���V�F�[�_�[�ɓn��
	Out.WPos = worldPosition;

	//���[���h�@�����s�N�Z���V�F�[�_�[�ɓn��
	Out.WNormal = N;

	return Out;
}
