#pragma once

namespace DeeDeeEngine {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) :m_Time(time) {

		}
		operator float() const { return m_Time; }//�� Timestep ��Ķ�����ʽת��Ϊ float ���͡�

		//����������£�ת��������������� Timestep ���ʵ����Ϊһ�� float ֵ

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f;}
	private:
		float m_Time;
	};
}