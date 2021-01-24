#include<iostream>
#include<olc_net.h>

enum class CustomMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
	bool FireBullet(float x, float y)
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::FireBullet;
		msg << x << y;
		Send(msg);
	}
};

int main()
{
	CustomClient c;
	c.Connect("community.onelonecoder.com", 60000);
	c.FireBullet(2.5f, 5.0f);
	return 0;

}