#pragma pack_matrix(row_major)

//---------------------------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[
//---------------------------------------------------------------------------------

//�󂯎��e�s�N�Z���̏��
struct PS_INPUT
{
	float4 SPos		: SV_Position;		//	��ʍ��W
	float4 Color	: COLOR;		//	�F

	//�s�N�Z���V�F�[�_�[�Ŗ��邳�v�Z���������̂�
	float3 WPos		: TEXCOORD1;//���[���h���W
	float3 WNormal	: TEXCOORD2;//���[���h�@��
};

//�v�Z�������ʁA��ʂɕ\�������F
struct PS_OUTPUT
{
	float4 Color	: SV_Target0;		//	�F
};

//---------------------------------------------------------------------------------
//�e�s�N�Z���̏���
//---------------------------------------------------------------------------------
PS_OUTPUT main( PS_INPUT In )
{

	//���C�g���W
	float4 LightPos	= { -2.0f, 5.0f, 0.0f, 0.0f };

	PS_OUTPUT Out;

	Out.Color = In.Color;

	//�e�s�N�Z�����烉�C�g�܂ł̃x�N�g��
	float3 dir = LightPos.xyz - In.WPos;

	//���̃x�N�g���̂����K��
	float3 L = normalize( dir );

	//���[���h�@�������K��
	float3 N = normalize( In.WNormal );

	//���̃s�N�Z�����ǂ̂��炢���C�g�̕��������Ă��邩
	float power = max( 0.0f, dot( N, L ) );

	//��ŋ��߂邽������0.0���`1.0���Ȃ̂ł���
	//�����3�i�K
	//�ɕ����Ă݂܂�
	if (power < 0.2f) {
		power = 0.2f;
	}else if (power < 0.7f) {
		power = 0.5f;
	}else {
		power = 0.8f;
	}

	//���̋����̖��邳�ɂ��܂�
	Out.Color = In.Color * power*50;

	//Out.Color.g = 1.0f;
	Out.Color.a = 1.0f;

	Out.Color = float4(N, 1);

	//Out.Color = float4(power.xxx, 1);
	return Out;
}
