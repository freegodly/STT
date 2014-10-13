

#include "CommuBase.h"
#include "CommuPipe.h"
#include "CommuTelnet.h"
#include "CommuSerial.h"
CCommuFactory::CCommuFactory()
{
	m_eCommuType = eCTypeUnknow;
}
CCommuFactory::~CCommuFactory()
{

}

//////////////////////////////////////////////////////////////////////////
//����˵��:
//����ͨѶ���ʹ�����Ӧ��ͨ����
//����˵��:
//strSend[IN]��Ҫ���͵�����
//strTXRate[OUT]��������
//strRXRate[OUT]��������
//����˵��:
//��¼�ɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
ICommuBase *CCommuFactory::fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError)
{
	ICommuBase * pResult = 0;
	switch (eCommuType)
	{
	case eCTypePipe:
		{
			pResult = new CCommuPipe();
			break;
		}
	case eCTypeSerial:
		{
			pResult = new CCommuSerial();
			break;
		}
	case eCTypeTelnet:
		{
			pResult = new CCommuTelnet();
			break;
		}
	case eCTypeUnknow:
		{
			strError = "δ֪��ͨѶ����";
			break;
		}
	default:
		{
			strError = "��Ч��ͨѶ����";
			break;
		}
	}
	m_eCommuType = eCommuType;
	return pResult;
}
