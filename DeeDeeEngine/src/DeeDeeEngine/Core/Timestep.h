#pragma once

namespace DeeDeeEngine {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) :m_Time(time) {

		}
		operator float() const { return m_Time; }//将 Timestep 类的对象隐式转换为 float 类型。

		//在这种情况下，转换运算符允许您将 Timestep 类的实例视为一个 float 值

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f;}
	private:
		float m_Time;
	};
}