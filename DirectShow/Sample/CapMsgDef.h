#pragma once
#ifndef CapMsgDef
#define CapMsgDef

const int WM_START = 0x400;    //�˲�������ͷ��Ϣ0x400��ʾ�ľ���1024  
//const int WS_CHILD = 0x40000000L;
//const int WS_VISIBLE = 0x10000000L;
//const int SWP_NOMOVE = 0x2;
//const int SWP_NOZORDER = 0x4;
const int WM_CAP_GET_CAPSTREAMPTR = WM_START + 1;
const int WM_CAP_SET_CALLBACK_ERROR = WM_START + 2;//�����ջش���
const int WM_CAP_SET_CALLBACK_STATUS = WM_START + 3;//�����ջ�״̬
const int WM_CAP_SET_CALLBACK_YIELD = WM_START + 4;//�����ջس���
const int WM_CAP_SET_CALLBACK_FRAME = WM_START + 5;//�����ջؽṹ
const int WM_CAP_SET_CALLBACK_VIDEOSTREAM = WM_START + 6;//�����ջ���Ƶ��
const int WM_CAP_SET_CALLBACK_WAVESTREAM = WM_START + 7;//�����ջ���Ƶ����
const int WM_CAP_GET_USER_DATA = WM_START + 8;//���ʹ��������
const int WM_CAP_SET_USER_DATA = WM_START + 9;//����ʹ��������
const int WM_CAP_DRIVER_CONNECT = WM_START + 10;//������������
const int WM_CAP_DRIVER_DISCONNECT = WM_START + 11;//�Ͽ�������������
const int WM_CAP_DRIVER_GET_NAME = WM_START + 12;//���������������
const int WM_CAP_DRIVER_GET_VERSION = WM_START + 13;//�����������汾
const int WM_CAP_DRIVER_GET_CAPS = WM_START + 14;//�����������ñ��
const int WM_CAP_FILE_SET_CAPTURE_FILE = WM_START + 20;//���ò����ļ�
const int WM_CAP_FILE_GET_CAPTURE_FILE = WM_START + 21;//��ò����ļ�
const int WM_CAP_FILE_ALLOCATE = WM_START + 22;//�����ļ�
const int WM_CAP_FILE_SAVEAS = WM_START + 23;//����ļ�Ϊ
const int WM_CAP_FILE_SET_INFOCHUNK = WM_START + 24;//���ÿ�ʼ�ļ�
const int WM_CAP_FILE_SAVEDIB = WM_START + 25;//�����ļ�
const int WM_CAP_EDIT_COPY = WM_START + 30;//�༭����
const int WM_CAP_SET_AUDIOFORMAT = WM_START + 35;//������Ƶ��ʽ
const int WM_CAP_GET_AUDIOFORMAT = WM_START + 36;//������Ƶ��ʽ
const int WM_CAP_DLG_VIDEOFORMAT = WM_START + 41;//1065 ����Ƶ��ʽ���öԻ���
const int WM_CAP_DLG_VIDEOSOURCE = WM_START + 42;//1066 ���������öԻ������öԱȶ����ȵ�
const int WM_CAP_DLG_VIDEODISPLAY = WM_START + 43;//1067 ����Ƶ��ʾ
const int WM_CAP_GET_VIDEOFORMAT = WM_START + 44;//1068 �����Ƶ��ʽ
const int WM_CAP_SET_VIDEOFORMAT = WM_START + 45;//1069 ������Ƶ��ʽ
const int WM_CAP_DLG_VIDEOCOMPRESSION = WM_START + 46;//1070 ��ѹ�����öԻ���
const int WM_CAP_SET_PREVIEW = WM_START + 50;//����Ԥ��
const int WM_CAP_SET_OVERLAY = WM_START + 51;//���ø���
const int WM_CAP_SET_PREVIEWRATE = WM_START + 52;//����Ԥ������
const int WM_CAP_SET_SCALE = WM_START + 53;//���ÿ̶�
const int WM_CAP_GET_STATUS = WM_START + 54;//���״̬
const int WM_CAP_SET_SCROLL = WM_START + 55;//���þ�
const int WM_CAP_GRAB_FRAME = WM_START + 60;//�����ṹ
const int WM_CAP_GRAB_FRAME_NOSTOP = WM_START + 61;//ֹͣ�����ṹ
const int WM_CAP_SEQUENCE = WM_START + 62;//����
const int WM_CAP_SEQUENCE_NOFILE = WM_START + 63;//ʹ��WM_CAP_SEUENCE_NOFILE��Ϣ��capCaptureSequenceNoFile�꣩�����Բ�������ļ�д�����ݡ�����Ϣ������ϻص�����ʱ���ã����������Ӧ�ó���ֱ��ʹ������Ƶ���ݡ�
const int WM_CAP_SET_SEQUENCE_SETUP = WM_START + 64;//���ð�װ����
const int WM_CAP_GET_SEQUENCE_SETUP = WM_START + 65;//��ð�װ����
const int WM_CAP_SET_MCI_DEVICE = WM_START + 66;//����ý����ƽӿ�
const int WM_CAP_GET_MCI_DEVICE = WM_START + 67;//���ý����ƽӿ� 
const int WM_CAP_STOP = WM_START + 68;//ֹͣ
const int WM_CAP_ABORT = WM_START + 69;//�쳣�ж�
const int WM_CAP_SINGLE_FRAME_OPEN = WM_START + 70;//�򿪵�һ�Ľṹ
const int WM_CAP_SINGLE_FRAME_CLOSE = WM_START + 71;//�رյ�һ�Ľṹ
const int WM_CAP_SINGLE_FRAME = WM_START + 72;//��һ�Ľṹ
const int WM_CAP_PAL_OPEN = WM_START + 80;//����Ƶ
const int WM_CAP_PAL_SAVE = WM_START + 81;//������Ƶ
const int WM_CAP_PAL_PASTE = WM_START + 82;//ճ����Ƶ
const int WM_CAP_PAL_AUTOCREATE = WM_START + 83; //�Զ�����
const int WM_CAP_PAL_MANUALCREATE = WM_START + 84;//�ֶ�����
const int WM_CAP_SET_CALLBACK_CAPCONTROL = WM_START + 85;// �����ջصĴ���


#endif // !CapMsgDef
