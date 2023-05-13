
//---------------------------------------------------------------------------------
//	���_�V�F�[�_�[
//---------------------------------------------------------------------------------

//	�󂯎��e���_�̏��
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	���[�J�����W
	float3 LNormal	: NORMAL;		//	���[�J���@��
	float4 Color	: COLOR;		//	�F
};

//	�s�N�Z���V�F�[�_�[�ɓn�����
struct VS_OUTPUT
{
	float4 SPos		: POSITION;		//	��ʍ��W
	float4 Color	: COLOR;		//	�F
};

float4x4 World		: register(c94);	//	���[���h�ϊ��s��
float4x4 View		: register(c6);	//	�r���[�ϊ��s��
float4x4 Projection	: register(c2);	//	�v���W�F�N�V�����ϊ��s��

//---------------------------------------------------------------------------------
//	�e���_�̏���
//---------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//�傫�ȃ��f���ɂ��ĕ`�悵����
	//���[�J�����W��ύX���܂�
	float4 LPos = In.LPos;

	//�@�������ɒ��_���ړ�������
	LPos.xyz += In.LNormal * 0.03f;

	//���W�ϊ�
	float4 WPos = mul(LPos, World);
	float4 CPos = mul(WPos, View);
	Out.SPos = mul(CPos, Projection);


	//Out.Color = In.Color;
	Out.Color = float4(0.5f, 0.5f, 0.5f, 0.5f);

	return Out;

}

