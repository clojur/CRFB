#pragma once
#include "VprestressBar.h"

/***************************************************************************************


                  ����ԤӦ���ֽ���ࣨȫ��ֻ����һ�����϶��󼴿������ƣ�


****************************************************************************************/

class CDIP_BAR_API CVprestressObjects : public CObject
{
public:
	CVprestressObjects();
	virtual ~CVprestressObjects();

	//�Զ��������ԤӦ���ֽ�
	void AutoDesign(CRigidFrameBridge* brg);
	//���¸�д�����ע���
	void RewriteBarname();

	vector<CVprestressBar*> m_bars;

	double m_Space;   //����ԤӦ���Զ���Ƽ��
	double m_Limit;   //�ָֽ��ԤӦ���ֽ��߲��õķֽ糤��
};


