
#include "USART1.h"
#include "stdio.h"

class Clcd
{
public:
Clcd(void);
~Clcd(void);
private:

protected:

};


Clcd::Clcd(void)
{
	usart1_configure();	
	printf("构造函数\r\n");
}

Clcd::~Clcd(void)
{
	printf("析造函数\r\n");
}
volatile class Clcd LCD;

int main(void)
{
	
	

	while (1)
	{
	}
}
