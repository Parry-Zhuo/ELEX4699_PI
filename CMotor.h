class CMotor
{
public:
	CMotor();
	~CMotor();
	void set_pwm_left(int val);
	void set_pwm_right(int val);
	int get_pwm_left();
	int get_pwm_right();
	void stop();
	void forward(float time);
	void backward(float time);
	void left(float time);
	void right(float time);
};